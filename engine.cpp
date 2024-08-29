#include "engine.h"
#include "ImGui/imgui.h"

uint8_t* GameBase = NULL;
uint8_t** GName = NULL;
UWorld** GWorld = NULL;

fn_GetBoneMatrix _GetBoneMatrix = NULL;
fn_LineTraceSingle _LineTraceSingle = NULL;	//ÉäÏßcall
fn_ProjectWorldLocationToScreen _ProjectWorldLocationToScreen = NULL;

void EngineInit()
{
	GameBase = (uint8_t*)GetModuleHandleA(0);
	printf("GameBase:%p\n", GameBase);
	GWorld = (UWorld**)(GameBase + 0x5A76A68);
	printf("GWorld:%p\n", GWorld);
	GName = (uint8_t**)(GameBase + 0x5D7DF40);
	printf("GName:%p\n", GName);

	_GetBoneMatrix = (fn_GetBoneMatrix)(GameBase + 0x2FADCE0);
	_LineTraceSingle = (fn_LineTraceSingle)(GameBase + 0x314ED60);
	_ProjectWorldLocationToScreen = (fn_ProjectWorldLocationToScreen)(GameBase + 0x30908F0);
}

string GetName(uint32_t id)
{
	uint32_t block = id >> 16;
	uint32_t offset = id & 65535;

	FNameEntry* info = (FNameEntry*)(GName[2 + block] + 2 * offset);
	return string(info->AnsName, info->Len);
}

UWorld* GetWorld()
{
	return *GWorld;
}

bool WorldToScreen(Vector3 WorldLocation, Vector2& ScreenLocation, void* playerController)
{
	//void* PlayerColl = GetWorld()->OwningGameInstance->LocalPlayers.Data[0]->PlayerController;
	return _ProjectWorldLocationToScreen(playerController, WorldLocation, ScreenLocation, false);
}

Vector3 GetBoneLocation(void* Mesh, int32_t BoneIndex)
{
	Vector3 Pos;
	FMatrix Matrix;

	_GetBoneMatrix(Mesh, &Matrix, BoneIndex);

	Pos.x = Matrix._41;
	Pos.y = Matrix._42;
	Pos.z = Matrix._43;

	return Pos;
}

//Vector3 GetAimRotation()
//{
//	Vector3 aimRota{};
//	if (!localPlayer || !aimActor)
//		return aimRota;
//	Vector3 EnemyPos = GetBoneLocation(aimActor->Mesh, Bone::Head);
//	Vector3 CameraPos = localPlayer->PlayerCameraManager->LastFrameCameraCachePrivate.POV.Location;
//
//	Vector3 diff{ EnemyPos.x - CameraPos.x,
//			  EnemyPos.y - CameraPos.y,
//			  EnemyPos.z - CameraPos.z,
//	};
//
//	aimRota.y = atan2(diff.y, diff.x) * 180 / 3.1415926;
//	aimRota.x = atan2(diff.z, sqrt(diff.x * diff.x + diff.y * diff.y)) * 180 / 3.1415926;
//	return aimRota;
//}

bool LineTraceSingle(Vector3 Start, Vector3 End, TArray<AActor*>& ActorsToIgnore)
{
	FHitResult OutHit;
	return _LineTraceSingle(GetWorld(), Start, End, 0, true, ActorsToIgnore, 0, OutHit, true, { 0,0,0,0 }, { 0,0,0,0 }, 0.f);
}
