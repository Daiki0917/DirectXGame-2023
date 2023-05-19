#pragma once
#include"Vector3.h"
#define USE_MATH_DEFINES
#include<math.h>
#include<cmath>

    /// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4 final {
	float m[4][4];
};

//スケーリング
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

// 回転行列X
Matrix4x4 MakeRotX(float thete);

// 回転行列Y
Matrix4x4 MakeRotY(float thete);

// 回転行列Z
Matrix4x4 MakeRotZ(float thete);

// 平行移動行列
Matrix4x4 MakeTranslate(const Vector3&trans);

//アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);

// 代入演算子オーバーロード
Matrix4x4& operator*=(Matrix4x4& m1, const Matrix4x4& m2);

// 2公演算子オーバーロード
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);

// アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);



