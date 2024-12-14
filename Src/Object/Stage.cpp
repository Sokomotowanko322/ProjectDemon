#include <vector>
#include <map>
#include <DxLib.h>
#include "../Utility/Utility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "Player.h"
#include "Planet.h"
#include "NormalEnemy.h"
#include "Common/Collider.h"
#include "Common/Transform.h"
#include "Stage.h"

Stage::Stage(Player& player)
	:
	resMng_(ResourceManager::GetInstance()),
	player_(player)
{
	activeName_ = NAME::STAGE;
	step_ = 0.0f;
}

Stage::~Stage(void)
{
	// �f��
	planets_.clear();

	// �[���I�u�W�F�N�g
	normalEnemy_.clear();
}

void Stage::Init(void)
{
	// ���C���ƂȂ�X�e�[�W����
	MakeMainStage();
	
	// ���̑���
	MakeEnvironment();
	
	step_ = -1.0f;

}

void Stage::Update(void)
{

	//// �f��
	//for (const auto& s : planets_)
	//{
	//	s.second->Update();
	//}

	//// �[���I�u�W�F�N�g
	//for (const auto& s : skulls_)
	//{
	//	s->Update();
	//}

}

void Stage::Draw(void)
{

	// �f��
	for (const auto& s : planets_)
	{
		s.second->Draw();
	}

	//// �[���I�u�W�F�N�g
	//for (const auto& s : skulls_)
	//{
	//	s->Draw();
	//}

}

void Stage::ChangeStage(NAME type)
{

	activeName_ = type;

	// �Ώۂ̃X�e�[�W���擾����
	activePlanet_ = GetPlanet(activeName_);

	// �X�e�[�W�̓����蔻����v���C���[�ɐݒ�
	player_.ClearCollider();
	player_.AddCollider(activePlanet_.lock()->GetTransform().collider);

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

	// �ŏ��̘f��
	//------------------------------------------------------------------------------
	Transform planetTrans;
	planetTrans.SetModel(
		resMng_.LoadModelDuplicate(ResourceManager::SRC::STAGE));
	planetTrans.scl = {250.0,200.0,250.0};
	planetTrans.quaRot = Quaternion();
	planetTrans.pos = { 0.0f, -400.0f, 0.0f };

	// �����蔻��(�R���C�_)�쐬
	planetTrans.MakeCollider(Collider::TYPE::STAGE);

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

	auto normalEnemy = std::make_shared<NormalEnemy>();
	normalEnemy->Init();
	normalEnemy->SetPos({ -300.0f, -30.0f, 500.0f });
	normalEnemy_.emplace_back(std::move(normalEnemy));

	normalEnemy = std::make_shared<NormalEnemy>();
	normalEnemy->Init();
	normalEnemy->SetPos({ 250.0f, -60.0f, 900.0f });
	normalEnemy_.emplace_back(std::move(normalEnemy));

	normalEnemy = std::make_shared<NormalEnemy>();
	normalEnemy->Init();
	normalEnemy->SetPos({ -20.0f, -45.0f, 1600.0f });
	normalEnemy_.emplace_back(std::move(normalEnemy));

}
