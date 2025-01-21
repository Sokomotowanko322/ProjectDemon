#pragma once
#include "Common/Transform.h"

class Player;
class ResourceManager;
class SceneManager;
class AnimationController;

class EnemyBase
{
public:
	// コンストラクタ
	EnemyBase(void);

	// デストラクタ
	~EnemyBase();

	virtual void Init(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	const Transform& GetEnemyTransform(void) const;
	virtual VECTOR GetPos(void) const;

private:

protected:

	// プレイヤー
	std::weak_ptr<Player> player_;

	// シングルトン参照
	ResourceManager& resMng_;
	SceneManager& scnMng_;
	ColliderManager& colMng_;

	// アニメーション制御
	std::unique_ptr<AnimationController> animationController_;

	// モデル制御の基本情報
	Transform enemyTransform_;

	// 回転の差分
	VECTOR diff_;
	
	// クォータニオン回転
	Quaternion quaRot_;
	// クォータニオン回転値代入用
	Quaternion rot_;

	// 生存フラグ
	bool isAlive_;

	// 回転の内積
	float dot_;

};
