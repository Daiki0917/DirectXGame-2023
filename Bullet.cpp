#include "Bullet.h"
#include "cassert"
#include<cmath>
#include<math.h>
#include<iostream>

void PlayerBullet::Initialize(Model* model, const Vector3& position) {
	 //NULLポイントチェック
	assert(model);

	// 受け渡し
	model_ = model;
	textureHandle_ = TextureManager::Load("white1x1.png");
	// ワールド変換の初期化
	worldTransform_.Initialize();

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

}
void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void PlayerBullet::Update() 
{ 
	worldTransform_.UpdateMatrix(); 
}