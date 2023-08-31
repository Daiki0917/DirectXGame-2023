#include<Model.h>
#include "Vector3.h"
#include <WorldTransform.h>

/// <summary>
/// 敵キャラの弾
/// </summary>
class EnemyBullet {
public:

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 衝突を検出したら呼び出されるコールバック関数
	/// </summary>
	void OnCollision();

	bool IsDead() const { return isDead_; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	const float GetEnemyBulletRadius() { return enemyBulletRadius; }
	const float enemyBulletRadius = 1.0f;

private:

	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHandle_ = 0u;
	Vector3 velocity_;
	// 寿命<frm>
	static const int32_t kLifeTime = 60 * 5;
	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;
	//敵弾フラグ
	bool isAlive = true;
};
