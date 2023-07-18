#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "DebugCamera.h"
#include "RailCamera.h"
#include<list>
#include<sstream>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();


	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	/// <param name="enemyBullet"></param>
	void AddEnemyBullet(EnemyBullet* enemyBullet);
	/// <summary>
	/// 敵追加
	/// </summary>
	/// <param name="pos"></param>
	void AddEnemy(Vector3 pos);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	uint32_t textureHandle_ = 0;
	// 3Dモデル
	Model* model_ = nullptr;
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// ピュープロジェクション
	ViewProjection viewProjection_;
	// 自キャラ
	Player* player_ = nullptr;
	//敵キャラ
	Enemy* enemy_ = nullptr;
	//敵リスト
	std::list<Enemy*> enemys_;
	//敵弾
	EnemyBullet* enemyBullet_ = nullptr;
	//敵弾リスト
	std::list<EnemyBullet*> bullets_;
	// デバックカメラ
	DebugCamera* debugCamera_ = nullptr;
	// デバックカメラ有効
	bool isDebugCameraActive_ = true;
	//3Dモデル
	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;
	//レールカメラ
	RailCamera* reilCamera_ = nullptr;
	//レールカメラ有効
	bool isReilCamera_ = true;
	//敵発生コマンド
	std::stringstream enemyPopCommands;
	//待機フラグ
	float standFlag = false;
	//待機タイマー
	int32_t standTimer = 120;

};


//	//Sprite* sprite_ = nullptr;
//
//	/// <summary>
//	/// ゲームシーン用
//	/// </summary>
//};
