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
	// �v���C���[
	player_ = std::make_shared<Player>();
	player_->Init();
	weapon_ = std::make_shared<Weapon>();
	colliderController_ = std::make_unique<ColliderController>();
	colliderController_->SetCollision(OBJECT_TYPE::PLAYER);

	// �X�e�[�W
	stage_ = std::make_unique<Stage>(player_);
	stage_->Init();
	
	// ��
	soul_ = std::make_shared<Soul>(player_);
	soul_->Init();

	// �G�I�u�W�F�N�g�̐���
	const auto& enemy = std::make_shared<TestEnemy>(player_);
	enemy->Init();
	
	MakeEnemies();
	
	// �X�J�C�h�[��
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
	// �V�[���J��
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTriggered(InputManager::ACTION::SCENE_CHANGE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT);
	}

	// �J�����̏Փ˔���
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
	// �`�揈��
	skyDome_->Draw();
	for (const auto& e : testEnemy_)
	{
		e->Draw();
	}
	soul_->Draw();
	stage_->Draw();
	player_->Draw();

	// �w���v
	DrawFormatString(840, 20, 0x000000, "�ړ��@�@�FWASD");
	DrawFormatString(840, 40, 0x000000, "�J�����@�F���L�[");
	DrawFormatString(840, 60, 0x000000, "�_�b�V���F�EShift");
	DrawFormatString(840, 80, 0x000000, "�W�����v�F�_(�o�N�X��)");
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
			// �G�t�F�N�g�̍Đ�
			player_->HitEffect(HitPos);
		}
		/*VECTOR HitPos = weapon_->GetPos();*/

		player_->IsAttack();

		//// �G�t�F�N�g�̍Đ�
		//player_->HitEffect(HitPos);
		const auto& enemy = testEnemy_.size();
		for (const auto& e : testEnemy_)
		{
			e->SetDamage(700);
		}
		
		// �V�F�C�N����
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
