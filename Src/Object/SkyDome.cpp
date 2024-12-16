#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Utility/Utility.h"
#include "Common/Transform.h"
#include "SkyDome.h"

SkyDome::SkyDome(const Transform& syncTransform) : syncTransform_(syncTransform)
{
	state_ = MOVE_STATE::NONE;
	
	// ��ԊǗ�
	stateChanges_.emplace(MOVE_STATE::NONE, std::bind(&SkyDome::ChangeStateNone, this));
	stateChanges_.emplace(MOVE_STATE::STAY, std::bind(&SkyDome::ChangeStateStay, this));
	stateChanges_.emplace(MOVE_STATE::FOLLOW, std::bind(&SkyDome::ChangeStateFollow, this));
}

SkyDome::~SkyDome(void)
{
}

void SkyDome::Init(void)
{
	// ���f������̊�{���
	transform_.SetModel(
		resMng_.LoadModelDuplicate(
			ResourceManager::SRC::SKY_DOME));
	transform_.scl = SCALES;
	transform_.pos = Utility::VECTOR_ZERO;
	transform_.quaRot = Quaternion::Euler(
		0.0f, 
		Utility::Deg2RadF(180.0f),
		0.0f
	);
	transform_.quaRotLocal = Quaternion();
	transform_.Update();

	// Z�o�b�t�@����(�˂������΍�)
	MV1SetUseZBuffer(transform_.modelId, false);
	MV1SetWriteZBuffer(transform_.modelId, false);

	// ��ԑJ��
	auto sceneId = scnMng_.GetSceneID();
	if(sceneId == SceneManager::SCENE_ID::TITLE)
	{
		ChangeState(MOVE_STATE::STAY);
	}
	else
	{
		ChangeState(MOVE_STATE::FOLLOW);
	}
}

void SkyDome::Update(void)
{
	// �X�V�X�e�b�v
	stateUpdate_();
}

void SkyDome::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}

void SkyDome::ChangeState(MOVE_STATE state)
{

	// ��ԕύX
	state_ = state;

	// �e��ԑJ�ڂ̏�������
	stateChanges_[state_]();

}

void SkyDome::ChangeStateNone(void)
{
	stateUpdate_ = std::bind(&SkyDome::UpdateNone, this);
}

void SkyDome::ChangeStateStay(void)
{
	stateUpdate_ = std::bind(&SkyDome::UpdateStay, this);
}

void SkyDome::ChangeStateFollow(void)
{

	stateUpdate_ = std::bind(&SkyDome::UpdateFollow, this);

	transform_.pos = syncTransform_.pos;
	transform_.Update();

}

void SkyDome::UpdateNone(void)
{
}

void SkyDome::UpdateStay(void)
{
}

void SkyDome::UpdateFollow(void)
{
	transform_.pos = syncTransform_.pos;
	transform_.Update();
}
