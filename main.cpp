#include <Novice.h>
#include "math.h"
#include <cmath>
#include <imgui.h>
#include <numbers>
#define USE_MATH_DEFINES
#define NOMINMAX
#include <assert.h>
#include <algorithm>


const char kWindowTitle[] = "LE2D_19_マスダ_アキヒロ_タイトル";


struct Vector3
{
	float x;
	float y;
	float z;
};

struct Matrix3x3
{
	float m[3][3];
};

struct Matrix4x4
{
	float m[4][4];
};

struct AxisAngle 
{
	Vector3 axis;
	float angle;
};

#pragma region ベクトルの計算
//加算
Vector3 Add(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;
	result = { v1.x + v2.x,v1.y + v2.y,v1.z + v2.z };
	return result;
};



//減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;
	result = { v1.x - v2.x,v1.y - v2.y,v1.z - v2.z };
	return result;
};



//スカラー倍
Vector3 Multiply(float scalar, const Vector3& v)
{
	Vector3 result;
	result = { v.x * scalar,v.y * scalar,v.z * scalar };
	return result;
};


/*
//内積
float Dot(const Vector3& v1, const Vector3& v2)
{
	float result;
	result = { v1.x * v2.x + v1.y * v2.y + v1.z * v2.z };
	return result;
};
*/



float Dot(const Vector3& a, const Vector3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}




//長さ(ノルム)
float Length(const Vector3& v)
{
	float result;
	result = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	return result;
};


/*
//正規化
Vector3 Nomalize(const Vector3& v)
{
	Vector3 result;
	result =
	{
		v.x / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z),
		v.y / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z),
		v.z / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z)
	};
	return result;
};
*/


Vector3 Normalize(const Vector3& v)
{
	float len = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	return { v.x / len, v.y / len, v.z / len };
}




/*
// クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return result;
}
*/


Vector3 Cross(const Vector3& a, const Vector3& b)
{
	return 
	{
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	};
}







// a からスキュー対称行列を作る
Matrix3x3 SkewSymmetricMatrix(const Vector3& a)
{
	Matrix3x3 mat{};
	mat.m[0][0] = 0.0f;    mat.m[0][1] = -a.z;    mat.m[0][2] = a.y;
	mat.m[1][0] = a.z;     mat.m[1][1] = 0.0f;    mat.m[1][2] = -a.x;
	mat.m[2][0] = -a.y;    mat.m[2][1] = a.x;     mat.m[2][2] = 0.0f;
	return mat;
}

// 行列 × ベクトル（3x3 × 3x1）
Vector3 Multiply(const Matrix3x3& m, const Vector3& v)
{
	return
	{
		m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z,
		m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z,
		m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z
	};
}












#pragma endregion

#pragma region 行列の計算

//m1    m1.m[0][0] a11   m1.m[0][1] a12   m1.m[0][2] a13   m1.m[0][3] a14
// 
//      m1.m[1][0] a21   m1.m[1][1] a22   m1.m[1][2] a23   m1.m[1][3] a24 
// 
//      m1.m[2][0] a31   m1.m[2][1] a32   m1.m[2][2] a33   m1.m[2][3] a34 
// 
//      m1.m[3][0] a41   m1.m[3][1] a42   m1.m[3][2] a43   m1.m[3][3] a44 



//m2    m2.m[0][0] b11   m2.m[0][1] b12   m2.m[0][2] b13   m2.m[0][3] b14 
// 
//      m2.m[1][0] b21   m2.m[1][1] b22   m2.m[1][2] b23   m2.m[1][3] b24 
// 
//      m2.m[2][0] b31   m2.m[2][1] b32   m2.m[2][2] b33   m2.m[2][3] b34 
// 
//      m2.m[3][0] b41   m2.m[3][1] b42   m2.m[3][2] b43   m2.m[3][3] b44 



