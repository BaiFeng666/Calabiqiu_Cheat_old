#pragma once
#include "base.h"
#include "struct_ue.h"
#include "class_ue.h"

//typedef bool (WINAPI* fn_ProjectWorldLocationToScreen)(void* pthis, Vector3 WorldLocation, Vector2& ScreenLocation, bool bPlayerViewportRelative);
using fn_ProjectWorldLocationToScreen = bool(WINAPI*)(void* pthis, Vector3 WorldLocation, Vector2& ScreenLocation, bool bPlayerViewportRelative);

//typedef FMatrix* (WINAPI* fn_GetBoneMatrix)(void* pthis, FMatrix* Matrix, int32_t BoneIndex);
using fn_GetBoneMatrix = FMatrix*(WINAPI*)(void* pthis, FMatrix* Matrix, int32_t BoneIndex);

//typedef bool (WINAPI* fn_LineTraceSingle)(UWorld* WorldContextObject, Vector3 Start, Vector3 End, char TraceChannel, bool bTraceComplex, TArray<AActor*>& ActorsToIgnore, int DrawDebugType, FHitResult& OutHit, bool bIgnoreSelf, FLinearColor TraceColor, FLinearColor TraceHitColor, float DrawTime);
using fn_LineTraceSingle = bool(WINAPI*)(UWorld* WorldContextObject, Vector3 Start, Vector3 End, int TraceChannel,
	bool bTraceComplex, TArray<AActor*>& ActorsToIgnore, int DrawDebugType, FHitResult& OutHit, bool bIgnoreSelf, FLinearColor
	TraceColor, FLinearColor TraceHitColor, float DrawTime);

void EngineInit();

string GetName(uint32_t id);

UWorld* GetWorld();

bool WorldToScreen(Vector3 WorldLocation, Vector2& ScreenLocation, void* playerController);

//Vector2 WorldToScreen(Vector3 world_location, Vector3 cameraPos, Vector3 cameraRot, float cameraFov);
//
//bool WorldToScreen(Vector3 world_location, Vector2& screen_location, Vector3 cameraPos, Vector3 cameraRot, float cameraFov);
//
//bool WorldToScreen(Vector3 world_location, Vector2& screen_location, const FMinimalViewInfo& camera);

Vector3 GetBoneLocation(void* Mesh, int32_t BoneIndex);

//Vector3 GetAimRotation();

bool LineTraceSingle(Vector3 Start, Vector3 End, TArray<AActor*>& ActorsToIgnore);



