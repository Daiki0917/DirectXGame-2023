#include "Skydome.h"

void Skydome::Initialize(Model* model) 
{
	// 受け渡し
	model_ = model;
	// ワールド変換の初期化
	worldTransform_.Initialize();
}

void Skydome::Update() 
{  
	//行列を計算・転送する
	worldTransform_.UpdateMatrix();
}

void Skydome::Draw(ViewProjection& viewProjection)
{
	//3Dモデル描画
	model_->Draw(worldTransform_, viewProjection);
}