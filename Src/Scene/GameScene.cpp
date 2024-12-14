#include <DxLib.h>
#include "../Utility/Utility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Manager/InputManager.h"
#include "../Object/Common/Capsule.h"
#include "../Object/Common/Collider.h"
#include "../Object/SkyDome.h"
#include "../Object/Stage.h"
#include "../Object/Soul.h"
#include "../Object/NormalEnemy.h"
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
}

void GameScene::Init(void)
{
	// �v���C���[
	player_ = std::make_shared<Player>();
	player_->Init();
	
	// �X�e�[�W
	stage_ = std::make_unique<Stage>(*player_);
	stage_->Init();
	
	soul_ = std::make_shared<Soul>(player_);
	soul_->Init();

	// �G
	normalEnemy_ = std::make_unique<NormalEnemy>();
	normalEnemy_->Init();

	
	// �X�e�[�W�̏����ݒ�
	stage_->ChangeStage(Stage::NAME::STAGE);

	// �X�J�C�h�[��
	skyDome_ = std::make_unique<SkyDome>(player_->GetTransform());
	skyDome_->Init();

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
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	normalEnemy_->Update();
	skyDome_->Update();

	stage_->Update();

	player_->Update();
	soul_->Update();

}

void GameScene::Draw(void)
{
	// �w�i
	skyDome_->Draw();
	normalEnemy_->Draw();
	
	soul_->Draw();
	stage_->Draw();
	
	player_->Draw();

	// �w���v
	DrawFormatString(840, 20, 0x000000, "�ړ��@�@�FWASD");
	DrawFormatString(840, 40, 0x000000, "�J�����@�F���L�[");
	DrawFormatString(840, 60, 0x000000, "�_�b�V���F�EShift");
	DrawFormatString(840, 80, 0x000000, "�W�����v�F�_(�o�N�X��)");
}