//行列の加法
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 result;
	result.m[0][0] = m1.m[0][0] + m2.m[0][0];
	result.m[0][1] = m1.m[0][1] + m2.m[0][1];
	result.m[0][2] = m1.m[0][2] + m2.m[0][2];
	result.m[0][3] = m1.m[0][3] + m2.m[0][3];
	result.m[0][4] = m1.m[0][4] + m2.m[0][4];


	result.m[1][0] = m1.m[1][0] + m2.m[1][0];
	result.m[1][1] = m1.m[1][1] + m2.m[1][1];
	result.m[1][2] = m1.m[1][2] + m2.m[1][2];
	result.m[1][3] = m1.m[1][3] + m2.m[1][3];
	result.m[1][4] = m1.m[1][4] + m2.m[1][4];


	result.m[2][0] = m1.m[2][0] + m2.m[2][0];
	result.m[2][1] = m1.m[2][1] + m2.m[2][1];
	result.m[2][2] = m1.m[2][2] + m2.m[2][2];
	result.m[2][3] = m1.m[2][3] + m2.m[2][3];
	result.m[2][4] = m1.m[2][4] + m2.m[2][4];


	result.m[3][0] = m1.m[3][0] + m2.m[3][0];
	result.m[3][1] = m1.m[3][1] + m2.m[3][1];
	result.m[3][2] = m1.m[3][2] + m2.m[3][2];
	result.m[3][3] = m1.m[3][3] + m2.m[3][3];
	result.m[3][4] = m1.m[3][4] + m2.m[3][4];





	return result;
}

//行列の減法
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 result;
	result.m[0][0] = m1.m[0][0] - m2.m[0][0];
	result.m[0][1] = m1.m[0][1] - m2.m[0][1];
	result.m[0][2] = m1.m[0][2] - m2.m[0][2];
	result.m[0][3] = m1.m[0][3] - m2.m[0][3];
	result.m[0][4] = m1.m[0][4] - m2.m[0][4];


	result.m[1][0] = m1.m[1][0] - m2.m[1][0];
	result.m[1][1] = m1.m[1][1] - m2.m[1][1];
	result.m[1][2] = m1.m[1][2] - m2.m[1][2];
	result.m[1][3] = m1.m[1][3] - m2.m[1][3];
	result.m[1][4] = m1.m[1][4] - m2.m[1][4];


	result.m[2][0] = m1.m[2][0] - m2.m[2][0];
	result.m[2][1] = m1.m[2][1] - m2.m[2][1];
	result.m[2][2] = m1.m[2][2] - m2.m[2][2];
	result.m[2][3] = m1.m[2][3] - m2.m[2][3];
	result.m[2][4] = m1.m[2][4] - m2.m[2][4];


	result.m[3][0] = m1.m[3][0] - m2.m[3][0];
	result.m[3][1] = m1.m[3][1] - m2.m[3][1];
	result.m[3][2] = m1.m[3][2] - m2.m[3][2];
	result.m[3][3] = m1.m[3][3] - m2.m[3][3];
	result.m[3][4] = m1.m[3][4] - m2.m[3][4];





	return result;
}

