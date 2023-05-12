#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include <Input.h>

class Bullet {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="mode">モデル</param>
	/// <param name="position">初期座標</param>
	void Intiialize(Model* model, const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	
private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	// キーボード入力
	Input* input_ = nullptr;

};