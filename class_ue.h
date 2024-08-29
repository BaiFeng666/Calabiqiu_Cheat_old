#pragma once
#include "struct_ue.h"
#include <string>

#pragma pack(1)
template <typename T>
class TArray {
public:
	void Initialize()
	{
		Data = new T[2];
		Count = 0;
		Max = 0;
	}

	void push(T Actor, T Actor1)
	{
		Data = new T[2];
		Data[0] = Actor;
		Data[1] = Actor1;
		Count = 2;
		Max = 2;
	};

	void Pop()
	{
		delete[] Data;
	};

	T* Data;
	int Count;
	int Max;
};
/*
class USkeletalMesh {
public:
	char pa_00[0x2E0];
	TArray<BoneName> Names;
};


class USceneComponent {
public:
	char pa_00[0x240];
	FTransform ComponentToWorld;//0x240 size:0x60
};

class USkinnedMeshComponent : public USceneComponent {
public:
	char pa_02A0[0x308];	//0x2A0
	USkeletalMesh* SkeletalMesh;// 0x05A8(0x0008)
	char pa_05B0[0x50];	//0x5B0
	TArray<FTransform> BoneTransform1; //600
	TArray<FTransform> BoneTransform2; //610

};
*/

class UCyCharacterAttributeSet
{
public:
	char                                          pa_0000[0x0050];
	FGameplayAttributeData                        ViewRadius;                                                   // 0x0050(0x0010)
	FGameplayAttributeData                        Health;                                                       // 0x0060(0x0010)
	FGameplayAttributeData                        HealthMax;                                                    // 0x0070(0x0010)
	FGameplayAttributeData                        Armor;                                                        // 0x0080(0x0010)
	FGameplayAttributeData                        ArmorMax;                                                     // 0x0090(0x0010)
	FGameplayAttributeData                        ExtraArmor;                                                   // 0x00A0(0x0010)
	FGameplayAttributeData                        ExtraArmorMax;                                                // 0x00B0(0x0010)
	FGameplayAttributeData                        DamageFactor;                                                 // 0x00C0(0x0010)
	FGameplayAttributeData                        SpeedFactor;                                                  // 0x00D0(0x0010)
	FGameplayAttributeData                        JumpSpeedFactor;                                              // 0x00E0(0x0010)
	FGameplayAttributeData                        Damage;                                                       // 0x00F0(0x0010)
	FGameplayAttributeData                        TakeHealthDamage;                                             // 0x0100(0x0010)
	FGameplayAttributeData                        TakeExtraArmorDamage;                                         // 0x0110(0x0010)
	FGameplayAttributeData                        TakeArmorDamage;                                              // 0x0120(0x0010)
};

class APlayerState
{
public:
	char pa00[0x0320];
	class AActor* PawnPrivate;// 0x0320(0x0008)
	char pa328[0x0E0];
	char AttributeTeamID; // 0x0408(0x0001)
	char pa409[0x287];	// 0x0419(0x0117)
	UCyCharacterAttributeSet* CyAttributeSet; // 0x0690(0x0008)
};

class AActor {
public:
	char pa_00[0x2E0];
	APlayerState* PlayerState;		//0x02E0
	char pa_2B8[0x10];
	class AController* Controller; // 0x02F8
	char pa_0300[0x20];
	void* Mesh;	// 0x0320

	std::string GetName();

	bool IsVisible(AActor* LocalPlayer, Bone BoneIndex, Vector3 camPos);
};

class ULevel {
public:
	char pa_00[0x0098];
	TArray<AActor*> Actors;
};

class APlayerCameraManager {
public:
	char pa_00[0x2270];
	FCameraCacheEntry LastFrameCameraCachePrivate;
};

class AController {
public:
	char pa_00[0x2F0];
	AActor* pawn;//0x2F0
	char pa_0300[0x30];
	float ViewY;//0x328
	float ViewX;//0x32C  360 + view
	char pa_330[0x28];
	APlayerCameraManager* PlayerCameraManager;//0x0358
};

class UPlayer {
public:
	char pa00[0x30];
	AController* PlayerController;
};

class ULocalPlayer : public UPlayer {
public:
};

class AGameStateBase
{
public:
	char pa00[0x02D8];
	TArray<APlayerState*> PlayerArray;// 0x02D8(0x0010)
};

class UGameInstance
{
public:
	char pa_00[0x38];
	TArray<ULocalPlayer*> LocalPlayers;
};

class UWorld {
public:
	char pa_00[0x30];
	ULevel* PersistentLevel;		//0x0030
	char pa_0038[0xF8];
	AGameStateBase* GameState;		//0x0130(0x0008)
	char pa_00138[0x50];
	UGameInstance* OwningGameInstance; //0x0188
};

#pragma pack()