#include "stdafx.h"
#include "Vector3.h"

float Vector3::Dist(const Vector3 & V1, const Vector3 & V2)
{
	return sqrtf(DistSquared(V1, V2));
}

float Vector3::DistSquared(const Vector3 & V1, const Vector3 & V2)
{
	float xLength = V2.X - V1.X;
	float yLength = V2.Y - V1.Y;
	float zLength = V2.Z - V1.Z;
	return xLength * xLength + yLength * yLength + zLength * zLength;
}

float Vector3::Dot(const Vector3 & V1, const Vector3 & V2)
{
	return V1.X*V2.X + V1.Y*V2.Y + V1.Z*V2.Z;
}

Vector3 Vector3::Make2DPoint(float InX, float InY)
{
	return Vector3(InX, InY);
}

Vector3 Vector3::Make2DVector(float InX, float InY)
{
	return Vector3(InX, InY, 0);
}

Vector3 Vector3::operator*(const Matrix3 & Mat) const
{
	Vector3 result;
	result.X = X * Mat._11 + Y * Mat._12 + Z * Mat._13;
	result.Y = X * Mat._21 + Y * Mat._22 + Z * Mat._23;
	result.Z = X * Mat._31 + Y * Mat._32 + Z * Mat._33;
	return result;
}

Vector3 Vector3::operator-(const Vector3 & Other) const
{
	Vector3 result;
	result.X = X - Other.X;
	result.Y = Y - Other.Y;
	result.Z = Z - Other.Z;
	return result;
}

Vector3 Vector3::operator+(const Vector3 & Other) const
{
	Vector3 result;
	result.X = X + Other.X;
	result.Y = Y + Other.Y;
	result.Z = Z + Other.Z;
	return result;
}

Vector3 Vector3::operator*(float Scale) const
{
	Vector3 result;
	result.X = X * Scale;
	result.Y = Y * Scale;
	result.Z = Z * Scale;
	return result;

}

bool Vector3::Equals(const Vector3 & V, float tolerance) const
{
	return 
		(fabs(X - V.X) <= tolerance) && 
		(fabs(Y - V.Y) <= tolerance) && 
		(fabs(Z - V.Z) <= tolerance);
}
