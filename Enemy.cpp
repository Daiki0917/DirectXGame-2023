#include "Enemy.h"

Enemy::~Enemy() {
	// bullet_の開放
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

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

	//発射関数の呼び出し
	//Fire();

	//接近フェーズ
	ApproachInitialize();

}

void Enemy::Update()
{
	// デスフラグの立った球の削除
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	switch (phase_) {
	case Phase::Approach:
	default:
		// 移動(ベクトルを加算)
		worldTransform_.translation_.z += velocity_.z;
		// 規定の位置に到達したら離脱
		if (worldTransform_.translation_.z < 0.0f) {
			phase_ = Phase::Leave;
		}
		ApproachUpdate();
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


	// 弾更新
	for (EnemyBullet* bullet:bullets_) 
	{
		bullet->Update();
	}
	
}

void Enemy::Draw(ViewProjection& viewProjection) 
{
    // モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	//弾の描画
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::Fire()
{
	assert(player_);

	// 弾の速度
	const float kBulletSpeed = 1.0f;

	//自キャラのワールド座標を取得する
	player_->GetWorldPosition();
	//敵キャラのワールド座標を取得する
	GetWorldPosition();
	//敵キャラ->自キャラの差分ベクトルを求める
	Vector3 vector = {
	player_->GetWorldPosition().x - GetWorldPosition().x,
	player_->GetWorldPosition().y - GetWorldPosition().y,
	player_->GetWorldPosition().z - GetWorldPosition().z};
	//ベクトルの正規化
	float length = 
	sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z); 
	//ベクトルの長さを、早さに合わせる
	assert(length != 0.0f);
	Vector3 dir = {vector.x / length, vector.y / length, vector.z / length};

	Vector3 velocity = {dir.x * kBulletSpeed, dir.y * kBulletSpeed, dir.z * kBulletSpeed};
	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	// 弾を登録する
	bullets_.push_back(newBullet);
}

void Enemy::ApproachInitialize() {
	// 発動タイマーを初期化
	fireTimer = kFireInterval;
}

void Enemy::ApproachUpdate() {
	// 発射タイマーカウントダウン
	fireTimer--;
	// 指定時間に達した
	if (fireTimer <= 0) {
		// 弾を発射
		Fire();
		// 発動タイマーを初期化
		fireTimer = kFireInterval;
	}
}

void Enemy::OnCollision() {}

Vector3 Enemy::GetWorldPosition() {
     //ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}