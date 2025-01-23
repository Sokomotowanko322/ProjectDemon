#include <DxLib.h>
#include "../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{

	std::unique_ptr<Resource> res;

#pragma region �摜�t�@�C��

	// �^�C�g���摜
	res = std::make_unique<Resource>(Resource::TYPE::IMG, Application::PATH_IMAGE + "TitleImage.png");
	resourcesMap_.emplace(SRC::TITLE_IMAGE, std::move(res));
	res = std::make_unique<Resource>(Resource::TYPE::IMG, Application::PATH_IMAGE + "Start.png");
	resourcesMap_.emplace(SRC::START_LOGO, std::move(res));
	
	// ���U���g�摜
	res = std::make_unique<Resource>(Resource::TYPE::IMG, Application::PATH_IMAGE + "ResultImage.png");
	resourcesMap_.emplace(SRC::RESULT, std::move(res));

	// PushSpace
	res = std::make_unique<Resource>(Resource::TYPE::IMG, Application::PATH_IMAGE + "PushSpace.png");
	resourcesMap_.emplace(SRC::PUSH_SPACE, std::move(res));

	// �v���C���[�e
	res = std::make_unique<Resource>(Resource::TYPE::IMG, Application::PATH_IMAGE + "Shadow.png");
	resourcesMap_.emplace(SRC::PLAYER_SHADOW, std::move(res));

	// �g�D�[���p�}�b�v�e�N�X�`��
	res = std::make_unique<Resource>(Resource::TYPE::IMG, Application::PATH_MODEL + "Player/ToonMap.png");
	resourcesMap_.emplace(SRC::TEX_TOON, std::move(res));

#pragma endregion

#pragma region ���f��

	// �v���C���[
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Player.mv1");
	resourcesMap_.emplace(SRC::PLAYER, std::move(res));

	// ����
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Weapon/Katana.mv1");
	resourcesMap_.emplace(SRC::PLAYER_WEAPON, std::move(res));

	// �X�J�C�h�[��
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "SkyDome/SkyDome.mv1");
	resourcesMap_.emplace(SRC::SKY_DOME, std::move(res));

	// �X�e�[�W
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage/Stage.mv1");
	resourcesMap_.emplace(SRC::STAGE, std::move(res));
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage/Colliders.mv1");
	resourcesMap_.emplace(SRC::STAGE_COLLIDER, std::move(res));

	// �G���f��
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/NormalEnemy.mv1");
	resourcesMap_.emplace(SRC::NORMAL_ENEMY, std::move(res));
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/EnemyBoss.mv1");
	resourcesMap_.emplace(SRC::ENEMY_BOSS_TYPE1, std::move(res));
	
#pragma endregion

#pragma region �v���C���[�̃A�j���[�V����
	
	// �v���C���[
	// �ړ��n
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Walk.mv1");
	resourcesMap_.emplace(SRC::PLAYER_WALK, std::move(res));
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Run.mv1");
	resourcesMap_.emplace(SRC::PLAYER_RUN, std::move(res));

	// �U��
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/ComboFirst.mv1");
	resourcesMap_.emplace(SRC::PLAYER_FIRSTCOMBO, std::move(res));
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/ComboSecond.mv1");
	resourcesMap_.emplace(SRC::PLAYER_SECONDCOMBO, std::move(res));
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/ComboThird.mv1");
	resourcesMap_.emplace(SRC::PLAYER_THIRDCOMBO, std::move(res));
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Counter.mv1");
	resourcesMap_.emplace(SRC::PLAYER_COUNTER, std::move(res));

	// �z������
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Inhale.mv1");
	resourcesMap_.emplace(SRC::PLAYER_INHALE, std::move(res));

#pragma endregion

#pragma region �G�̃A�j���[�V����
	
	// �G
	// �ړ��n
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/NormalEnemyAnim/Idle.mv1");
	resourcesMap_.emplace(SRC::NORMAL_ENEMY_IDLE, std::move(res));
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/NormalEnemyAnim/Walk.mv1");
	resourcesMap_.emplace(SRC::NORMAL_ENEMY_WALK, std::move(res));
	
	// �U���n
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/NormalEnemyAnim/NormalAttack.mv1");
	resourcesMap_.emplace(SRC::NORMAL_ENEMY_NORMALATTACK, std::move(res));

	// ���S
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/NormalEnemyAnim/Death.mv1");
	resourcesMap_.emplace(SRC::NORMAL_ENEMY_DEATH, std::move(res));

#pragma endregion

#pragma region �G�t�F�N�g�S��

	// ��
	res = std::make_unique<Resource>(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "BlueSoul.efkefc");
	resourcesMap_.emplace(SRC::EFFECT_BLUESOUL, std::move(res));

	// �z���G�t�F�N�g
	res = std::make_unique<Resource>(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Inhale.efkefc");
	resourcesMap_.emplace(SRC::EFFECT_INHALE, std::move(res));
	
	// �q�b�g���G�t�F�N�g
	res = std::make_unique<Resource>(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Slash_Hit.efkefc");
	resourcesMap_.emplace(SRC::EFFECT_HIT, std::move(res));
	
	// ���ɒǏ]����G�t�F�N�g
	res = std::make_unique<Resource>(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "SwordLine.efkproj");
	resourcesMap_.emplace(SRC::PLAYER_WEAPON_SWORDLINE, std::move(res));

#pragma endregion

}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second.Release();
	}

	loadedMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	resourcesMap_.clear();
	delete instance_;
}

const Resource& ResourceManager::Load(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return dummy_;
	}
	return res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res.handleId_);
	res.duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

Resource& ResourceManager::_Load(SRC src)
{

	// ���[�h�ς݃`�F�b�N
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	// ���\�[�X�o�^�`�F�b�N
	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// �o�^����Ă��Ȃ�
		return dummy_;
	}

	// ���[�h����
	rPair->second->Load();

	// �O�̂��߃R�s�[�R���X�g���N�^
	loadedMap_.emplace(src, *rPair->second);

	return *rPair->second;

}
