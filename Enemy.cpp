#include "Enemy.h"

void Enemy::Initialize(Model* model,const Vector3& position) {
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	//モデル
	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("kedama.png");

	// ワールド変換の初期化
	worldTransform_.Initialize();

	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = {-0.2f, -0.2f, -0.2f};
}

void Enemy::Update()
{
	switch (phase_) {
	case Phase::Approach:
	default:
		// 移動(ベクトルを加算)
		worldTransform_.translation_.z += velocity_.z;
		// 規定の位置に到達したら離脱
		if (worldTransform_.translation_.z < 0.0f) {
			phase_ = Phase::Leave;
		}
		break;
	case Phase::Leave:
		// 移動(ベクトルを加算)
		worldTransform_.translation_.x += velocity_.x;
		worldTransform_.translation_.y += velocity_.y;
		break;
	}
	// 座標を移動させる(1フレーム分の移動量を足しこむ)
	/*worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;*/

	// 座標を元に行列の更新を行う。
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) 
{
    // モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}