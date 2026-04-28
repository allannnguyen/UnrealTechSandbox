param(
    [string]$ConfigPath = ".b2backup.json"
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

function Get-ProjectRoot {
    return (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
}

function Get-B2Authorization {
    param(
        [Parameter(Mandatory = $true)][string]$KeyId,
        [Parameter(Mandatory = $true)][string]$ApplicationKey
    )

    $basicAuthBytes = [System.Text.Encoding]::UTF8.GetBytes("{0}:{1}" -f $KeyId, $ApplicationKey)
    $basicAuth = [Convert]::ToBase64String($basicAuthBytes)

    return Invoke-RestMethod `
        -Method Get `
        -Uri "https://api.backblazeb2.com/b2api/v4/b2_authorize_account" `
        -Headers @{ Authorization = "Basic $basicAuth" }
}

function Get-B2UploadUrl {
    param(
        [Parameter(Mandatory = $true)]$AuthorizationResponse,
        [Parameter(Mandatory = $true)][string]$BucketId
    )

    $storageApi = $AuthorizationResponse.apiInfo.storageApi
    $body = @{ bucketId = $BucketId } | ConvertTo-Json -Compress

    return Invoke-RestMethod `
        -Method Post `
        -Uri ("{0}/b2api/v4/b2_get_upload_url" -f $storageApi.apiUrl) `
        -Headers @{
            Authorization = $AuthorizationResponse.authorizationToken
            "Content-Type" = "application/json"
        } `
        -Body $body
}

function Send-B2File {
    param(
        [Parameter(Mandatory = $true)][string]$ZipPath,
        [Parameter(Mandatory = $true)][string]$RemoteFileName,
        [Parameter(Mandatory = $true)]$UploadInfo
    )

    $sha1 = (Get-FileHash -Path $ZipPath -Algorithm SHA1).Hash.ToLowerInvariant()
    $escapedName = [System.Uri]::EscapeDataString($RemoteFileName).Replace("%2F", "/")

    Invoke-RestMethod `
        -Method Post `
        -Uri $UploadInfo.uploadUrl `
        -Headers @{
            Authorization = $UploadInfo.authorizationToken
            "X-Bz-File-Name" = $escapedName
            "Content-Type" = "application/zip"
            "X-Bz-Content-Sha1" = $sha1
        } `
        -InFile $ZipPath
}

function New-BackupArchive {
    param(
        [Parameter(Mandatory = $true)][string]$ProjectRoot,
        [Parameter(Mandatory = $true)]$Config
    )

    $outputDirectory = Join-Path $ProjectRoot $Config.outputDirectory
    $tempDirectory = Join-Path $ProjectRoot ".backup-temp"

    New-Item -ItemType Directory -Force -Path $outputDirectory | Out-Null
    New-Item -ItemType Directory -Force -Path $tempDirectory | Out-Null

    $timestamp = Get-Date -Format "yyyy-MM-dd_HH-mm-ss"
    $archiveName = "TechSandbox_{0}.zip" -f $timestamp
    $archivePath = Join-Path $outputDirectory $archiveName
    $stagingPath = Join-Path $tempDirectory ("staging_{0}" -f $timestamp)

    if (Test-Path $stagingPath) {
        Remove-Item -Recurse -Force $stagingPath
    }

    New-Item -ItemType Directory -Force -Path $stagingPath | Out-Null

    foreach ($relativePath in $Config.includePaths) {
        $sourcePath = Join-Path $ProjectRoot $relativePath
        if (-not (Test-Path $sourcePath)) {
            Write-Warning ("Skipping missing path: {0}" -f $relativePath)
            continue
        }

        $destinationPath = Join-Path $stagingPath $relativePath
        $destinationParent = Split-Path -Parent $destinationPath
        if ($destinationParent) {
            New-Item -ItemType Directory -Force -Path $destinationParent | Out-Null
        }

        Copy-Item -Recurse -Force -Path $sourcePath -Destination $destinationPath
    }

    if (Test-Path $archivePath) {
        Remove-Item -Force $archivePath
    }

    Compress-Archive -Path (Join-Path $stagingPath "*") -DestinationPath $archivePath -CompressionLevel Optimal
    Remove-Item -Recurse -Force $stagingPath

    return $archivePath
}

$projectRoot = Get-ProjectRoot
$resolvedConfigPath = Join-Path $projectRoot $ConfigPath

if (-not (Test-Path $resolvedConfigPath)) {
    throw "Missing config file: $resolvedConfigPath. Copy .b2backup.example.json to .b2backup.json and fill in your Backblaze values."
}

$config = Get-Content -Raw -Path $resolvedConfigPath | ConvertFrom-Json
$archivePath = New-BackupArchive -ProjectRoot $projectRoot -Config $config

$authorization = Get-B2Authorization -KeyId $config.keyId -ApplicationKey $config.applicationKey
$uploadInfo = Get-B2UploadUrl -AuthorizationResponse $authorization -BucketId $config.bucketId

$remotePrefix = [string]$config.remotePrefix
if ([string]::IsNullOrWhiteSpace($remotePrefix)) {
    $remoteFileName = [System.IO.Path]::GetFileName($archivePath)
}
else {
    $remoteFileName = "{0}/{1}" -f $remotePrefix.TrimEnd('/'), [System.IO.Path]::GetFileName($archivePath)
}

$uploadResponse = Send-B2File -ZipPath $archivePath -RemoteFileName $remoteFileName -UploadInfo $uploadInfo

Write-Host ("Created backup: {0}" -f $archivePath)
Write-Host ("Uploaded to B2 as: {0}" -f $remoteFileName)
Write-Host ("Backblaze file ID: {0}" -f $uploadResponse.fileId)
