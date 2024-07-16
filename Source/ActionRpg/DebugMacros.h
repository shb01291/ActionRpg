#pragma once

#include "DrawDebugHelpers.h"
#define DRAW_SPHERE_SingleFrame(Location)if(GetWorld()) DrawDebugSphere(GetWorld(),Location, 25.f,12, FColor::Red, false, -1.f);
#define DRAW_SPHERE_COLOR(Location, Color) if(GetWorld()) DrawDebugSphere(GetWorld(),Location, 8.f,12, Color, false, 3.f);
#define DRAW_SPHERE(Location) if(GetWorld()) DrawDebugSphere(GetWorld(),Location, 25.f,12, FColor::Red, true);
