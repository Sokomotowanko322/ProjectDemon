#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "ActorBase.h"

ActorBase::ActorBase(void)
	:
	resMng_(ResourceManager::GetInstance()),
	scnMng_(SceneManager::GetInstance()),
	colMng_(SceneManager::GetInstance().GetColManager())
{
}

ActorBase::~ActorBase(void)
{
}

const Transform& ActorBase::GetTransform(void) const
{
	return transform_;
}
