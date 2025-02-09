#pragma once
#include <memory>
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	// リソース名
	enum class SRC
	{
		TITLE_IMAGE,
		START_LOGO,
		RESULT,
		PUSH_SPACE,
		SPEECH_BALLOON,
		PLAYER,
		PLAYER_WALK,
		PLAYER_RUN,
		PLAYER_FIRSTCOMBO,
		PLAYER_SECONDCOMBO,
		PLAYER_THIRDCOMBO,
		PLAYER_COUNTER,
		PLAYER_INHALE,
		PLAYER_SHADOW,
		PLAYER_WEAPON,
		PLAYER_WEAPON_SWORDLINE,
		SKY_DOME,
		STAGE,
		STAGE_COLLIDER,
		FALL_PLANET,
		FLAT_PLANET_01,
		FLAT_PLANET_02,
		LAST_PLANET,
		SPECIAL_PLANET,
		FOOT_SMOKE,
		WARP_STAR,
		WARP_STAR_ROT_EFF,
		WARP_ORBIT,
		BLACK_HOLE,
		GOAL_STAR,
		CLEAR,
		TEX_TOON,
		CENTAUR,
		NORMAL_ENEMY,
		NORMAL_ENEMY_IDLE,
		NORMAL_ENEMY_WALK,
		NORMAL_ENEMY_NORMALATTACK,
		NORMAL_ENEMY_DEATH,
		ENEMY_BOSS_TYPE1,
		MUSCOMORPH,
		EFFECT_BLUESOUL,
		EFFECT_INHALE,
		EFFECT_HIT,
	};

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ResourceManager& GetInstance(void);

	// 初期化
	void Init(void);

	// 解放(シーン切替時に一旦解放)
	void Release(void);

	// リソースの完全破棄
	void Destroy(void);

	// リソースのロード
	const Resource& Load(SRC src);

	// リソースの複製ロード(モデル用)
	int LoadModelDuplicate(SRC src);

private:

	// 静的インスタンス
	static ResourceManager* instance_;

	// リソース管理の対象
	std::map<SRC, std::unique_ptr<Resource>> resourcesMap_;

	// 読み込み済みリソース
	std::map<SRC, Resource&> loadedMap_;

	Resource dummy_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager(void);
	ResourceManager(const ResourceManager& manager) = default;
	~ResourceManager(void) = default;

	// 内部ロード
	Resource& _Load(SRC src);

};