//行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 result;

	result.m[0][0] = { (m1.m[0][0] * m2.m[0][0]) + (m1.m[0][1] * m2.m[1][0]) + (m1.m[0][2] * m2.m[2][0]) + (m1.m[0][3] * m2.m[3][0]) };
	result.m[0][1] = { (m1.m[0][0] * m2.m[0][1]) + (m1.m[0][1] * m2.m[1][1]) + (m1.m[0][2] * m2.m[2][1]) + (m1.m[0][3] * m2.m[3][1]) };
	result.m[0][2] = { (m1.m[0][0] * m2.m[0][2]) + (m1.m[0][1] * m2.m[1][2]) + (m1.m[0][2] * m2.m[2][2]) + (m1.m[0][3] * m2.m[3][2]) };
	result.m[0][3] = { (m1.m[0][0] * m2.m[0][3]) + (m1.m[0][1] * m2.m[1][3]) + (m1.m[0][2] * m2.m[2][3]) + (m1.m[0][3] * m2.m[3][3]) };



	result.m[1][0] = { (m1.m[1][0] * m2.m[0][0]) + (m1.m[1][1] * m2.m[1][0]) + (m1.m[1][2] * m2.m[2][0]) + (m1.m[1][3] * m2.m[3][0]) };
	result.m[1][1] = { (m1.m[1][0] * m2.m[0][1]) + (m1.m[1][1] * m2.m[1][1]) + (m1.m[1][2] * m2.m[2][1]) + (m1.m[1][3] * m2.m[3][1]) };
	result.m[1][2] = { (m1.m[1][0] * m2.m[0][2]) + (m1.m[1][1] * m2.m[1][2]) + (m1.m[1][2] * m2.m[2][2]) + (m1.m[1][3] * m2.m[3][2]) };
	result.m[1][3] = { (m1.m[1][0] * m2.m[0][3]) + (m1.m[1][1] * m2.m[1][3]) + (m1.m[1][2] * m2.m[2][3]) + (m1.m[1][3] * m2.m[3][3]) };



	result.m[2][0] = { (m1.m[2][0] * m2.m[0][0]) + (m1.m[2][1] * m2.m[1][0]) + (m1.m[2][2] * m2.m[2][0]) + (m1.m[2][3] * m2.m[3][0]) };
	result.m[2][1] = { (m1.m[2][0] * m2.m[0][1]) + (m1.m[2][1] * m2.m[1][1]) + (m1.m[2][2] * m2.m[2][1]) + (m1.m[2][3] * m2.m[3][1]) };
	result.m[2][2] = { (m1.m[2][0] * m2.m[0][2]) + (m1.m[2][1] * m2.m[1][2]) + (m1.m[2][2] * m2.m[2][2]) + (m1.m[2][3] * m2.m[3][2]) };
	result.m[2][3] = { (m1.m[2][0] * m2.m[0][3]) + (m1.m[2][1] * m2.m[1][3]) + (m1.m[2][2] * m2.m[2][3]) + (m1.m[2][3] * m2.m[3][3]) };



	result.m[3][0] = { (m1.m[3][0] * m2.m[0][0]) * (m1.m[3][1] * m2.m[1][0]) + (m1.m[3][2] * m2.m[2][0]) + (m1.m[3][3] * m2.m[3][0]) };
	result.m[3][1] = { (m1.m[3][0] * m2.m[0][1]) + (m1.m[3][1] * m2.m[1][1]) + (m1.m[3][2] * m2.m[2][1]) + (m1.m[3][3] * m2.m[3][1]) };
	result.m[3][2] = { (m1.m[3][0] * m2.m[0][2]) + (m1.m[3][1] * m2.m[1][2]) + (m1.m[3][2] * m2.m[2][2]) + (m1.m[3][3] * m2.m[3][2]) };
	result.m[3][3] = { (m1.m[3][0] * m2.m[0][3]) + (m1.m[3][1] * m2.m[1][3]) + (m1.m[3][2] * m2.m[2][3]) + (m1.m[3][3] * m2.m[3][3]) };

	return result;
}

