#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include"AxisIndicator.h"
#include<fstream>


GameScene::GameScene() {}

GameScene::~GameScene() {
	delete debugCamera_;
	delete player_;
	delete model_;
	delete skydome_;
	delete modelSkydome_; 
	delete reilCamera_;
	for (Enemy* enemy : enemys_)
	{
		delete enemy;
	}
	// bullet_の開放
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	textureHandle_ = TextureManager::Load("sample.png");

	titleTexture_ = TextureManager::Load("title.png");
	spriteTitle_ = Sprite::Create(titleTexture_, {640, 360}, {1, 1, 1,1}, {0.5f, 0.5f});

	Operation_ = TextureManager::Load("sousa.png");
	spriteOperation_ = Sprite::Create(Operation_, {640, 360}, {1, 1, 1, 1}, {0.5f, 0.5f});

	GameClear_ = TextureManager::Load("GameClear.png");
	spriteGameClear_ = Sprite::Create(GameClear_, {640, 360}, {1, 1, 1, 1}, {0.5f, 0.5f});

	GameOver_ = TextureManager::Load("GameOver.png");
	spriteGameOver_ = Sprite::Create(GameOver_, {640, 360}, {1, 1, 1, 1}, {0.5f, 0.5f});

	model_ = Model::Create();
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	// 自キャラの生成
	player_ = new Player();
	Vector3 playerPosition = {0, 0, -30};
	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_, playerPosition);
	// デバックカメラ
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);
	// レールカメラ
	reilCamera_ = new RailCamera();
	reilCamera_->Initialize({0, 0, 0}, {0, 0, 0});
	// レティクルのテクスチャ
	TextureManager::Load("target.png");
	////敵キャラの初期化
	// enemy_ = new Enemy();
	////敵キャラの初期化
	// enemy_->Initialize(model_, {1,1,50});
	// enemy_->SetGameScene(this);
	////敵キャラに自キャラのアドレスを渡す
	// enemy_->SetPlayer(player_);

	// 3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_);

	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するviewProjectionを参照する(アドレスなし)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	AddEnemy({0.f, 5.f, 100.f});
	LoadEnemyPopData();
}

void GameScene::Update() {
	switch (sceneNo) {
	case TITLE:
		if (input_->TriggerKey(DIK_SPACE)) {
			sceneNo = OPERATION;
		}
		break;

	case OPERATION:
		if (input_->TriggerKey(DIK_SPACE)) {
			sceneNo = GAMEPLAY;
		}
		break;

	case GAMEPLAY:
			player_->Update(viewProjection_);
			debugCamera_->Update();
			skydome_->Update();
			reilCamera_->Update();
			// 弾更新
			// デスフラグの立った球の削除
			bullets_.remove_if([](EnemyBullet* bullet) {
				if (bullet->IsDead()) {
					delete bullet;
					return true;
				}
				return false;
			});
			for (EnemyBullet* bullet : bullets_) {
				bullet->Update();
			}

			// デスフラグの立った球の削除
			enemys_.remove_if([](Enemy* enemy) {
				if (enemy->GetIsDead()) {
					delete enemy;
				}
				return false;
			});

			for (Enemy* enemy : enemys_) {
				enemy->Update();
			}

			UpdateEnemyPopCommands();
#ifndef _DEBUG
			if (input_->TriggerKey(DIK_1)) {
				isDebugCameraActive_ = true;
			}
#endif // !_DEBUG
			if (isDebugCameraActive_) {
				debugCamera_->Update();
				viewProjection_.matView = debugCamera_->GetViewProjection().matView;
				viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
				viewProjection_.TransferMatrix();
			} else if (!isDebugCameraActive_) {
				reilCamera_->Update();
				viewProjection_.matView = reilCamera_->GetViewProjection().matView;
				viewProjection_.matProjection = reilCamera_->GetViewProjection().matProjection;
				viewProjection_.TransferMatrix();
			}
			CheckAllCollisions();

		    if (player_->GetPlayerAlive() == false)
			{
			    sceneNo = GAMEOVER;
		    }

			if (input_->TriggerKey(DIK_2))
			{
			    sceneNo = GAMECLEAR;
			}
		break;

	case GAMECLEAR:
		if (input_->TriggerKey(DIK_1)) {
			sceneNo = TITLE;
		}
		break;

	case GAMEOVER:
		if (input_->TriggerKey(DIK_1)) {
			sceneNo = TITLE;
		}
		break;
	}
}


