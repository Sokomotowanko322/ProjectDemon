#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Object/Common/Collider/ColliderManager.h"
#include "../Object/Common/AnimationController.h"
#include "EnemyBase.h"

EnemyBase::EnemyBase(void)
	: resMng_(ResourceManager::GetInstance()), 
	colMng_(SceneManager::GetInstance().GetColManager()),
	scnMng_(SceneManager::GetInstance())
{
}

EnemyBase::~EnemyBase(void)
{
}

const Transform& EnemyBase::GetEnemyTransform(void) const
{
	return enemyTransform_;
}

VECTOR EnemyBase::GetPos(void) const
{
	return enemyTransform_.pos;
}