//逆行列
Matrix4x4 Inverse(const Matrix4x4& m)
{

	Matrix4x4 result{};
	float determinant =
		(
			(m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3]) +
			(m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1]) +
			(m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2]) -
			(m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1]) -
			(m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3]) -
			(m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2]) -
			(m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3]) -
			(m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1]) -
			(m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2]) +
			(m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1]) +
			(m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3]) +
			(m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2]) +
			(m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3]) +
			(m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1]) +
			(m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2]) -
			(m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1]) -
			(m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3]) -
			(m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2]) -
			(m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0]) -
			(m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0]) -
			(m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0]) +
			(m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0]) +
			(m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0]) +
			(m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0])

			);

	result =
	{
		(
			(m.m[1][1] * m.m[2][2] * m.m[3][3]) + (m.m[1][2] * m.m[2][3] * m.m[3][1]) + (m.m[1][3] * m.m[2][1] * m.m[3][2])
			- (m.m[1][3] * m.m[2][2] * m.m[3][1]) - (m.m[1][2] * m.m[2][1] * m.m[3][3]) - (m.m[1][1] * m.m[2][3] * m.m[3][2])
		) / determinant,

		(
			-(m.m[0][1] * m.m[2][2] * m.m[3][3]) - (m.m[0][2] * m.m[2][3] * m.m[3][1]) - (m.m[0][3] * m.m[2][1] * m.m[3][2])
			+ (m.m[0][3] * m.m[2][2] * m.m[3][1]) + (m.m[0][2] * m.m[2][1] * m.m[3][3]) + (m.m[0][1] * m.m[2][3] * m.m[3][2])
		) / determinant,

		(
			(m.m[0][1] * m.m[1][2] * m.m[3][3]) + (m.m[0][2] * m.m[1][3] * m.m[3][1]) + (m.m[0][3] * m.m[1][1] * m.m[3][2])
			- (m.m[0][3] * m.m[1][2] * m.m[3][1]) - (m.m[0][2] * m.m[1][1] * m.m[3][3]) - (m.m[0][1] * m.m[1][3] * m.m[3][2])
		) / determinant,

		(
			-(m.m[0][1] * m.m[1][2] * m.m[2][3]) - (m.m[0][2] * m.m[1][3] * m.m[2][1]) - (m.m[0][3] * m.m[1][1] * m.m[2][2])
			+ (m.m[0][3] * m.m[1][2] * m.m[2][1]) + (m.m[0][2] * m.m[1][1] * m.m[2][3]) + (m.m[0][1] * m.m[1][3] * m.m[2][2])
		) / determinant,

			//======================================================================================================================================

			(
				-(m.m[1][0] * m.m[2][2] * m.m[3][3]) - (m.m[1][2] * m.m[2][3] * m.m[3][0]) - (m.m[1][3] * m.m[2][0] * m.m[3][2])
				+ (m.m[1][3] * m.m[2][2] * m.m[3][0]) + (m.m[1][2] * m.m[2][0] * m.m[3][3]) + (m.m[1][0] * m.m[2][3] * m.m[3][2])
			) / determinant,

			(
				(m.m[0][0] * m.m[2][2] * m.m[3][3]) + (m.m[0][2] * m.m[2][3] * m.m[3][0]) + (m.m[0][3] * m.m[2][0] * m.m[3][2])
				- (m.m[0][3] * m.m[2][2] * m.m[3][0]) - (m.m[0][2] * m.m[2][0] * m.m[3][3]) - (m.m[0][0] * m.m[2][3] * m.m[3][2])
			) / determinant,

			(
				-(m.m[0][0] * m.m[1][2] * m.m[3][3]) - (m.m[0][2] * m.m[1][3] * m.m[3][0]) - (m.m[0][3] * m.m[1][0] * m.m[3][2])
				+ (m.m[0][3] * m.m[1][2] * m.m[3][0]) + (m.m[0][2] * m.m[1][0] * m.m[3][3]) + (m.m[0][0] * m.m[1][3] * m.m[3][2])
			) / determinant,

			(
				(m.m[0][0] * m.m[1][2] * m.m[2][3]) + (m.m[0][2] * m.m[1][3] * m.m[2][0]) + (m.m[0][3] * m.m[1][0] * m.m[2][2])
				- (m.m[0][3] * m.m[1][2] * m.m[2][0]) - (m.m[0][2] * m.m[1][0] * m.m[2][3]) - (m.m[0][0] * m.m[1][3] * m.m[2][2])
			) / determinant,

			//======================================================================================================================================


			(
				(m.m[1][0] * m.m[2][1] * m.m[3][3]) + (m.m[1][1] * m.m[2][3] * m.m[3][0]) + (m.m[1][3] * m.m[2][0] * m.m[3][1])
				- (m.m[1][3] * m.m[2][1] * m.m[3][0]) - (m.m[1][1] * m.m[2][0] * m.m[3][3]) - (m.m[1][0] * m.m[2][3] * m.m[3][1])
			) / determinant,

			(
				-(m.m[0][0] * m.m[2][1] * m.m[3][3]) - (m.m[0][1] * m.m[2][3] * m.m[3][0]) - (m.m[0][3] * m.m[2][0] * m.m[3][1])
				+ (m.m[0][3] * m.m[2][1] * m.m[3][0]) + (m.m[0][1] * m.m[2][0] * m.m[3][3]) + (m.m[0][0] * m.m[2][3] * m.m[3][1])
			) / determinant,

			(
				(m.m[0][0] * m.m[1][1] * m.m[3][3]) + (m.m[0][1] * m.m[1][3] * m.m[3][0]) + (m.m[0][3] * m.m[1][0] * m.m[3][1])
				- (m.m[0][3] * m.m[1][1] * m.m[3][0]) - (m.m[0][1] * m.m[1][0] * m.m[3][3]) - (m.m[0][0] * m.m[1][3] * m.m[3][1])
			) / determinant,

			(
				-(m.m[0][0] * m.m[1][1] * m.m[2][3]) - (m.m[0][1] * m.m[1][3] * m.m[2][0]) - (m.m[0][3] * m.m[1][0] * m.m[2][1])
				+ (m.m[0][3] * m.m[1][1] * m.m[2][0]) + (m.m[0][1] * m.m[1][0] * m.m[2][3]) + (m.m[0][0] * m.m[1][3] * m.m[2][1])
			) / determinant,

			//======================================================================================================================================


			(
				-(m.m[1][0] * m.m[2][1] * m.m[3][2]) - (m.m[1][1] * m.m[2][2] * m.m[3][0]) - (m.m[1][2] * m.m[2][0] * m.m[3][1])
				+ (m.m[1][2] * m.m[2][1] * m.m[3][0]) + (m.m[1][1] * m.m[2][0] * m.m[3][2]) + (m.m[1][0] * m.m[2][2] * m.m[3][1])
			) / determinant,

			(
				(m.m[0][0] * m.m[2][1] * m.m[3][2]) + (m.m[0][1] * m.m[2][2] * m.m[3][0]) + (m.m[0][2] * m.m[2][0] * m.m[3][1])
				- (m.m[0][2] * m.m[2][1] * m.m[3][0]) - (m.m[0][1] * m.m[2][0] * m.m[3][2]) - (m.m[0][0] * m.m[2][2] * m.m[3][1])
			) / determinant,

			(
				-(m.m[0][0] * m.m[1][1] * m.m[3][2]) - (m.m[0][1] * m.m[1][2] * m.m[3][0]) - (m.m[0][2] * m.m[1][0] * m.m[3][1])
				+ (m.m[0][2] * m.m[1][1] * m.m[3][0]) + (m.m[0][1] * m.m[1][0] * m.m[3][2]) + (m.m[0][0] * m.m[1][2] * m.m[3][1])
			) / determinant,

			(
				(m.m[0][0] * m.m[1][1] * m.m[2][2]) + (m.m[0][1] * m.m[1][2] * m.m[2][0]) + (m.m[0][2] * m.m[1][0] * m.m[2][1])
				- (m.m[0][2] * m.m[1][1] * m.m[2][0]) - (m.m[0][1] * m.m[1][0] * m.m[2][2]) - (m.m[0][0] * m.m[1][2] * m.m[2][1])
			) / determinant,



	};




	return result;
}