void GameScene::Draw(){
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	switch (sceneNo) 
	{
	case TITLE:
		// 前景スプライト描画前処理
		Sprite::PreDraw(commandList);

		/// <summary>
		/// ここに前景スプライトの描画処理を追加できる
		/// </summary>
		spriteTitle_->Draw();
		// スプライト描画後処理
		Sprite::PostDraw();
		break;

	case OPERATION:
		// 前景スプライト描画前処理
		Sprite::PreDraw(commandList);

		/// <summary>
		/// ここに前景スプライトの描画処理を追加できる
		/// </summary>
		spriteOperation_->Draw();
		// スプライト描画後処理
		Sprite::PostDraw();
		break;

	case GAMEPLAY:
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
		for (Enemy* enemy : enemys_) {
			enemy->Draw(viewProjection_);
		}
		// 天球
		skydome_->Draw(viewProjection_);

		// 弾の描画
		for (EnemyBullet* bullet : bullets_) {
			bullet->Draw(viewProjection_);
		}
		Model::PostDraw();
		// 3Dモデル描画
		/*model_->Draw(worldTransform_, viewProjection_, textureHandle_);*/

		// 3Dオブジェクト描画後処理

#pragma endregion

#pragma region 前景スプライト描画
		// 前景スプライト描画前処理
		Sprite::PreDraw(commandList);

		/// <summary>
		/// ここに前景スプライトの描画処理を追加できる
		/// </summary>
		player_->DrawUI();
		// スプライト描画後処理
		Sprite::PostDraw();

		// 自キャラの描画
		// player_->Draw();
#pragma endregion
		break;

	case GAMECLEAR:
		// 前景スプライト描画前処理
		Sprite::PreDraw(commandList);

		/// <summary>
		/// ここに前景スプライトの描画処理を追加できる
		/// </summary>
		spriteGameClear_->Draw();
		// スプライト描画後処理
		Sprite::PostDraw();
		break;

	case GAMEOVER:
		// 前景スプライト描画前処理
		Sprite::PreDraw(commandList);

		/// <summary>
		/// ここに前景スプライトの描画処理を追加できる
		/// </summary>
		spriteGameOver_->Draw();
		// スプライト描画後処理
		Sprite::PostDraw();
		break;
	}
}

void GameScene::CheckAllCollisions() {
	// 判定対象AとBの座標
	Vector3 posA, posB;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = bullets_;

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
	//自弾と敵キャラすべての当たり判定
	for (PlayerBullet* bullets : playerBullets) {
		for (Enemy* enemy :enemys_) 
		{
			// 敵キャラの座標
			posA = enemy->GetWorldPosition();
			// 自弾の座標
			posB = bullets->GetWorldPosition();

			float distance = (posB.x - posA.x) * (posB.x - posA.x) +
			                 (posB.y - posA.y) * (posB.y - posA.y) +
			                 (posB.z - posA.z) * (posB.z - posA.z);
			float length = (enemy->enemyRadius + bullets->playerBulletRadius) *
			               (enemy->enemyRadius + bullets->playerBulletRadius);

			// 弾と弾の交差判定
			if (distance <= length) 
			{
				// 敵キャラの衝突時コールバックを呼び出す
				enemy->OnCollision();
				// 自弾の衝突時コールバックを呼び出す
				bullets->OnCollision();
			}
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

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet)
{
	//リストに登録する
	bullets_.push_back(enemyBullet);
}

void GameScene::AddEnemy(Vector3 pos)
{
	//敵の作成
	Enemy* obj = new Enemy();
	//敵の初期化
	obj->Initialize(model_, pos);
	//敵キャラに自キャラのアドレスを渡す
	obj->SetPlayer(player_);
	//敵キャラにゲームシーンを渡す
	obj->SetGameScene(this);

	//リストに登録
	enemys_.push_back(obj);
}


void GameScene::LoadEnemyPopData()
{
	//ファイルを開く
	std::ifstream file;
	file.open("./Resources/enemyPop.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands()
{
	//待機処理
	if (standFlag)
	{
		standTimer--;
		if (standTimer <= 0)
		{
			// 待機完了
			standFlag = false;
		}
		return;
	}
	
	//1行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (getline(enemyPopCommands, line))
	{
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');
		//"//から始まる行はコメント
		if (word.find("//") == 0)
		{
			//コメント行を飛ばす
			continue;
		}
		//POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			AddEnemy(Vector3(x, y, z));
		}
			//WAITコマンド
		else if (word.find("WAIT") == 0)
		{
			getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = atoi(word.c_str());

			//待機開始
			standFlag = true; // 待機中フラグ
			standTimer=waitTime;//待機タイマー
			//コマンドループを抜ける
			break;
		}
	}
}