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

GameScene::GameScene(void)
{
	player_ = nullptr;
	skyDome_ = nullptr;
	stage_ = nullptr;
}

GameScene::~GameScene(void)
{
	testEnemy_.clear();
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
	
	// 魂
	soul_ = std::make_shared<Soul>(player_);
	soul_->Init();

	// 敵オブジェクトの生成
	const auto& enemy = std::make_shared<TestEnemy>(player_);
	enemy->Init();
	
	MakeEnemies();
	
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

	for (const auto& e : testEnemy_)
	{
		e->Update();
	}

	player_->Update();
	soul_->Update();

	if (player_->IsAttack())
	{
		SetCollision();
	}
	
}

void GameScene::Draw(void)
{
	// 描画処理
	skyDome_->Draw();
	for (const auto& e : testEnemy_)
	{
		e->Draw();
	}
	soul_->Draw();
	stage_->Draw();
	player_->Draw();

	// ヘルプ
	DrawFormatString(840, 20, 0x000000, "移動　　：WASD");
	DrawFormatString(840, 40, 0x000000, "カメラ　：矢印キー");
	DrawFormatString(840, 60, 0x000000, "ダッシュ：右Shift");
	DrawFormatString(840, 80, 0x000000, "ジャンプ：＼(バクスラ)");
}

void GameScene::SetCollision(void)
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
		}
		/*VECTOR HitPos = weapon_->GetPos();*/

		player_->IsAttack();

		//// エフェクトの再生
		//player_->HitEffect(HitPos);
		const auto& enemy = testEnemy_.size();
		for (const auto& e : testEnemy_)
		{
			e->SetDamage(700);
		}
		
		// シェイクする
		camera_.SetCameraShake(1.0f, 5.0f, 20.0f);

		/*Vibration();*/

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

}
