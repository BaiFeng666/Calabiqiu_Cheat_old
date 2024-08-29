#pragma once
#include <cstdint>
#include "enum.h"

typedef struct _D3DMATRIX {
	union {
		struct {
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;
		};
		float m[4][4];
	};
} D3DMATRIX;


//¾ØÕó
struct FMatrix
{
	float _11, _12, _13, _14;
	float _21, _22, _23, _24;
	float _31, _32, _33, _34;
	float _41, _42, _43, _44;

	FMatrix operator*(const FMatrix& other)
	{
		FMatrix NewMatrix;
		NewMatrix._11 = this->_11 * other._11 + this->_12 * other._21 + this->_13 * other._31 + this->_14 * other._41;
		NewMatrix._12 = this->_11 * other._12 + this->_12 * other._22 + this->_13 * other._32 + this->_14 * other._42;
		NewMatrix._13 = this->_11 * other._13 + this->_12 * other._23 + this->_13 * other._33 + this->_14 * other._43;
		NewMatrix._14 = this->_11 * other._14 + this->_12 * other._24 + this->_13 * other._34 + this->_14 * other._44;
		NewMatrix._21 = this->_21 * other._11 + this->_22 * other._21 + this->_23 * other._31 + this->_24 * other._41;
		NewMatrix._22 = this->_21 * other._12 + this->_22 * other._22 + this->_23 * other._32 + this->_24 * other._42;
		NewMatrix._23 = this->_21 * other._13 + this->_22 * other._23 + this->_23 * other._33 + this->_24 * other._43;
		NewMatrix._24 = this->_21 * other._14 + this->_22 * other._24 + this->_23 * other._34 + this->_24 * other._44;
		NewMatrix._31 = this->_31 * other._11 + this->_32 * other._21 + this->_33 * other._31 + this->_34 * other._41;
		NewMatrix._32 = this->_31 * other._12 + this->_32 * other._22 + this->_33 * other._32 + this->_34 * other._42;
		NewMatrix._33 = this->_31 * other._13 + this->_32 * other._23 + this->_33 * other._33 + this->_34 * other._43;
		NewMatrix._34 = this->_31 * other._14 + this->_32 * other._24 + this->_33 * other._34 + this->_34 * other._44;
		NewMatrix._41 = this->_41 * other._11 + this->_42 * other._21 + this->_43 * other._31 + this->_44 * other._41;
		NewMatrix._42 = this->_41 * other._12 + this->_42 * other._22 + this->_43 * other._32 + this->_44 * other._42;
		NewMatrix._43 = this->_41 * other._13 + this->_42 * other._23 + this->_43 * other._33 + this->_44 * other._43;
		NewMatrix._44 = this->_41 * other._14 + this->_42 * other._24 + this->_43 * other._34 + this->_44 * other._44;

		return NewMatrix;
	}
};

struct Vector2
{
	float x, y;

	Vector2() {
		this->x = 0;
		this->y = 0;
	}

	Vector2(float x, float y) {
		this->x = x;
		this->y = y;
	}
};
struct Vector3 : public Vector2
{
	float z;

	Vector3() :Vector2(0, 0), z(0) {}

	Vector3(float x, float y, float z) : Vector2(x, y), z(z) {}

	Vector3 operator+(Vector3 v) {
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	Vector3 operator-(Vector3 v) {
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	Vector3 operator*(float v) {
		return Vector3(x * v, y * v, z * v);
	}

	float Dot(Vector3 v) {
		return x * v.x + y * v.y + z * v.z;
	}
};

struct Vecotr4 : public Vector3
{
	float w;
};

struct FGameplayAttributeData
{
	char                                          pa_0000[0x0008];                                               // 0x0000(0x0008)
	float                                         BaseValue;                                                    // 0x0008(0x0004)
	float                                         CurrentValue;                                                 // 0x000C(0x0004)
};

struct FHitResult
{
	char pa_00[0x008B];
};

struct FLinearColor
{
	float R, G, B, A;
};

struct BoneName
{
	uint32_t Name;
	int A;
	int B;

};
struct FTransform
{
	Vecotr4 Rotation;
	Vecotr4 Translation;
	Vecotr4 Scale3D;

	FMatrix ToMatrixWithScale();
};

struct FNameEntry
{
	uint16_t bIsWide : 1;
	uint16_t LowercaseProbeHash : 5;
	uint16_t Len : 10;

	union
	{
		char AnsName[1024];
		wchar_t WideName[1024];
	};
};

struct FMinimalViewInfo
{
	Vector3                                       Location;                                                     // 0x0000(0x0018)
	Vector3                                       Rotation;                                                     // 0x0018(0x0018)
	float                                         FOV;                                                          // 0x0030(0x0004)
};

struct FCameraCacheEntry
{
	float                                         Timestamp;                                                    // 0x0000(0x0004)
	char                                          pa_0004[0x000C];                                               // 0x0004(0x000C)
	FMinimalViewInfo                              POV;                                                          // 0x0010(0x0620)
};