//転置行列
Matrix4x4 Transpose(const Matrix4x4& m)
{
	Matrix4x4 result;

	result =
	{
		 m.m[0][0],m.m[1][0],m.m[2][0],m.m[3][0],
		 m.m[0][1],m.m[1][1],m.m[2][1],m.m[3][1],
		 m.m[0][2],m.m[1][2],m.m[2][2],m.m[3][2],
		 m.m[0][3],m.m[1][3],m.m[2][3],m.m[3][3]
	};
	return result;
}

//単位行列の作成
Matrix4x4 MakeIdentity4x4(const Matrix4x4)
{
	Matrix4x4 result;
	result =
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	return result;
}


//rcosθ+(1-cosθ)projnr+(nxr)sinθ
/*
// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale)
{
	Matrix4x4 result{};
	result.m[0][0] = scale.x;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = scale.y;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = scale.z;
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

// X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian)
{
	Matrix4x4 result{};

	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = std::cos(radian);
	result.m[1][2] = std::sin(radian);
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = -(std::sin(radian));
	result.m[2][2] = std::cos(radian);
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

// Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian)
{
	Matrix4x4 result{};

	result.m[0][0] = std::cos(radian);
	result.m[0][1] = 0.0f;
	result.m[0][2] = -(std::sin(radian));
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = std::sin(radian);
	result.m[2][1] = 0.0f;
	result.m[2][2] = std::cos(radian);
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

// Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian)
{
	Matrix4x4 result{};

	result.m[0][0] = std::cos(radian);
	result.m[0][1] = std::sin(radian);
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = -(std::sin(radian));
	result.m[1][1] = std::cos(radian);
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}
*/


// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(float theta)
{
	Matrix4x4 result{};
	result.m[0][0] = std::cos(theta);
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = std::cos(theta);
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = std::cos(theta);
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

Matrix4x4 MakeRotateMatrix(float theta, const Vector3& axisRaw)
{
	Matrix4x4 result{};

	// 軸ベクトルを正規化
	float len = std::sqrt(axisRaw.x * axisRaw.x + axisRaw.y * axisRaw.y + axisRaw.z * axisRaw.z);
	float x = axisRaw.x / len;
	float y = axisRaw.y / len;
	float z = axisRaw.z / len;

	// 三角関数
	float c = std::cos(theta);      // cosθ
	float s = std::sin(theta);      // sinθ
	float t = 1.0f - c;             // (1 - cosθ)

	// ロドリゲスの回転公式
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



// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate)
{
	Matrix4x4 result{};
	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	result.m[2][3] = 0.0f;

	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	result.m[3][3] = 1.0f;

	return result;
}
#pragma endregion




Matrix4x4 MakeRotateAxisAngle(const Vector3& axisRaw, float angle)
{
	Vector3 axis = Normalize(axisRaw);
	float x = axis.x, y = axis.y, z = axis.z;
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








AxisAngle DecomposeRotation(const Matrix4x4& R) 
{
	AxisAngle result;

	float trace = R.m[0][0] + R.m[1][1] + R.m[2][2];
	float cosTheta = (trace - 1.0f) * 0.5f;
	cosTheta = std::clamp(cosTheta, -1.0f, 1.0f); // 安定性

	result.angle = std::acos(cosTheta);
	float sinTheta = std::sin(result.angle);

	if (std::abs(sinTheta) < 1e-6f)
	{
		// 角度が 0 または 180 の場合、軸は不定 → デフォルト返す
		result.axis = { 1.0f, 0.0f, 0.0f };
	} else 
	{
		result.axis = 
		{
			(R.m[2][1] - R.m[1][2]) / (2.0f * sinTheta),
			(R.m[0][2] - R.m[2][0]) / (2.0f * sinTheta),
			(R.m[1][0] - R.m[0][1]) / (2.0f * sinTheta)
		};
	}

	return result;
}

static const int kRowHeight = 20;
static const int kColumnWidth = 60;
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
				"%6.02f", matrix.m[row][column]
			);
		}
	}
}



// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) 
{

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};


	Matrix4x4 m1 =
	{
		3.2f,0.7f,9.6f,4.4f,
		5.5f,1.3f,7.8f,2.1f,
		6.9f,8.0f,2.6f,1.0f,
		0.5f,7.2f,5.1f,3.3f,
	};

	Matrix4x4 m2 =
	{
		4.1f,6.5f,3.3f,2.2f,
		8.8f,0.6f,9.9f,7.7f,
		1.1f,5.5f,6.6f,0.0f,
		3.3f,9.9f,8.8f,2.2f,
	};



	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0)
	{
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///




		Matrix4x4 resultAdd = Add(m1, m2);
		Matrix4x4 resultMultiply = Multiply(m1, m2);
		Matrix4x4 resultSubtract = Subtract(m1, m2);
		Matrix4x4 inverseM1 = Inverse(m1);
		Matrix4x4 inverseM2 = Inverse(m2);
		Matrix4x4 transposeM1 = Transpose(m1);
		Matrix4x4 transposeM2 = Transpose(m2);
		Matrix4x4 identity = MakeIdentity4x4(m1);




		Vector3 axis = Normalize({ 1.0f,1.0f,1.0f });
		float angle = 0.44f;
		Matrix4x4 rotateMatrix = MakeRotateAxisAngle(axis, angle);
		MatrixScreenPrintf(0, 0, rotateMatrix, "rotateMatrix");
		Matrix4x4 R = MakeRotateAxisAngle({ -0.578f, -0.578f, -0.578f }, 0.429f);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

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

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
