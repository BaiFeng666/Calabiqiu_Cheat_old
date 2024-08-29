#include "class_ue.h"
#include "Engine.h"
#include <basetsd.h>
#include <stdio.h>

std::string AActor::GetName()
{
	uint32_t Id = *(uint32_t*)((uint8_t*)this + 0x18);
	return ::GetName(Id);
}

bool AActor::IsVisible(AActor* LocalPlayer, Bone BoneIndex, Vector3 camPos)
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.push(LocalPlayer, this);

	Vector3 End = GetBoneLocation(this->Mesh, BoneIndex);
	bool ret = LineTraceSingle(camPos, End, ActorsToIgnore);

	ActorsToIgnore.Pop();
	return !ret;
}
