#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
#include "TitleScene.h"
#include "ResultScene.h"

ResultScene::ResultScene(void)
{
}

ResultScene::~ResultScene(void)
{
}

void ResultScene::Init(void)
{
	// íËì_ÉJÉÅÉâ
	mainCamera.ChangeMode(Camera::MODE::FIXED_POINT);
	resultImg_ = resMng_.Load(ResourceManager::SRC::RESULT).handleId_;
}

void ResultScene::Update(void)
{
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTriggered(InputManager::ACTION::SCENE_CHANGE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE_IMAGE);
	}
}

void ResultScene::Draw(void)
{
	DrawGraph(0, 0, resultImg_, true);
}
