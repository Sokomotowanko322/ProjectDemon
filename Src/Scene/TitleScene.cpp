#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
#include "TitleScene.h"

const int START_LOGO_POS_X = 900;
const int START_LOGO_POS_Y = 1100;
const int BLINK_COUNT = 40;
const int BLINK_RATE = 2;

TitleScene::TitleScene(void) :
	resMng_(ResourceManager::GetInstance()),
	count_(BLINK_COUNT)
{
}

TitleScene::~TitleScene(void)
{
	DeleteGraph(titleImg_);
	DeleteGraph(startImg_);
}

void TitleScene::Init(void)
{
	// íËì_ÉJÉÅÉâ
	titleImg_ = resMng_.Load(ResourceManager::SRC::TITLE_IMAGE).handleId_;
	startImg_ = resMng_.Load(ResourceManager::SRC::START_LOGO).handleId_;
}

void TitleScene::Update(void)
{
	// ÉVÅ[ÉìëJà⁄
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTriggered(InputManager::ACTION::SELECT_ENTER))
	{
		count_ = BLINK_RATE;
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}

}

void TitleScene::Draw(void)
{
	// ÉçÉSÇ»Ç«ÇÃï`âÊ
	DrawRotaGraph(Application::SCREEN_SIZE_HALF_X, Application::SCREEN_SIZE_HALF_Y, 1.0f, 0.0f, titleImg_, true);
	blinkCnt_++;
	//ì_ñ≈
	if (!((blinkCnt_ / count_) % BLINK_RATE))
	{
		DrawRotaGraph(START_LOGO_POS_X, START_LOGO_POS_Y, 0.8f, 0.0f, startImg_, true);
	}
}
