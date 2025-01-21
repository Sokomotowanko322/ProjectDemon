#pragma once
#include <memory>
#include <map>
#include "Common/Transform.h"

class ResourceManager;
class Collision;
class ColliderController;
class Player;

class Stage
{

public:

	// ステージの切り替え間隔
	static constexpr float TIME_STAGE_CHANGE = 1.0f;

	// ステージ名
	enum class NAME
	{
		STAGE,
	};

	// コンストラクタ
	Stage(std::weak_ptr<Player> player);

	// デストラクタ
	~Stage(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	// コライダを設定するmodelIdの取得
	int GetModelId(void);

private:

	Transform stageTransform_;
	Transform colliderTransform_;

	// シングルトン参照
	ResourceManager& resMng_;
	Collision& collision_;
	SceneManager& scnMng_;

	// プレイヤー
	std::shared_ptr<Player> player_;

	//衝突判定検知
	std::unique_ptr<ColliderController> colliderController_;

	// 環境オブジェクト
	void MakeEnvironment(void);

	// 衝突判定
	void SetCollisionStage(void);

};
