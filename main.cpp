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

Vector3 Transform(const Vector3& v, const Matrix4x4& m)
{
	Vector3 result;

	result.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0];
	result.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1];
	result.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2];

	return result;
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


//任意軸回転を表すQuaternionの生成
Quaternion MakeRotateAxisAngleQuaternion(const Vector3 axis, float angle)
{
	// 軸を正規化
	float len = std::sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
	if (len == 0.0f) {
		// 軸がゼロベクトルの場合、単位クォータニオンを返す
		return { 1.0f, 0.0f, 0.0f, 0.0f };
	}
	float nx = axis.x / len;
	float ny = axis.y / len;
	float nz = axis.z / len;

	// 半角を使う
	float halfAngle = angle * 0.5f;
	float s = std::sin(halfAngle);
	float c = std::cos(halfAngle);

	Quaternion q;
	q.w = c;
	q.x = nx * s;
	q.y = ny * s;
	q.z = nz * s;

	return q;
}

//ベクトルをQuaternionで回転させた結果のベクトルを求める
Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion)
{
	// q = quaternion
	// v = (0, vector)
	float qx = quaternion.x;
	float qy = quaternion.y;
	float qz = quaternion.z;
	float qw = quaternion.w;

	// t = 2 * cross(q.xyz, v)
	Vector3 t;
	t.x = 2.0f * (qy * vector.z - qz * vector.y);
	t.y = 2.0f * (qz * vector.x - qx * vector.z);
	t.z = 2.0f * (qx * vector.y - qy * vector.x);

	// v' = v + qw * t + cross(q.xyz, t)
	Vector3 result;
	result.x = vector.x + qw * t.x + (qy * t.z - qz * t.y);
	result.y = vector.y + qw * t.y + (qz * t.x - qx * t.z);
	result.z = vector.z + qw * t.z + (qx * t.y - qy * t.x);

	return result;
}


//Quaternionから回転行列を求める
Matrix4x4 MakeRotateMatrix(const Quaternion& q)
{
	Matrix4x4 mat{};

	float xx = q.x * q.x;
	float yy = q.y * q.y;
	float zz = q.z * q.z;
	float xy = q.x * q.y;
	float xz = q.x * q.z;
	float yz = q.y * q.z;
	float wx = q.w * q.x;
	float wy = q.w * q.y;
	float wz = q.w * q.z;

	mat.m[0][0] = 1.0f - 2.0f * (yy + zz);
	mat.m[0][1] = 2.0f * (xy + wz);//////////
	mat.m[0][2] = 2.0f * (xz - wy);/////////
	mat.m[0][3] = 0.0f;

	mat.m[1][0] = 2.0f * (xy - wz);
	mat.m[1][1] = 1.0f - 2.0f * (xx + zz);
	mat.m[1][2] = 2.0f * (yz + wx);//////////
	mat.m[1][3] = 0.0f;

	mat.m[2][0] = 2.0f * (xz + wy);//////////
	mat.m[2][1] = 2.0f * (yz - wx);////////
	mat.m[2][2] = 1.0f - 2.0f * (xx + yy);
	mat.m[2][3] = 0.0f;

	// 同次座標変換用（位置や拡縮が無いので単位行列）
	mat.m[3][0] = 0.0f;
	mat.m[3][1] = 0.0f;
	mat.m[3][2] = 0.0f;
	mat.m[3][3] = 1.0f;

	return mat;
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



void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label)
{
	Novice::ScreenPrintf(x, y, "%s", label);


	for (int row = 0; row < 4; ++row)
	{
		for (int column = 0; column < 4; ++column)
		{
			Novice::ScreenPrintf
			(
				x + column * kColumnWidth,
				y + (row + 1) * kRowHeight,
				"%6.03f", matrix.m[row][column]
			);
		}
	}
}

void VectorScreenPrintf(int x, int y, const Vector3& Vector, const char* label)
{
	Novice::ScreenPrintf(x, y, "%.02f", Vector.x);

	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", Vector.y);

	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", Vector.z);

	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
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

		

		Quaternion rotation = MakeRotateAxisAngleQuaternion
		(
			Normalize(Vector3{ 1.0f,0.4f,-0.2f }), 0.45f
		);
		Vector3 pointY = { 2.1f,-0.9f,1.3f };
		Matrix4x4 rotateMatrix = MakeRotateMatrix(rotation);
		Vector3 rotateByQuaternion = RotateVector(pointY, rotation);
		Vector3 rotateByMatrix = Transform(pointY, rotateMatrix);



		///
		/// ↑更新処理ここまで
		///



		/// ↓描画処理ここから
		///
		
		
		QuaternionScreenPrintf(0, kRowHeight * 0, rotation, "                               :  rotation");
		MatrixScreenPrintf(0, kRowHeight * 1, rotateMatrix, "rotateMatrix");
		VectorScreenPrintf(0, kRowHeight * 6, rotateByQuaternion, "           :  rotateByQuaterion");
		VectorScreenPrintf(0, kRowHeight * 7, rotateByMatrix, "           :  rotateByMatrix");






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