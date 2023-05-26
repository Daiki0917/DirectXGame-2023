#include <Model.h>
#include <WorldTransform.h>

/// <summary>
/// 敵
/// </summary>
class Enemy {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize( Model* model,const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;
	Model* model_ ;
	uint32_t textureHandle_;
	Vector3 velocity_;
};
