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

	// 振動
	void Vibration(void);

private:

	// 衝突判定
	void SetPlayerAttackCollision(void);
	void SetCollision(void);

	// 敵の生成
	void MakeEnemies(void);
	
	// 魂の生成
	void MakeSouls(void);

	void DrawHp(float hp, float maxHp, int x, int y, int width, int height);

	
	// ステージ
	std::unique_ptr<Stage> stage_;
	
	// 魂
	std::vector<std::shared_ptr<Soul>> soul_;

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
	
	float soulAliveCnt_;
};
