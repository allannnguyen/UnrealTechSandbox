# Backblaze B2 Backup Setup

This project includes a low-effort backup script that:

1. Creates a zip of the important Unreal project files.
2. Uploads that zip to a Backblaze B2 bucket.

## First-time setup

1. Create a Backblaze B2 bucket.
2. Create an application key with access to that bucket and `writeFiles`.
3. Copy `.b2backup.example.json` to `.b2backup.json`.
4. Fill in:
   - `keyId`
   - `applicationKey`
   - `bucketId`
   - optional `remotePrefix`
5. Run:

```powershell
powershell -ExecutionPolicy Bypass -File .\Scripts\Backup-ToB2.ps1
```

## What gets backed up

By default the zip includes:

- `Config/`
- `Content/`
- `Source/`
- `TechSandbox.uproject`
- `README.md`
- `.gitignore`
- `.gitattributes`

Generated Unreal folders such as `Binaries/`, `DerivedDataCache/`, `Intermediate/`, and `Saved/` are intentionally excluded.

## Notes

- The private config file `.b2backup.json` is ignored by Git.
- Backup zips are written to `Backups/` before upload.
- You can add more paths by editing `includePaths` in `.b2backup.json`.

## Backblaze docs

- [Authorize account](https://www.backblaze.com/apidocs/b2-authorize-account)
- [Get upload URL](https://www.backblaze.com/apidocs/b2-get-upload-url)
- [Upload file](https://www.backblaze.com/apidocs/b2-upload-file)
