#pragma once
#include"Vector3.h"
#define USE_MATH_DEFINES
#include<math.h>
#include<cmath>
#include<cassert>

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

//逆行列
Matrix4x4 Inverse(const Matrix4x4& m);

//ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

// 座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

//内積
float Dot(const Vector3& v1, const Vector3& v2);

// 長さ(ノルム)
float Length(const Vector3& v);

//正規化
Vector3 Normalize(const Vector3& v);

Vector3& operator+=(Vector3& v1, const Vector3& v2);

const Vector3 operator+(const Vector3& v1, const Vector3& v2);

Vector3& operator*=(Vector3& v, float s);

const Vector3 operator*(const Vector3& v, float s);

Vector3& operator-=(Vector3& v1, const Vector3&v2);

const Vector3 operator-(const Vector3& v1, const Vector3& v2);

Vector3& operator/=(Vector3& v, float s);

const Vector3 operator/(const Vector3& v, float s);

