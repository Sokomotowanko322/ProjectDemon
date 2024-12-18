#pragma once
#include <memory>
#include "SceneBase.h"
class Stage;
class SkyDome;
class Player;
class Soul;
class NormalEnemy;
class TestEnemy;

class GameScene : public SceneBase
{

public:
	
	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:

	// ステージ
	std::unique_ptr<Stage> stage_;
	
	// 魂
	std::shared_ptr<Soul> soul_;

	// 敵
	std::shared_ptr<NormalEnemy> normalEnemy_;
	std::shared_ptr<TestEnemy> testEnemy_;

	// スカイドーム
	std::unique_ptr<SkyDome> skyDome_;

	// プレイヤー
	std::shared_ptr<Player> player_;
};
