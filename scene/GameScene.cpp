#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"AxisIndicator.h"


GameScene::GameScene() {}

GameScene::~GameScene() {
	delete debugCamera_;
	delete enemy_;
	delete player_;
	delete model_;
	delete skydome_;
	delete modelSkydome_; 
	delete reilCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	textureHandle_ = TextureManager::Load("sample.png");
	model_ = Model::Create();
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	player_->Initialize(model_,textureHandle_);
	//デバックカメラ
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	//レールカメラ
	reilCamera_ = new RailCamera();
	reilCamera_->Initialize({0,0,0},{0,0,0});
	//敵キャラの初期化
	enemy_ = new Enemy();
	//敵キャラの初期化
	enemy_->Initialize(model_, {1,1,50});
	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);
	//3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_);
	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するviewProjectionを参照する(アドレスなし)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
}

void GameScene::Update() 
{ 
	player_->Update();
	debugCamera_->Update();
	skydome_->Update();
	reilCamera_->Update();
	
	if (enemy_)
	{
		enemy_->Update();
	}
#ifndef _DEBUG
	if (input_->TriggerKey(DIK_1))
	{
		isDebugCameraActive_ = true;
	}
#endif // !_DEBUG
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} 
	else if (!isDebugCameraActive_)
	{
		reilCamera_->Update(); 
		viewProjection_.matView = reilCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = reilCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();

	}
	CheckAllCollisions();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	/*sprite_->Draw();*/
	
	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	player_->Draw(viewProjection_);
	if (enemy_)
	{
		enemy_->Draw(viewProjection_);
	}
	//天球
	skydome_->Draw(viewProjection_);
	Model::PostDraw();
	//3Dモデル描画
	/*model_->Draw(worldTransform_, viewProjection_, textureHandle_);*/

	// 3Dオブジェクト描画後処理
	
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

	//自キャラの描画
	//player_->Draw();
#pragma endregion
}

void GameScene::CheckAllCollisions() {
	// 判定対象AとBの座標
	Vector3 posA, posB;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

#pragma region 
	// 自キャラと敵弾の当たり判定
	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラと敵弾すべての当たり判定
	for (EnemyBullet* bullets : enemyBullets) {
		// 敵弾の座標
		posB = bullets->GetWorldPosition();

		float distance = (posB.x - posA.x) * (posB.x - posA.x) +
		                 (posB.y - posA.y) * (posB.y - posA.y) +
		                 (posB.z - posA.z) * (posB.z - posA.z);
		float length = (player_->playerRadius + bullets->enemyBulletRadius) *
		               (player_->playerRadius + bullets->enemyBulletRadius);

		// 弾と弾の交差判定
		if (distance <= length) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullets->OnCollision();
		}
	}
#pragma endregion

#pragma region 
	// 自弾と敵キャラの当たり判定
	//敵キャラの座標
	posA = enemy_->GetWorldPosition();

	//自弾と敵キャラすべての当たり判定
	for (PlayerBullet* bullets : playerBullets) {
	    //自弾の座標
		posB = bullets->GetWorldPosition();

		float distance = (posB.x - posA.x) * (posB.x - posA.x) +
		                 (posB.y - posA.y) * (posB.y - posA.y) +
		                 (posB.z - posA.z) * (posB.z - posA.z);
		float length = (enemy_->enemyRadius + bullets->playerBulletRadius) *
		               (enemy_->enemyRadius + bullets->playerBulletRadius);

		// 弾と弾の交差判定
		if (distance <= length) {
			// 敵キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();
			// 自弾の衝突時コールバックを呼び出す
			bullets->OnCollision();
		}
	}


#pragma endregion

#pragma region 
	// 自弾と敵弾の当たり判定
	for (PlayerBullet* pBullets : playerBullets){
		for (EnemyBullet* eBullets : enemyBullets) {
			// 自弾の座標
			posA = pBullets->GetWorldPosition();
			// 敵弾の座標
			posB = eBullets->GetWorldPosition();

			float distance = (posB.x - posA.x) * (posB.x - posA.x) +
			                 (posB.y - posA.y) * (posB.y - posA.y) +
			                 (posB.z - posA.z) * (posB.z - posA.z);

			float length = (pBullets->playerBulletRadius + eBullets->enemyBulletRadius) *
			               (pBullets->playerBulletRadius + eBullets->enemyBulletRadius);

			// 弾と弾の交差判定
			if (distance <= length) {
				// 敵弾の衝突時コールバックを呼び出す
				eBullets->OnCollision();
				// 自弾の衝突時コールバックを呼び出す
				pBullets->OnCollision();
			}
		}
	}
#pragma endregion
}




