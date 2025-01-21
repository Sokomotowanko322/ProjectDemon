#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "SceneBase.h"

SceneBase::SceneBase(void) : 
	resMng_(ResourceManager::GetInstance()),
	camera_(SceneManager::GetInstance().GetCamera()),
	collision_(SceneManager::GetInstance().GetCollision())
{
}

SceneBase::~SceneBase()
{
}

void SceneBase::Init(void)
{
}

void SceneBase::Update(void)
{
}

void SceneBase::Draw(void)
{
}
