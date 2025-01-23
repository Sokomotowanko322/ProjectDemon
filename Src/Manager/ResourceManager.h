#pragma once
#include <memory>
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	// ���\�[�X��
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

	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static ResourceManager& GetInstance(void);

	// ������
	void Init(void);

	// ���(�V�[���ؑ֎��Ɉ�U���)
	void Release(void);

	// ���\�[�X�̊��S�j��
	void Destroy(void);

	// ���\�[�X�̃��[�h
	const Resource& Load(SRC src);

	// ���\�[�X�̕������[�h(���f���p)
	int LoadModelDuplicate(SRC src);

private:

	// �ÓI�C���X�^���X
	static ResourceManager* instance_;

	// ���\�[�X�Ǘ��̑Ώ�
	std::map<SRC, std::unique_ptr<Resource>> resourcesMap_;

	// �ǂݍ��ݍς݃��\�[�X
	std::map<SRC, Resource&> loadedMap_;

	Resource dummy_;

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	ResourceManager(void);
	ResourceManager(const ResourceManager& manager) = default;
	~ResourceManager(void) = default;

	// �������[�h
	Resource& _Load(SRC src);

};
