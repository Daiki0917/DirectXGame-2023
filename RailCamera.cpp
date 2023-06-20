#include "RailCamera.h"

void RailCamera::Initialize(const Vector3& position, const Vector3& radian)
{
	//ワールドトランスフォームの設定
	//ワールド座標を受け取り設定
	worldTransform_.translation_ = position;
	//回転角(ラジアン)を受け取り設定
	worldTransform_.rotation_ = radian;
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void RailCamera::Update() 
{
	//移動
	Vector3 move = {0.0f, 0.0f, 0.0f};
	//回転
	Vector3 rotate = {0.0f, 0.0f, 0.0f};
	// ワールドトランスフォームの座標の数値を加算したりする(移動)
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;
	// ワールドトランスフォームの角度の数値を加算したりする(回転)
	worldTransform_.rotation_.x += rotate.x;
	worldTransform_.rotation_.y += rotate.y;
	worldTransform_.rotation_.z += rotate.z;
	//ワールドトランスフォームのワールド行列再計算
	worldTransform_.UpdateMatrix();

	//カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	//カメラの座標を画面表示する処理
	ImGui::Begin("Camera");
	//スライダーでカメラのtranslationを表示
	ImGui::DragFloat3("translation", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("rotation", &worldTransform_.rotation_.x, 0.01f);
	ImGui::End();
}