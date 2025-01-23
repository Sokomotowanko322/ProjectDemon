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

#pragma region 画像ファイル

	// タイトル画像
	res = std::make_unique<Resource>(Resource::TYPE::IMG, Application::PATH_IMAGE + "TitleImage.png");
	resourcesMap_.emplace(SRC::TITLE_IMAGE, std::move(res));
	res = std::make_unique<Resource>(Resource::TYPE::IMG, Application::PATH_IMAGE + "Start.png");
	resourcesMap_.emplace(SRC::START_LOGO, std::move(res));
	
	// リザルト画像
	res = std::make_unique<Resource>(Resource::TYPE::IMG, Application::PATH_IMAGE + "ResultImage.png");
	resourcesMap_.emplace(SRC::RESULT, std::move(res));

	// PushSpace
	res = std::make_unique<Resource>(Resource::TYPE::IMG, Application::PATH_IMAGE + "PushSpace.png");
	resourcesMap_.emplace(SRC::PUSH_SPACE, std::move(res));

	// プレイヤー影
	res = std::make_unique<Resource>(Resource::TYPE::IMG, Application::PATH_IMAGE + "Shadow.png");
	resourcesMap_.emplace(SRC::PLAYER_SHADOW, std::move(res));

	// トゥーン用マップテクスチャ
	res = std::make_unique<Resource>(Resource::TYPE::IMG, Application::PATH_MODEL + "Player/ToonMap.png");
	resourcesMap_.emplace(SRC::TEX_TOON, std::move(res));

#pragma endregion

#pragma region モデル

	// プレイヤー
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Player.mv1");
	resourcesMap_.emplace(SRC::PLAYER, std::move(res));

	// 武器
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Weapon/Katana.mv1");
	resourcesMap_.emplace(SRC::PLAYER_WEAPON, std::move(res));

	// スカイドーム
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "SkyDome/SkyDome.mv1");
	resourcesMap_.emplace(SRC::SKY_DOME, std::move(res));

	// ステージ
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage/Stage.mv1");
	resourcesMap_.emplace(SRC::STAGE, std::move(res));
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage/Colliders.mv1");
	resourcesMap_.emplace(SRC::STAGE_COLLIDER, std::move(res));

	// 敵モデル
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/NormalEnemy.mv1");
	resourcesMap_.emplace(SRC::NORMAL_ENEMY, std::move(res));
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/EnemyBoss.mv1");
	resourcesMap_.emplace(SRC::ENEMY_BOSS_TYPE1, std::move(res));
	
#pragma endregion

#pragma region プレイヤーのアニメーション
	
	// プレイヤー
	// 移動系
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Walk.mv1");
	resourcesMap_.emplace(SRC::PLAYER_WALK, std::move(res));
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Run.mv1");
	resourcesMap_.emplace(SRC::PLAYER_RUN, std::move(res));

	// 攻撃
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/ComboFirst.mv1");
	resourcesMap_.emplace(SRC::PLAYER_FIRSTCOMBO, std::move(res));
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/ComboSecond.mv1");
	resourcesMap_.emplace(SRC::PLAYER_SECONDCOMBO, std::move(res));
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/ComboThird.mv1");
	resourcesMap_.emplace(SRC::PLAYER_THIRDCOMBO, std::move(res));
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Counter.mv1");
	resourcesMap_.emplace(SRC::PLAYER_COUNTER, std::move(res));

	// 吸収動作
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Inhale.mv1");
	resourcesMap_.emplace(SRC::PLAYER_INHALE, std::move(res));

#pragma endregion

#pragma region 敵のアニメーション
	
	// 敵
	// 移動系
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/NormalEnemyAnim/Idle.mv1");
	resourcesMap_.emplace(SRC::NORMAL_ENEMY_IDLE, std::move(res));
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/NormalEnemyAnim/Walk.mv1");
	resourcesMap_.emplace(SRC::NORMAL_ENEMY_WALK, std::move(res));
	
	// 攻撃系
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/NormalEnemyAnim/NormalAttack.mv1");
	resourcesMap_.emplace(SRC::NORMAL_ENEMY_NORMALATTACK, std::move(res));

	// 死亡
	res = std::make_unique<Resource>(Resource::TYPE::MODEL, Application::PATH_MODEL + "Enemy/NormalEnemyAnim/Death.mv1");
	resourcesMap_.emplace(SRC::NORMAL_ENEMY_DEATH, std::move(res));

#pragma endregion

#pragma region エフェクト全般

	// 魂
	res = std::make_unique<Resource>(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "BlueSoul.efkefc");
	resourcesMap_.emplace(SRC::EFFECT_BLUESOUL, std::move(res));

	// 吸収エフェクト
	res = std::make_unique<Resource>(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Inhale.efkefc");
	resourcesMap_.emplace(SRC::EFFECT_INHALE, std::move(res));
	
	// ヒット時エフェクト
	res = std::make_unique<Resource>(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Slash_Hit.efkefc");
	resourcesMap_.emplace(SRC::EFFECT_HIT, std::move(res));
	
	// 剣に追従するエフェクト
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

	// ロード済みチェック
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	// リソース登録チェック
	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// 登録されていない
		return dummy_;
	}

	// ロード処理
	rPair->second->Load();

	// 念のためコピーコンストラクタ
	loadedMap_.emplace(src, *rPair->second);

	return *rPair->second;

}
