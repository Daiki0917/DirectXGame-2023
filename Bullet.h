﻿#include <Model.h>
#include "Vector3.h"
#include <WorldTransform.h>
/// <summary>
/// 自キャラの弾
/// </summary>
class PlayerBullet {

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Initialize(Model* model, const Vector3& position,const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション
	/// </param>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 衝突を検出したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision();

	bool IsDead() const { return isDead_; }

	Vector3 GetWorldPosition();

	const float GetPlayerBulletRadius() { return playerBulletRadius; }
	const float playerBulletRadius = 1.0f;


private:
	WorldTransform  worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Vector3 velocity_;
	//寿命<frm>
	static const int32_t kLifeTime = 60 * 5;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;
};