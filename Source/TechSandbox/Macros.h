#pragma once

#define DRAW_LINE(StartLocation, EndLocation) if (GetWorld()) DrawDebugLine(World, StartLocation, EndLocation, FColor::Red, true, -1.f, 0, 1.f);
#define DRAW_SPHERE(Location, Radius) if (GetWorld()) DrawDebugSphere(World, Location, Radius, 12, FColor::Red, true, 1.f, 0, 1.f);