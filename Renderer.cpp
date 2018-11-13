
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

void DrawLine(const Vector3 &start, const Vector3 &end)
{
	//L = aP + bQ, a+b = 1
	
	float length = Vector3::Dist(end, start);
	float inc = 1.0f / length;
	int nlength = (int)(length + 1);

	for (int i = 0; i <= nlength; i++)
	{
		float t = inc * i;
		if (t >= length) t = 1.0f;
		Vector3 pt = start * (1.0f - t) + end * t;
		PutPixel(IntPoint(pt));
	}
}


void DrawTriangle(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3)
{
	Vector2 minPos = Vector2(INFINITY, INFINITY);
	IntPoint maxPos = Vector2(-INFINITY, -INFINITY);

	if (p1.X < minPos.X) minPos.X = p1.X;
	if (p1.Y < minPos.Y) minPos.Y = p1.Y;
	if (p1.X > maxPos.X) maxPos.X = p1.X;
	if (p1.Y > maxPos.Y) maxPos.Y = p1.Y;

	if (p2.X < minPos.X) minPos.X = p2.X;
	if (p2.Y < minPos.Y) minPos.Y = p2.Y;
	if (p2.X > maxPos.X) maxPos.X = p2.X;
	if (p2.Y > maxPos.Y) maxPos.Y = p2.Y;

	if (p3.X < minPos.X) minPos.X = p3.X;
	if (p3.Y < minPos.Y) minPos.Y = p3.Y;
	if (p3.X > maxPos.X) maxPos.X = p3.X;
	if (p3.Y > maxPos.Y) maxPos.Y = p3.Y;
	
	


	Vector3 u = p2 - p1;
	Vector3 v = p3 - p1;

	float dotUU = Vector3::Dot(u, u);
	float dotUV = Vector3::Dot(u, v);
	float dotVV = Vector3::Dot(v, v);

	float invDenom = 1.0f / (dotUU * dotVV - dotUV * dotUV);

	IntPoint minPt(minPos);
	IntPoint maxPt(maxPos);

	for (int x = minPos.X; x < maxPos.X; x++)
	{
		for (int y = minPos.Y; y < maxPos.Y; y++)
		{
			IntPoint pt(x, y);
			Vector3 w = pt.ToVector3() - p1;
			float dotUW = Vector3::Dot(u, w);
			float dotVW = Vector3::Dot(v, w);

			float s = (dotVV * dotUW - dotUV * dotVW) * invDenom;
			float t= (dotUU * dotVW - dotUV * dotUW) * invDenom;
			if (s >= 0 && t >= 0 && ((s + t) <= 1))
			{
				PutPixel(pt);
			}

		}
	}

}


void UpdateFrame(void)
{
	// Buffer Clear
	SetColor(32, 128, 255);
	Clear();

	// 이동
	static float xPos = 0;
	static float yPos = 0;
	//스케일
	static float scale = 1;
	if (scale < 1)
	{
		scale = 1;
	}

	//회전
	static float degree = 0;;

	// input
	if (GetAsyncKeyState(VK_LEFT)) degree -= 80;
	if (GetAsyncKeyState(VK_RIGHT)) degree += 80;
	if (GetAsyncKeyState(VK_UP)) yPos += 1;
	if (GetAsyncKeyState(VK_DOWN)) yPos -= 1;
	if (GetAsyncKeyState(VK_PRIOR)) scale += 0.1;
	if (GetAsyncKeyState(VK_NEXT)) scale -= 0.1;

	//Define Matrix
	Matrix3 tMat;
	tMat.SetTranslation(xPos, yPos);

	Matrix3 sMat;
	sMat.SetScale(scale, scale, 1);

	Matrix3 rMat;
	rMat.SetRotation(degree);

	Matrix3 trsMat;
	trsMat = tMat*rMat*sMat;

	Vector3 start = Vector3::Make2DPoint(-40, 10) * trsMat;
	Vector3 end = Vector3::Make2DPoint(40, 10) * trsMat;



	// Draw
	SetColor(255, 0, 0);

	//DrawLine(start, end);

	Vector3 p1 = Vector3::Make2DPoint(-80, -80) * trsMat;
	Vector3 p2 = Vector3::Make2DPoint(-80, 80) * trsMat;
	Vector3 p3 = Vector3::Make2DPoint(80,  -80) * trsMat;
	Vector3 p4 = Vector3::Make2DPoint(80, 80) * trsMat;


	DrawTriangle(p1, p2, p3);
	DrawTriangle(p2, p3, p4);

	//Define vector3 space
	/*float radius = 100.0f;
	int nradius = (int)radius;

	for (int i = -nradius; i <= nradius; i++)
	{
		for (int j = -nradius; j <= nradius; j++)
		{
			PutPixel(Vector3((float)i, (float)j)*trsMat);
		}
	}
*/
	// Buffer Swap 
	BufferSwap();
}

//백업
// Draw a filled circle with radius 100
//Vector3 center(0.0f, 0.0f);
//float radius = 100.0f;
//int nradius = (int)radius;
//
//static float degree = 0;
//degree += 0.1f;
//degree = fmodf(degree, 360.0f);
//
//Matrix3 rotMat;
//rotMat.SetRotation(degree);
//rotMat.Transpose();
//
//static float maxscale = 3;
//static float scale = ((sinf(Deg2Rad(degree * 2)) + 1) *0.5) * maxscale;
//if (scale < 0.5)
//{
//	scale = 0.5f;
//}
//
//Matrix3 scaleMat;
//scaleMat.SetScale(scale, scale, scale);
//
//static float move = 1;
//move += 1;
//float theta = fmodf(move, 360);
//float pos = sinf(Deg2Rad(theta)) * 150;
////move = fmodf(move, 150);
//Matrix3 translationMat;
//translationMat.SetTranslation(pos, pos);
//
//Matrix3 SR = scaleMat * rotMat;
//Matrix3 TRS = translationMat * rotMat  *  scaleMat;
//
//for (int i = -nradius; i <= nradius; i++)
//{
//	for (int j = -nradius; j <= nradius; j++)
//	{
//		PutPixel(Vector3((float)i, (float)j) * TRS);
//	}
//}