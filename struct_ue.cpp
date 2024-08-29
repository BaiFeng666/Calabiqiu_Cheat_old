#include "struct_ue.h"

FMatrix FTransform::ToMatrixWithScale()
{
	FMatrix m;

	m._41 = Translation.x;
	m._42 = Translation.y;
	m._43 = Translation.z;

	float x2 = Rotation.x + Rotation.x;
	float y2 = Rotation.y + Rotation.y;
	float z2 = Rotation.z + Rotation.z;

	float xx2 = Rotation.x * x2;
	float yy2 = Rotation.y * y2;
	float zz2 = Rotation.z * z2;
	m._11 = (1.0f - (yy2 + zz2)) * Scale3D.x;
	m._22 = (1.0f - (xx2 + zz2)) * Scale3D.y;
	m._33 = (1.0f - (xx2 + yy2)) * Scale3D.z;


	float yz2 = Rotation.y * z2;
	float wx2 = Rotation.w * x2;
	m._32 = (yz2 - wx2) * Scale3D.z;
	m._23 = (yz2 + wx2) * Scale3D.y;


	float xy2 = Rotation.x * y2;
	float wz2 = Rotation.w * z2;
	m._21 = (xy2 - wz2) * Scale3D.x;
	m._12 = (xy2 + wz2) * Scale3D.x;


	float xz2 = Rotation.x * z2;
	float wy2 = Rotation.w * y2;
	m._31 = (xz2 + wy2) * Scale3D.z;
	m._13 = (xz2 - wy2) * Scale3D.x;

	m._14 = 0.0f;
	m._24 = 0.0f;
	m._34 = 0.0f;
	m._44 = 1.0f;

	return m;
}
