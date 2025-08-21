#pragma once

#include "DrawDebugHelpers.h"

#define DRAW_SPHERE(location) if(GetWorld()) DrawDebugSphere(GetWorld(), location, 25.f, 12, FColor::Red, true);
#define DRAW_SPHERE_SINGLE_FRAME(location) if(GetWorld()) DrawDebugSphere(GetWorld(), location, 25.f, 12, FColor::Red, false, -1.f);
#define DRAW_COLOR_SPHERE(location, color) if(GetWorld()) DrawDebugSphere(GetWorld(), location, 25.f, 12, color, true);
#define DRAW_COLOR_SPHERE_DURATION(location, color, duration) if(GetWorld()) DrawDebugSphere(GetWorld(), location, 25.f, 12, color, false, duration);
#define DRAW_COLOR_SPHERE_DURATION_RADIUS(location, color, duration, radius) if(GetWorld()) DrawDebugSphere(GetWorld(), location, radius, 12, color, false, duration);
#define DRAW_LINE(startLocation, endLocation) if(GetWorld()) DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Red, true, -1.f, 0, 1.f);
#define DRAW_LINE_SINGLE_FRAME(startLocation, endLocation) if(GetWorld()) DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Red, false, -1.f, 0, 1.f);
#define DRAW_POINT(location) if(GetWorld()) DrawDebugPoint(GetWorld(), location, 15.f, FColor::Red, true);
#define DRAW_POINT_SINGLE_FRAME(location) if(GetWorld()) DrawDebugPoint(GetWorld(), location, 15.f, FColor::Red, false, -1.f);
#define DRAW_VECTOR(startLocation, endLocation) if(GetWorld()) \
	{ \
		DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Red, true, -1.f, 0, 1.f); \
		DrawDebugPoint(GetWorld(), endLocation, 15.f, FColor::Red, true); \
	}
#define DRAW_VECTOR_SINGLE_FRAME(startLocation, endLocation) if(GetWorld()) \
	{ \
		DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Red, false, -1.f, 0, 1.f); \
		DrawDebugPoint(GetWorld(), endLocation, 15.f, FColor::Red, false, -1.f); \
	}
#define DRAW_ARROW(startLocation, endLocation) if(GetWorld()) DrawDebugDirectionalArrow(GetWorld(), startLocation, endLocation, 15.f, FColor::Red, true);
#define DRAW_ARROW_SINGLE_FRAME(startLocation, endLocation) if(GetWorld()) DrawDebugDirectionalArrow(GetWorld(), startLocation, endLocation, 15.f, FColor::Red, false, -1.f);