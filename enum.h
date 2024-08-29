#pragma once
#include "base.h"


// Enum  POLYGON.EGunShootingType
enum class EGunShootingType : uint8_t
{
	BOLT = 0,
	SEMI_AUTO = 1,
	AUTO = 2,
	EGunShootingType_MAX = 3
};

enum Bone {

	Root = 0,
	pelvis = 8,
	spine_01 = 9,
	spine_02 = 10,
	spine_03 = 11,
	neck_01 = 62,
	Head = 6,

	upperarm_l = 13,
	lowerarm_l = 14,
	hand_l = 15,

	upperarm_r = 38,
	lowerarm_r = 39,
	hand_r = 40,

	thigh_l = 64,
	calf_l = 65,
	foot_l = 67,

	thigh_r = 70,
	calf_r = 71,
	foot_r = 73
};