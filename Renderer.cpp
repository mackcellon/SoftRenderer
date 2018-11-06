
#include "stdafx.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"

#include "Vector.h"
#include "IntPoint.h"

bool IsInRange(int x, int y);
void PutPixel(int x, int y);

bool IsInRange(int x, int y)
{
	return (abs(x) < (g_nClientWidth / 2)) && (abs(y) < (g_nClientHeight / 2));
}

void PutPixel(const IntPoint& InPt)
{
	PutPixel(InPt.X, InPt.Y);
}

void PutPixel(int x, int y)
{
	if (!IsInRange(x, y)) return;

	ULONG* dest = (ULONG*)g_pBits;
	DWORD offset = g_nClientWidth * g_nClientHeight / 2 + g_nClientWidth / 2 + x + g_nClientWidth * -y;
	*(dest + offset) = g_CurrentColor;
}


void UpdateFrame(void)
{
	// Buffer Clear
	SetColor(32, 128, 255);
	Clear();

	// Draw
	SetColor(255, 0, 0);

	// Draw a filled circle with radius 100
	Vector3 center(0.0f, 0.0f);
	float radius = 100.0f;
	int nradius = (int)radius;

	static float degree = 0;
	degree += 0.1f;
	degree = fmodf(degree, 360.0f);

	Matrix3 rotMat;
	rotMat.SetRotation(degree);
	rotMat.Transpose();

	static float maxscale = 3;
	static float scale = ((sinf(Deg2Rad(degree * 2)) + 1) *0.5) * maxscale;
	if (scale < 0.5)
	{
		scale = 0.5f;
	}

	Matrix3 scaleMat;
	scaleMat.SetScale(scale, scale, scale);

	static float move = 1;
	move += 1;
	float theta = fmodf(move, 360);
	float pos = sinf(Deg2Rad(theta))*150;
	//move = fmodf(move, 150);
	Matrix3 translationMat;
	translationMat.SetTranslation(pos, pos);

	Matrix3 SR = scaleMat * rotMat;
	Matrix3 TRS = translationMat * rotMat  *  scaleMat;

	for (int i = -nradius; i <= nradius; i++)
	{
		for (int j = -nradius; j <= nradius; j++)
		{
			PutPixel(Vector3((float)i, (float)j) * TRS);
		}
	}

	// Buffer Swap 
	BufferSwap();
}
