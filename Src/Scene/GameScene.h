#pragma once
#include <memory>
#include "SceneBase.h"
class Stage;
class SkyDome;
class ColliderController;
class Player;
class Weapon;
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

	// 衝突判定
	bool CheckAttackCollisions(const VECTOR& p1, float r1, const VECTOR& p2, float r2);
	void SetCollision(void);

	void MakeEnemies(void);

	// ステージ
	std::unique_ptr<Stage> stage_;
	
	// 魂
	std::shared_ptr<Soul> soul_;

	// 敵オブジェクト
	std::vector<std::shared_ptr<TestEnemy>> testEnemy_;

	// 武器
	std::shared_ptr<Weapon> weapon_;
	
	// 衝突判定検知
	std::unique_ptr<ColliderController> colliderController_;

	// スカイドーム
	std::unique_ptr<SkyDome> skyDome_;

	// プレイヤー
	std::shared_ptr<Player> player_;
};
