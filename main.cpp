#include <Novice.h>
#include "math.h"
#include <cmath>
#include <imgui.h>
#include <numbers>
#define USE_MATH_DEFINES
#define NOMINMAX
#include <assert.h>
#include <algorithm>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif






const char kWindowTitle[] = "LE2D_19_マスダ_アキヒロ_タイトル";

/*

rotateMatrix
-1.000  0.000  0.000  0.000
 0.000 -1.000  0.000  0.000
 0.000  0.000 -1.000  0.000
 0.000  0.000  0.000  1.000

rotateMatrix1
 -0.342 -0.940  0.000  0.000
 -0.940  0.342  0.000  0.000
  0.000 -0.000 -1.000  0.000
  0.000  0.000  0.000  1.000

rotateMatrix1
  0.528 -0.654  0.542  0.000
  0.841  0.313 -0.442  0.000
  0.120  0.689  0.715  0.000
  0.000  0.000  0.000  1.000

 */
#pragma region ベクトル・行列の定義と基本演算子の実装

struct Vector3 
{
	float x;
	float y;
	float z;
	
	Vector3() : x(0), y(0), z(0) {}
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
	

	// 単項マイナス演算子のオーバーロード
	Vector3 operator-() const
	{
		return { -x, -y, -z };
	}
};


//4×4行列
struct Matrix4x4
{
	float m[4][4];
};

#pragma endregion


#pragma region Vector3の基本演算子の定義


float Dot(const Vector3& a, const Vector3& b) 
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 Cross(const Vector3& a, const Vector3& b)
{
	return
	{
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	};
}

float Length(const Vector3& v)
{
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3 Normalize(const Vector3& v) 
{
	float len = Length(v);
	if (len < 1e-6f) return { 0, 0, 0 }; // ゼロベクトルに対する保険
	return { v.x / len, v.y / len, v.z / len };
}

#pragma endregion






#pragma region Matrix4x4の基本演算子の定義

/**/
//任意軸回転行列を作る関数
Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle)
{
	Matrix4x4 result{};
	// 角度の余弦・正弦を求める
	float c = cosf(angle);
	float s = sinf(angle);
	float oneMinusC = 1.0f - c;

	result.m[0][0] = c + axis.x * axis.x * oneMinusC;
	result.m[0][1] = axis.x * axis.y * oneMinusC + axis.z * s;
	result.m[0][2] = axis.x * axis.z * oneMinusC - axis.y * s;
	result.m[0][3] = 0.0f;

	result.m[1][0] = axis.y * axis.x * oneMinusC - axis.z * s;
	result.m[1][1] = c + axis.y * axis.y * oneMinusC;
	result.m[1][2] = axis.y * axis.z * oneMinusC + axis.x * s;
	result.m[1][3] = 0.0f;

	result.m[2][0] = axis.z * axis.x * oneMinusC + axis.y * s;
	result.m[2][1] = axis.z * axis.y * oneMinusC - axis.x * s;
	result.m[2][2] = c + axis.z * axis.z * oneMinusC;
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}



Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to)
{
	Vector3 u = Normalize(from);
	Vector3 v = Normalize(to);

	Vector3 axis = Cross(u, v);
	float cosTheta = Dot(u, v);
	float sinTheta = Length(axis);

	if (cosTheta < -0.9999f) 
	{
		Vector3 ortho;
		if (fabs(u.x) < fabs(u.y) && fabs(u.x) < fabs(u.z))
			ortho = { 1, 0, 0 };
		else if (fabs(u.y) < fabs(u.z))
			ortho = { 0, 1, 0 };
		else
			ortho = { 0, 0, 1 };
		axis = Normalize(Cross(u, ortho));
		sinTheta =  0.0f;
		cosTheta = -1.0f;
	} else 
	{
		axis = Normalize(axis);
	}

	float x = axis.x, y = axis.y, z = axis.z;
	float c = cosTheta;
	float s = sinTheta;
	float t = 1.0f - c;

	Matrix4x4 result = {};
	result.m[0][0] = t * x * x + c;
	result.m[0][1] = t * x * y - s * z;
	result.m[0][2] = t * x * z + s * y;
	result.m[0][3] = 0;

	result.m[1][0] = t * x * y + s * z;
	result.m[1][1] = t * y * y + c;
	result.m[1][2] = t * y * z - s * x;
	result.m[1][3] = 0;

	result.m[2][0] = t * x * z - s * y;
	result.m[2][1] = t * y * z + s * x;
	result.m[2][2] = t * z * z + c;
	result.m[2][3] = 0;

	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = 0;
	result.m[3][3] = 1;

	return result;
}


#pragma endregion


static const int kRowHeight = 20;
static const int kColumnWidth = 60;

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label)
{
	Novice::ScreenPrintf(x, y - kRowHeight, "%s", label);
	for (int row = 0; row < 4; ++row)
	{
		for (int column = 0; column < 4; ++column) 
		{
			Novice::ScreenPrintf
			(
				x + column * kColumnWidth,
				y + row * kRowHeight,
				"%6.03f",
				matrix.m[row][column]
			);
		}
	}
}

// エントリーポイント
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	const int kWindowWidth = 1280;
	const int kWindowHeight = 720;

	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	char keys[256] = { 0 };
	char preKeys[256] = { 0 };


	
	
	
	
	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0)
	{
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);



		///
		///↓更新処理ここから
		/// 


		Matrix4x4 rotateMatrix0 =
			DirectionToDirection
			(
				Normalize(Vector3{ 1.0f,0.0f,0.0f }),
				Normalize(Vector3{ -1.0f,0.0f,0.0f })
			);


		Vector3 from0 = { 1.0f,0.7f,0.5f };
		Vector3 to0 = -from0;
		Matrix4x4 rotateMatrix1 = DirectionToDirection(from0, to0);


		Vector3 from1 = Normalize(Vector3{ -0.6f,0.9f,0.2f });
		Vector3 to1 = Normalize(Vector3{ 0.4f,0.7f,-0.5f });
		Matrix4x4 rotateMatrix2 = DirectionToDirection(from1, to1);

		








		
		



		///
		/// ↑更新処理ここまで
		///



		/// ↓描画処理ここから
		///
		
		//計算結果を描画
		MatrixScreenPrintf(0, 20, rotateMatrix0, "rotateMatrix0");
		MatrixScreenPrintf(0, kRowHeight * 5 + 20, rotateMatrix1, "rotateMatrix1");
		MatrixScreenPrintf(0, kRowHeight * 10 + 20, rotateMatrix2, "rotateMatrix2");

		///
		/// ↑描画処理ここまで
		///


		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0)
		{
			break;
		}
	}

	Novice::Finalize();
	return 0;
}