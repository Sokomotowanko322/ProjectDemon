#include <DxLib.h>
#include "../Utility/Utility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Manager/InputManager.h"
#include "../Object/Common/Capsule.h"
#include "../Object/Common/Collider/Collision.h"
#include "../Object/Common/Collider/Collider.h"
#include "../Object/Common/Collider/ColliderManager.h"
#include "../Object/Common/Collider/ColliderController.h"
#include "../Object/SkyDome.h"
#include "../Object/Stage.h"
#include "../Object/Soul.h"
#include "../Object/TestEnemy.h"
#include "../Object/NormalEnemy.h"
#include "../Object/Common/Weapon.h"
#include "../Object/Player.h"
#include "GameScene.h"

const int DEBUG_DAMAGE = 700;
const int DAMAGE = 150;
const float SOUL_ALIVECNT = 60.0f;

const int VIBRATION_POW_MAX = 1000;
const int VIBRATION_TIME_WEAK = 150;
const int VIBRATION_TIME_STRONG = 500;
const int DAMAGE_THRESHOLD_FOR_VIBRATION = 400;

GameScene::GameScene(void)
{
	player_ = nullptr;
	skyDome_ = nullptr;
	stage_ = nullptr;
}

GameScene::~GameScene(void)
{
	testEnemy_.clear();
	soul_.clear();
}

void GameScene::Init(void)
{
	// プレイヤー
	player_ = std::make_shared<Player>();
	player_->Init();
	weapon_ = std::make_shared<Weapon>();
	colliderController_ = std::make_unique<ColliderController>();
	colliderController_->SetCollision(OBJECT_TYPE::PLAYER);

	// ステージ
	stage_ = std::make_unique<Stage>(player_);
	stage_->Init();

	// 敵オブジェクトの生成
	const auto& enemy = std::make_shared<TestEnemy>(player_);
	enemy->Init();
	MakeEnemies();

	// 敵オブジェクトの生成
	const auto& s = std::make_shared<Soul>(player_, enemy->GetPos());

	// 生存時間
	soulAliveCnt_ = SOUL_ALIVECNT;

	// スカイドーム
	skyDome_ = std::make_unique<SkyDome>(player_->GetTransform());
	skyDome_->Init();

	colliderController_ = std::make_unique<ColliderController>();
	colliderController_->SetCollision(OBJECT_TYPE::PLAYER);
	mainCamera.SetFollow(&player_->GetTransform());
	mainCamera.ChangeMode(Camera::MODE::FOLLOW);

	auto& insI = InputManager::GetInstance();
	auto pad = insI.GetJPadState(JOYPAD_NO::PAD1);
	insI.SetControlType(InputManager::CONTROL_TYPE::CONTROLLER);
}

void GameScene::Update(void)
{
	float delta = SceneManager::GetInstance().GetDeltaTime();
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTriggered(InputManager::ACTION::SCENE_CHANGE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
	}

	// カメラの衝突判定
	camera_.SetHitModel(stage_->GetModelId());

	skyDome_->Update();

	stage_->Update();

	bool isOnce = false;


	player_->Update();
	if (player_->IsAttack())
	{
		SetPlayerAttackCollision();
	}

	// 敵の更新
	for (const auto& e : testEnemy_)
	{
		soulAliveCnt_ -= delta;
		e->Update();
		if (!e->IsAlive() && !isOnce)
		{
			isOnce = true;
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
		}
		if (e->IsAttack())
		{
			SetCollision();
		}
		if (!e->IsAlive() && soulAliveCnt_ >= 0.0f)
		{
			// 魂
			// 魂を生成してvectorに追加
			const auto& soul = std::make_shared<Soul>(player_, e->GetPos());
			soul_.emplace_back(soul);
			//souls->Update();
		}
	}
	//// 生成済みの魂を更新
	//for (auto& soul : soul_)
	//{
	//	soul->Update();
	//}

}

