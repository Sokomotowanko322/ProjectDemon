#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Object/Common/AnimationController.h"
#include "EnemyBase.h"

EnemyBase::EnemyBase(void)
	: resMng_(ResourceManager::GetInstance()),
	scnMng_(SceneManager::GetInstance())
{
}

EnemyBase::~EnemyBase(void)
{
}

const Transform& EnemyBase::GetEnemyTransform(void) const
{
	return enmyTransform_;
}

VECTOR EnemyBase::GetPos(void) const
{
	return enmyTransform_.pos;
}
