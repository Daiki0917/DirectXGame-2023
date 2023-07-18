#include "Model.h"
#include "WorldTransform.h"
#include"input/Input.h"
#include "ImGuiManager.h"
#include <cassert>
#include "Bullet.h"
#include<list>
#include"MathUtility.h"
#include<cmath>
#include<math.h>
#include<iostream>
#include"Vector3.h"
#include <Sprite.h>
#pragma once

/// <summary>
/// 自キャラ
/// </summary>
class Player {

public:
	///<summary>
	///初期化
	///</summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textureHandle);

	/// <summary>
	/// 更新
	/// </summary>
	void Update(ViewProjection& viewProjection);

	/// <summary>
	/// 描画
	/// </summary>
	/// /// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(ViewProjection&viewProjection);

	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();


	/// <summary>
	/// 衝突を検出したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision();

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() { return bullets_; }
	
	const float GetPlayerRadius() { return playerRadius; }
	const float playerRadius = 1.0f;

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	// ピュープロジェクション
	ViewProjection viewProjection_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//キーボード入力
	Input* input_ = nullptr;
	//弾
	/*PlayerBullet* bullet_ = nullptr;*/
	std::list<PlayerBullet*> bullets_;
	//3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;
	//3Dレティクルモデル
	Model* reticleModel_ = nullptr;
	//2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;
};