void GameScene::Draw(void)
{
		// 描画処理
	skyDome_->Draw();
	for (const auto& e : testEnemy_)
	{
		e->Draw();
		
	}
	for (const auto& s : soul_)
	{
		s->Draw();
	}
	stage_->Draw();
	player_->Draw();
	

	// HPを描画
	DrawHp(player_->GetHp(), player_->GetMaxHp(), 0, 10, 500, 30);
	
	// ヘルプ
	/*DrawFormatString(840, 20, 0x000000, "移動　　：WASD");
	DrawFormatString(840, 40, 0x000000, "カメラ　：矢印キー");
	DrawFormatString(840, 60, 0x000000, "ダッシュ：右Shift");
	DrawFormatString(840, 80, 0x000000, "ジャンプ：＼(バクスラ)");*/
}

void GameScene::SetPlayerAttackCollision(void)
{
	colliderController_->SetCollision(OBJECT_TYPE::WEAPON);
	
	const auto& hits = colliderController_->OnCollision(OBJECT_TYPE::ENEMY_MODEL);
	const auto& hitpoly = colliderController_->OnCollisionResultPoly(OBJECT_TYPE::ENEMY_MODEL);

	if (hits)
	{
		auto& hit = hitpoly.Dim;

		for (const auto& e : testEnemy_)
		{
			VECTOR HitPos = e->GetPos();
			// エフェクトの再生
			player_->HitEffect(HitPos);

			// ダメージを与える
			e->SetDamage(DAMAGE);
		}

		// シェイクする
		camera_.SetCameraShake(1.0f, 5.0f, 20.0f);

		Vibration();

	}


}

void GameScene::SetCollision(void)
{
	colliderController_->SetCollision(OBJECT_TYPE::ENEMY_MODEL);
	colliderController_->SetCollisionSub(ENEMY_SUBTYPE::HAND_L);

	const auto& hits = colliderController_->OnCollision(OBJECT_TYPE::PLAYER);
	const auto& hitpoly = colliderController_->OnCollisionResultPoly(OBJECT_TYPE::PLAYER);

	if (hits)
	{
		auto& hit = hitpoly.Dim;


		//VECTOR HitPos = e->GetPos();
		//// エフェクトの再生
		//player_->HitEffect(HitPos);

		// ダメージを与える
		player_->SetDamage(DEBUG_DAMAGE);

		if (player_->GetHp() <= 0)
		{
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
		}
	}

}

void GameScene::MakeEnemies(void)
{
	auto normalEnemy = std::make_shared<TestEnemy>(player_);
	normalEnemy->Init();
	normalEnemy->SetPos({ -300.0f, -30.0f, 500.0f });
	testEnemy_.emplace_back(std::move(normalEnemy));

	normalEnemy = std::make_shared<TestEnemy>(player_);
	normalEnemy->Init();
	normalEnemy->SetPos({ 250.0f, -60.0f, 500.0f });
	testEnemy_.emplace_back(std::move(normalEnemy));

	normalEnemy = std::make_shared<TestEnemy>(player_);
	normalEnemy->Init();
	normalEnemy->SetPos({ -20.0f, -45.0f, 500.0f });
	testEnemy_.emplace_back(std::move(normalEnemy));
	
	normalEnemy = std::make_shared<TestEnemy>(player_);
	normalEnemy->Init();
	normalEnemy->SetPos({ -100.0f, -30.0f, -500.0f });
	testEnemy_.emplace_back(std::move(normalEnemy));
	
	normalEnemy = std::make_shared<TestEnemy>(player_);
	normalEnemy->Init();
	normalEnemy->SetPos({ 160.0f, -30.0f, -500.0f });
	testEnemy_.emplace_back(std::move(normalEnemy));
	
}

void GameScene::DrawHp(float hp, float maxHp, int x, int y, int width, int height)
{
	//// HPの割合を計算
	float hpRatio = hp / maxHp;

	// 描画するHPの幅
	int hpWidth = static_cast<int>(width * hpRatio);

	// 緑色でHPバーを描画
	DrawBox(x, y, x + hpWidth, y + height, GetColor(0, 255, 0), true); 
}

void GameScene::Vibration(void)
{
	int damageShow_ = 500;
	int pow = damageShow_;
	int vibtime = VIBRATION_TIME_WEAK;

	if (pow >= VIBRATION_POW_MAX)
	{
		pow = VIBRATION_POW_MAX;
	}

	if (damageShow_ >= DAMAGE_THRESHOLD_FOR_VIBRATION)
	{
		vibtime = VIBRATION_TIME_STRONG;
	}

	StartJoypadVibration(DX_INPUT_PAD1, pow, vibtime, -1);
}
