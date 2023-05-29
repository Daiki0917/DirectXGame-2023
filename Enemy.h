#include <Model.h>
#include <WorldTransform.h>
#include<EnemyBullet.h>

/// <summary>
/// 敵
/// </summary>
class Enemy {
	
    // 行動フェーズ
	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	};

public:
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
	/// デストラクタ
	/// </summary>
	~Enemy();


private:
	WorldTransform worldTransform_;
	Model* model_;
	uint32_t textureHandle_;
	Vector3 velocity_;
	Phase phase_ = Phase::Approach;
	// 弾
	/*EnemyBullet* bullet_ = nullptr;*/
	std::list<EnemyBullet*> bullets_;
};
