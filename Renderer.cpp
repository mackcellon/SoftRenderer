
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


	//Startpoint
	Vector3 StartPoint(0.0f, 0.0f);

	//Circle Matrixes
	Matrix3 CircleTrs;
	Matrix3 CircleRot;

	CircleTrs.SetTranslation(200.0f, 0.0f);

	static float CircleDgree = 0;
	CircleDgree += 1;
	CircleDgree = fmodf(CircleDgree, 360.0f);
	CircleRot.SetRotation(CircleDgree);

	//CircleColor
	int CircleColor = (int)CircleDgree/40 + 20;
	SetColor(CircleColor * 34, CircleColor * 50, CircleColor * 1);

	// CircleDraw
	Vector3 center(0.0f, 0.0f);
	float radius = 30.0f;
	int nradius = (int)radius;

	for (int i = -nradius; i <= nradius; i++)
	{
		for (int j = -nradius; j <= nradius; j++)
		{
			Vector3 vertex(i, j);
			if (Vector3::Dist(center, vertex) <= radius)
			{
				PutPixel(Vector3(i, j)*CircleTrs*CircleRot);
			}
		}
	}

	//SquareMatix
	Matrix3 SqureRot;
	float Sradius = 50.0f;
	int Snradius = (int)Sradius;
	static float SquareDgr = 0;
	SquareDgr += 0.5;
	SquareDgr = fmodf(SquareDgr, 360.0f);
	SqureRot.SetRotation(SquareDgr);

	float maxScale = 1;
	float scale = ((sinf(Deg2Rad(SquareDgr * 2)) + 1) * 0.5) * maxScale;
	if (scale < 0.5f) scale = 0.5f;

	Matrix3 SquareScl;
	SquareScl.SetScale(scale, scale, scale);

	int SqaureColor = (int) SquareDgr / 10 + 5;
	SetColor(SqaureColor * 40, SqaureColor * 20, SqaureColor * 1);

	//SquareDraw
	for (int i = -Snradius; i <= Snradius; i++)
	{
		for (int j = -Snradius; j <= Snradius; j++)
		{
			PutPixel(Vector3((float)i, (float)j)*SqureRot*SquareScl);
		}
	}
	/*static float degree = 0;
	degree += 1;
	degree = fmodf(degree, 360.0f);

	Matrix3 rotMat;
	rotMat.SetRotation(degree);
	rotMat.Transpose();

	

	float maxPos = 150;
	float pos = sinf(Deg2Rad(degree)) * maxPos;
	Matrix3 translationMat;
	translationMat.SetTranslation(pos, pos);

	Matrix3 SR = scaleMat * rotMat;
	Matrix3 TRS = translationMat * rotMat * scaleMat;
	*/

	// Buffer Swap 
	BufferSwap();
}
