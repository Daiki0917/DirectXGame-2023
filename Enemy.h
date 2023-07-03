#pragma once 
#include "Model.h"
#include "WorldTransform.h"
#include "EnemyBullet.h"
#include "MathUtility.h"
#include <list>

//自キャラの前方宣言
class Player;

// GameSceneの全方宣言(苦肉の策)
class GameScene;
/// <summary>
/// 敵
/// </summary>
class Enemy {
	
    

public:
	static const int kFireInterval = 60;
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

	/// <summary>
	/// 接近フェーズ初期化
	/// </summary>
	void ApproachInitialize();

	/// <summary>
	/// 接近フェーズ更新
	/// </summary>
	void ApproachUpdate();

	/// <summary>
	/// 衝突を検出したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy();

	void SetPlayer(Player* player) { player_ = player; }
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//ゲットです
	bool GetIsDead() { return isEnemyDead_; }


	//弾リストを取得
	//const std::list<EnemyBullet*>& GetBullets() { return bullets_; }

	const float GetEnemyRadius() { return enemyRadius; }
	const float enemyRadius = 1.0f;

private:
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHandle_;
	Vector3 velocity_;
	// 行動フェーズ
	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	};
	Phase phase_ = Phase::Approach;
	// 弾
	/*EnemyBullet* bullet_ = nullptr;*/
	//std::list<EnemyBullet*> bullets_;
	//発射タイマー
	int32_t fireTimer = 0;
	//自キャラ
	Player* player_ = nullptr;
	//ゲームシーン
	GameScene* gameScene_ = nullptr;
	// デスフラグ
	bool isEnemyDead_ = false;
	// デスタイマー
	int32_t deathEnemyTimer_ = 60;

};


