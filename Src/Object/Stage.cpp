#include <vector>
#include <map>
#include <DxLib.h>
#include "../Utility/Utility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "Player.h"
#include "NormalEnemy.h"
#include "TestEnemy.h"
#include "../Object/Common/Collider/Collider.h"
#include "../Object/Common/Collider/ColliderController.h"
#include "../Object/Common/Collider/ColliderManager.h"
#include "Common/Transform.h"
#include "Stage.h"

static constexpr VECTOR MODEL_SCALE = { 200.0f,200.0f,200.0f };

Stage::Stage(std::weak_ptr<Player> player)
	:
	resMng_(ResourceManager::GetInstance()),
	scnMng_(SceneManager::GetInstance()),
	collision_(SceneManager::GetInstance().GetCollision()),
	player_(player)
{
	auto& myCol = scnMng_.GetCollision();
}

Stage::~Stage(void)
{
	
}

void Stage::Init(void)
{
	// モデルの基本設定(コライダに使うため)
	stageTransform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::STAGE));
	stageTransform_.scl = MODEL_SCALE;
	stageTransform_.pos = { 0.0f, -400.0f, 0.0f };
	stageTransform_.quaRot = Quaternion();
	stageTransform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, Utility::Deg2RadF(0.0f), 0.0f });
	
	stageTransform_.Update();
	
	colliderTransform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::STAGE_COLLIDER));
	colliderTransform_.scl = MODEL_SCALE;
	colliderTransform_.pos = { -530.0f, -400.0f, 590.0f };
	colliderTransform_.quaRot = Quaternion();
	colliderTransform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, Utility::Deg2RadF(0.0f), 0.0f });
	
	colliderTransform_.Update();
}

void Stage::Update(void)
{
	// モデル情報更新
	stageTransform_.Update();
	colliderTransform_.Update();

	// ステージモデルに対するコライダの設定
	scnMng_.GetColManager().AddCollider(
		OBJECT_TYPE::STAGE,
		COL_TYPE::MODEL,
		stageTransform_,
		true
	);

}

void Stage::Draw(void)
{
	MV1DrawModel(stageTransform_.modelId);
	//MV1DrawModel(colliderTransform_.modelId);
	scnMng_.GetColManager().DrawCollider(OBJECT_TYPE::PLAYER);
	//scnMng_.GetColManager().DrawCollider(OBJECT_TYPE::ENEMY_MODEL);
	scnMng_.GetColManager().DrawCollider(OBJECT_TYPE::WEAPON);
	scnMng_.GetColManager().DrawCollider(OBJECT_TYPE::PLAYER_FOOT);
}

int Stage::GetModelId(void)
{
	return stageTransform_.modelId;
}

void Stage::SetCollisionStage(void)
{

}
