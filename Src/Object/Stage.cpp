#include <vector>
#include <map>
#include <DxLib.h>
#include "../Utility/Utility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "Player.h"
#include "Planet.h"
#include "NormalEnemy.h"
#include "TestEnemy.h"
#include "Common/Collider.h"
#include "Common/Transform.h"
#include "Stage.h"

Stage::Stage(std::weak_ptr<Player> player)
	:
	resMng_(ResourceManager::GetInstance()),
	player_(player)
{
	activeName_ = NAME::STAGE;
	step_ = 0.0f;
}

Stage::~Stage(void)
{
	// 惑星
	planets_.clear();

	// 骸骨オブジェクト
	normalEnemy_.clear();
}

void Stage::Init(void)
{
	// メインとなるステージ生成
	MakeMainStage();

	// その他環境
	MakeEnvironment();

	step_ = -1.0f;

}

void Stage::Update(void)
{

	//// 惑星
	//for (const auto& s : planets_)
	//{
	//	s.second->Update();
	//}

	// 骸骨オブジェクト
	for (const auto& s : normalEnemy_)
	{
		s->Update();
	}

}

void Stage::Draw(void)
{

	// 惑星
	for (const auto& s : planets_)
	{
		s.second->Draw();
	}

	// 骸骨オブジェクト
	for (const auto& s : normalEnemy_)
	{
		s->Draw();
	}

}

void Stage::ChangeStage(NAME type)
{

	activeName_ = type;

	// 対象のステージを取得する
	activePlanet_ = GetPlanet(activeName_);

	// ステージの当たり判定をプレイヤーに設定
	player_->ClearCollider();
	player_->AddCollider(activePlanet_.lock()->GetTransform().collider);
	
	for(auto e : normalEnemy_)
	{
		e->ClearCollider();
		e->AddCollider(activePlanet_.lock()->GetEnemyTransform().collider);
	}

	step_ = TIME_STAGE_CHANGE;

}

std::weak_ptr<Planet> Stage::GetPlanet(NAME type)
{
	if (planets_.count(type) == 0)
	{
		return nullPlanet;
	}

	return planets_[type];
}

void Stage::MakeMainStage(void)
{

	// 最初の惑星
	//------------------------------------------------------------------------------
	Transform planetTrans;
	planetTrans.SetModel(
		resMng_.LoadModelDuplicate(ResourceManager::SRC::STAGE));
	Transform enemy;
	enemy.SetModel(
		resMng_.LoadModelDuplicate(ResourceManager::SRC::NORMAL_ENEMY));

	planetTrans.scl = { 250.0,200.0,250.0 };
	planetTrans.quaRot = Quaternion();
	planetTrans.pos = { 0.0f, -400.0f, 0.0f };

	// 当たり判定(コライダ)作成
	planetTrans.MakeCollider(Collider::TYPE::STAGE);
	enemy.MakeCollider(Collider::TYPE::STAGE);

	planetTrans.Update();

	NAME name = NAME::STAGE;
	std::shared_ptr<Planet> planet =
		std::make_shared<Planet>(
			name, Planet::TYPE::GROUND, planetTrans);
	planet->Init();
	planets_.emplace(name, std::move(planet));
	//------------------------------------------------------------------------------

}

void Stage::MakeEnvironment(void)
{

	auto normalEnemy = std::make_shared<TestEnemy>(player_);
	normalEnemy->Init();
	normalEnemy->SetPos({ -300.0f, -30.0f, 500.0f });
	normalEnemy_.emplace_back(std::move(normalEnemy));

	normalEnemy = std::make_shared<TestEnemy>(player_);
	normalEnemy->Init();
	normalEnemy->SetPos({ 250.0f, -60.0f, 900.0f });
	normalEnemy_.emplace_back(std::move(normalEnemy));

	normalEnemy = std::make_shared<TestEnemy>(player_);
	normalEnemy->Init();
	normalEnemy->SetPos({ -20.0f, -45.0f, 1600.0f });
	normalEnemy_.emplace_back(std::move(normalEnemy));

}
