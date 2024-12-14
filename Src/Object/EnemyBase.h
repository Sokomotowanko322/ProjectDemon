#pragma once
#include "Common/Transform.h"

class ResourceManager;
class SceneManager;

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

	const Transform& GetTransform(void) const;
	virtual VECTOR GetPos(void) const;

private:

protected:

	// シングルトン参照
	ResourceManager& resMng_;
	SceneManager& scnMng_;

	// モデル制御の基本情報
	Transform transform_;
};
