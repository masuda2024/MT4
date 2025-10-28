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


struct Quaternion
{
	float x;
	float y;
	float z;
	float w;
};


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



//Quaternionの積
Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs)
{
	Quaternion result;
	result.x = lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y;
	result.y = lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x;
	result.z = lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w;
	result.w = lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z;
	return result;
}
//単位Quaternionを返す
Quaternion IdentityQuaternion()
{
	Quaternion result;
	result.x = 0.0f;
	result.y = 0.0f;
	result.z = 0.0f;
	result.w = 1.0f;
	return result;
}

//共役Quaternionを返す
Quaternion Conjugate(const Quaternion& q)
{
	Quaternion result;
	result.x = -q.x;
	result.y = -q.y;
	result.z = -q.z;
	result.w = q.w;
	return result;
}

//Quaternionのnormを返す
float Norm(const Quaternion& q)
{
	return sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
}

//正規化したQuaterionを返す
Quaternion Normalize(const Quaternion& q)
{
	float norm = Norm(q);
	if (norm < 1e-6f) return IdentityQuaternion(); // ゼロ除算防止
	Quaternion result;
	result.x = q.x / norm;
	result.y = q.y / norm;
	result.z = q.z / norm;
	result.w = q.w / norm;
	return result;
}

//逆Quaternionを返す
Quaternion Inverse(const Quaternion& q)
{
	Quaternion conjugate = Conjugate(q);
	float normSq = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
	if (normSq < 1e-6f) return IdentityQuaternion(); // ゼロ除算防止
	Quaternion result;
	result.x = conjugate.x / normSq;
	result.y = conjugate.y / normSq;
	result.z = conjugate.z / normSq;
	result.w = conjugate.w / normSq;
	return result;
}

#pragma endregion


static const int kRowHeight = 20;
static const int kColumnWidth = 60;

// クォータニオンを画面に出力する関数
void QuaternionScreenPrintf(int x, int y, const Quaternion& q, const char* label)
{
	// ラベル表示
	Novice::ScreenPrintf(x, y, "%s", label);

	// 各成分を横並びで表示（x, y, z, w）
	Novice::ScreenPrintf(x + 0 * kColumnWidth, y, "%6.02f", q.x);
	Novice::ScreenPrintf(x + 1 * kColumnWidth, y, "%6.02f", q.y);
	Novice::ScreenPrintf(x + 2 * kColumnWidth, y, "%6.02f", q.z);
	Novice::ScreenPrintf(x + 3 * kColumnWidth, y, "%6.02f", q.w);
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


		Quaternion q1 = { 2.0f, 3.0f, 4.0f, 1.0f };
		Quaternion q2 = { 1.0f, 3.0f, 5.0f, 2.0f };
		Quaternion identity = IdentityQuaternion();
		Quaternion conj = Conjugate(q1);
		Quaternion inv = Inverse(q1);
		Quaternion normal = Normalize(q1);
		Quaternion mul1 = Multiply(q1, q2);
		Quaternion mul2 = Multiply(q2, q1);
		float norm = Norm(q1);




		///
		/// ↑更新処理ここまで
		///



		/// ↓描画処理ここから
		///
		
		
		QuaternionScreenPrintf(10, 10, identity, "                               :  Identity");
		QuaternionScreenPrintf(10, 30, conj,     "                               :  Conjugate");
		QuaternionScreenPrintf(10, 50, inv,      "                               :  Inverse");
		QuaternionScreenPrintf(10, 70, normal,   "                               :  Normalize");
		QuaternionScreenPrintf(10, 90, mul1,     "                               :  Multiply(q1, q2)");
		QuaternionScreenPrintf(10, 110, mul2,    "                               :  Multiply(q2, q1)");
		Novice::ScreenPrintf(10, 130,            "%6.02f                         :  Norm", norm);


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