#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
#include "TitleScene.h"

TitleScene::TitleScene(void) :
	resMng_(ResourceManager::GetInstance())
{
}

TitleScene::~TitleScene(void)
{
	DeleteGraph(titleLogo_);
}

void TitleScene::Init(void)
{
	// ��_�J����
	titleLogo_ = resMng_.Load(ResourceManager::SRC::TITLE_LOGO).handleId_;
}

void TitleScene::Update(void)
{

	// �V�[���J��
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTriggered(InputManager::ACTION::SCENE_CHANGE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}

}

void TitleScene::Draw(void)
{
	// ���S�Ȃǂ̕`��
	DrawRotaGraph(Application::SCREEN_SIZE_HALF_X, 230, 3.0, 0.0f, titleLogo_, true);

}
