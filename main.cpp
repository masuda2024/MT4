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

#pragma region ベクトル・行列の定義と基本演算子の実装
struct Vector3 
{
	float x;
	float y;
	float z;
	// 加算
	Vector3 operator+(const Vector3& other) const 
	{
		return { x + other.x, y + other.y, z + other.z };
	}

	// 減算
	Vector3 operator-(const Vector3& other) const 
	{
		return { x - other.x, y - other.y, z - other.z };
	}

	// スカラー倍
	Vector3 operator*(float scalar) const 
	{
		return { x * scalar, y * scalar, z * scalar };
	}
	// 単項マイナス演算子のオーバーロード
	Vector3 operator-() const
	{
		return { -x, -y, -z };
	}
};

//正規化
Vector3 Normalize(const Vector3& v)
{
	float len = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	if (len == 0.0f)
		return { 0, 0, 0 }; // ゼロベクトル対応（必要に応じて assert）
	return { v.x / len, v.y / len, v.z / len };
}

//4×4行列
struct Matrix4x4
{
	float m[4][4];
	
	// 単位行列を返す
	static Matrix4x4 Identity()
	{
		Matrix4x4 result{};
		for (int i = 0; i < 4; ++i)
			result.m[i][i] = 1.0f;
		return result;
	}

	// 任意軸回転行列を返す
	static Matrix4x4 RotationAxis(const Vector3& axis, float angle)
	{
		Vector3 a = Normalize(axis);
		float x = a.x, y = a.y, z = a.z;
		float c = std::cos(angle);
		float s = std::sin(angle);
		float t = 1.0f - c;

		Matrix4x4 result{};

		result.m[0][0] = t * x * x + c;
		result.m[0][1] = t * x * y - s * z;
		result.m[0][2] = t * x * z + s * y;
		result.m[0][3] = 0.0f;

		result.m[1][0] = t * x * y + s * z;
		result.m[1][1] = t * y * y + c;
		result.m[1][2] = t * y * z - s * x;
		result.m[1][3] = 0.0f;

		result.m[2][0] = t * x * z - s * y;
		result.m[2][1] = t * y * z + s * x;
		result.m[2][2] = t * z * z + c;
		result.m[2][3] = 0.0f;

		result.m[3][0] = 0.0f;
		result.m[3][1] = 0.0f;
		result.m[3][2] = 0.0f;
		result.m[3][3] = 1.0f;

		return result;
	}
};

#pragma endregion

#pragma region Vector3の基本演算子の定義


// ドット積
float Dot(const Vector3& a, const Vector3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

// クロス積
Vector3 Cross(const Vector3& a, const Vector3& b) 
{
	return 
	{
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	};
}

Vector3 Perpendicular(const Vector3& v) 
{
	if (std::abs(v.x) < std::abs(v.y) && std::abs(v.x) < std::abs(v.z))
		return Normalize(Cross(v, Vector3(1, 0, 0)));
	else if (std::abs(v.y) < std::abs(v.z))
		return Normalize(Cross(v, Vector3(0, 1, 0)));
	else
		return Normalize(Cross(v, Vector3(0, 0, 1)));
}

#pragma endregion


#pragma region Matrix4x4の基本演算子の定義



Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to)
{
	Vector3 f = Normalize(from);
	Vector3 t = Normalize(to);
	float dot = Dot(f, t);

	// 同じ方向
	if (dot > 0.9999f) 
	{
		return Matrix4x4::Identity(); // 単位行列
	}

	// 反対方向
	if (dot < -0.9999f)
	{
		// from と直交するベクトルを軸に回転（180度回転）
		Vector3 axis = Normalize(Perpendicular(f)); // 直交ベクトルを生成する関数が必要
		return Matrix4x4::RotationAxis(axis, (float)M_PI); // 180度回転
	}

	Vector3 axis = Normalize(Cross(f, t));
	float angle = std::acos(dot); // 角度（ラジアン）

	return Matrix4x4::RotationAxis(axis, angle);
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


		Vector3 from0 = { 1.0f,0.7f,0.5f };
		Vector3 to0 = -from0;
		Vector3 from1 = Normalize(Vector3{ -0.6f,0.9f,0.2f });
		Vector3 to1 = Normalize(Vector3{ 0.4f,0.7f,-0.5f });
		Matrix4x4 rotateMatrix0 =
			DirectionToDirection
			(
				Normalize(Vector3{ 1.0f,0.0f,0.0f }),
				Normalize(Vector3{ -1.0f,0.0f,0.0f })
			);
		Matrix4x4 rotateMatrix1 = DirectionToDirection(from0, to0);
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