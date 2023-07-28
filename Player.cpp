#include "Player.h"


Player::~Player() {
	// bullet_の開放
	for (PlayerBullet* bullet : bullets_)
	{
		delete bullet;
	}

	delete sprite2DReticle_;
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="model"></param>
/// <param name="textureHandle"></param>
void Player::Initialize(Model* model, uint32_t textureHandle,Vector3 pos) {
	// NULLポイントチェック
	assert(model);

	// 受け渡し
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.translation_= pos;

	// ワールド変換の初期化
	worldTransform_.Initialize();

	//3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();

	// シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	//レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("target.png");

	//スプライト生成
	sprite2DReticle_ = Sprite::Create(textureReticle, {640, 360}, {1, 1, 1, 1}, {0.5f,0.5f});
}

/// <summary>
/// 更新
/// </summary>
void Player::Update(ViewProjection& viewProjection) {

	//回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A))
	{
		worldTransform_.rotation_.y -= kRotSpeed;
	}
	else if (input_->PushKey(DIK_D))
	{
		worldTransform_.rotation_.y += kRotSpeed;
	}
	//デスフラグの立った球の削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	
	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キャラクターの移動速さ
	const float kCharcterSpeed = 0.2f;

	// 押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharcterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) 
	{
		move.x += kCharcterSpeed;
	}

	// 押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_UP)) 
	{
		move.y += kCharcterSpeed;
	} 
	else if (input_->PushKey(DIK_DOWN)) 
	{
		move.y -= kCharcterSpeed;
	}

	//自機のワールド座標から3Dレティクルのワールド座標を計算
	{ 
		//自機から3Dレティクルへの距離
		const float kDistancePlayerTo3DReticle = 50.0f;

		//自機から3Dレティクルへのオフセット(Z+向き)
		Vector3 offset = {0, 0, 1.0f};

		//自機のワールド行列の回転を反映
		offset = TransformNormal(offset, worldTransform_.matWorld_); 

		//ベクトルの長さを整える
		offset = Normalize(offset) * kDistancePlayerTo3DReticle;

		//3Dレティクルの座標を設定
		worldTransform3DReticle_.translation_ = GetWorldPosition() + offset;

		//ワールド行列更新と転送
		worldTransform3DReticle_.UpdateMatrix();
	}

	//3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	{ 
		Vector3 positionReticle = worldTransform3DReticle_.translation_;

		//ビューポート行列
		Matrix4x4 matViewport =MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

		//ビュー行列とプロジェクト行列、ビューポート行列を合成する
		Matrix4x4 matViewProjectionViewport =
		   viewProjection.matView * viewProjection.matProjection * matViewport;

		//ワールド➝スクリーン座標変換（ここで3Dから2Dになる)
		positionReticle = Transform(positionReticle, matViewProjectionViewport);

		//スプライトのレティクルに座標設定
		sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
	}

	// マウスカーソルのスクリーン座標からワールド座標を取得して3Dレティクル配置
	{
		POINT mousePosition;
		// マウス座標(スクリーン座標)を取得する
		GetCursorPos(&mousePosition);

		// クライアントエリア座標に変換する
		HWND hwnd = WinApp::GetInstance()->GetHwnd();
		ScreenToClient(hwnd, &mousePosition);

		sprite2DReticle_->SetPosition(Vector2(float(mousePosition.x), float(mousePosition.y)));

		// ビュープロジェクションビューポート合成行列
		Matrix4x4 viewPort =
		    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

		Matrix4x4 matVPV = viewProjection.matView * viewProjection.matProjection * viewPort;

		// 合成行列の逆行列を計算する
		Matrix4x4 matInverseVPV = Inverse(matVPV);

		// スクリーン座標
		Vector3 posNear = Vector3(float(mousePosition.x), float(mousePosition.y), 0);
		Vector3 posFar = Vector3(float(mousePosition.x), float(mousePosition.y), 1);

		// スクリーン座標系からワールド座標系へ
		posNear = Transform(posNear, matInverseVPV);
		posFar = Transform(posFar, matInverseVPV);

		// マウスレイの方向
		Vector3 mouseDirection = posFar - posNear;
		mouseDirection = Normalize(mouseDirection);
		// カメラから照準オブジェクトの距離
		const float kDistanceTestObject = 100.0f;
		worldTransform3DReticle_.translation_ = posNear + mouseDirection * kDistanceTestObject;
		// ワールド行列の更新と転送
		worldTransform3DReticle_.UpdateMatrix();

	}

	// 範囲制限
	const float kMoveLimitX = 20.0f;
	const float kMoveLimitY = 20.0f;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	// 座標移動（ベクトルの加算）
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	worldTransform_.matWorld_ = MakeAffineMatrix(
	worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// 行列を定数バッファーに転送
	worldTransform_.TransferMatrix();

#ifdef DEBUG
	// キャラクターの座標を画面表示する処理
	ImGui::Begin("Player");
	float sliderValue[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	ImGui::SliderFloat3("position", sliderValue, -20.0f, 20.0f);
	worldTransform_.translation_ = {sliderValue[0], sliderValue[1], sliderValue[2]};
	ImGui::End();
#endif // DEBUG

	//キャラクター攻撃処理
	Attack();

	//弾更新
	for (PlayerBullet* bullet:bullets_)
	{
		bullet->Update();
	}
}

/// <summary>
/// 描画
/// </summary>
void Player::Draw(ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//reticleModel_->Draw(worldTransform3DReticle_, viewProjection);

	for (PlayerBullet*bullet:bullets_)
	{
		bullet->Draw(viewProjection);
	}
	
}

void Player::DrawUI() 
{
	sprite2DReticle_->Draw(); 
}

/// <summary>
/// 攻撃
/// </summary>
void Player::Attack(){ 
	if (input_->TriggerKey(DIK_SPACE))
	{
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity,worldTransform_.matWorld_);

		 //自機から標準オブジェクトへのベクトル
		velocity = worldTransform3DReticle_.translation_ - worldTransform_.translation_;
		velocity = Normalize(velocity) * 1;

		//弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);

		// 弾を登録する
		bullets_.push_back(newBullet);

		
    }
}



Vector3 Player::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
void Player::OnCollision() 
{

}

