#include <EffekseerForDXLib.h>
#include "../Utility/Utility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Resource.h"
#include "Common/Transform.h"
#include "Player.h"
#include "TestEnemy.h"
#include "NormalEnemy.h"
#include "Soul.h"

const VECTOR SOUL_SCALE = { 10.0f,10.0f,10.0f };
const float SOUL_STOP_POS = 1.0f;
const float SOUL_CENTER_POS = 200.0f;
const float SOUL_MOVE_SPEED = 30.0f;
const float SOUL_INHALE_SPEED = 10.0f;
const float SOUL_ROT_SPEED = 10.0f;
const float DEVIDE_STEPCOUNT = 0.5f;
const float EFFECT_PLAYSPEED = 0.001f;

Soul::Soul(std::weak_ptr<Player> player)
{
	player_ = player;

	// �G�̍��W�����Ƃ邽�ߐ���
	normalEnemy_ = std::make_shared<TestEnemy>(player_);

	// ���V���x�Ȃǂ̏�����
	soulMoveCnt_ = 0.0f;
	rotationStep_ = 0.0f;
	effectRot_ = 0.0f;
	moveSpeed_ = SOUL_MOVE_SPEED;

	// �G�t�F�N�g������
	effectSoulResId_[SOUL_TYPE::BLUE] = (ResourceManager::GetInstance().Load(ResourceManager::SRC::EFFECT_BLUESOUL).handleId_);
	SetSpeedPlayingEffekseer3DEffect(effectSoulResId_[SOUL_TYPE::BLUE], EFFECT_PLAYSPEED);
}

Soul::~Soul(void)
{
	StopEffect();
	DeleteEffekseerEffect(effectSoulResId_[SOUL_TYPE::BLUE]);
}

void Soul::Init(void)
{	
	// �v���C���[�̍��W���擾
	playerPos_ = player_.lock()->GetLeftHandPos();

	// ���ꂼ��̈ʒu���擾
	followSoulPos_[SOUL_TYPE::BLUE] = normalEnemy_->GetPos();
}

void Soul::Update(void)
{
	playerPos_ = player_.lock()->GetLeftHandPos();
	SetScalePlayingEffekseer3DEffect(effectSoulPlayId_[SOUL_TYPE::BLUE], SOUL_SCALE.x, SOUL_SCALE.y, SOUL_SCALE.z);	
	
	if (player_.lock()->GetNowAnim() == Player::ANIM_TYPE::INHALE)
	{
		UpdateInhale();
	}
	else
	{
		UpdateFloating();
	}

}
int cnt = 0;
void Soul::Draw(void)
{

	DrawFormatString(0, 20, GetColor(255, 255, 255), "soulPosition: (%0.2f,%0.2f,%0.2f)",
		followSoulPos_[SOUL_TYPE::BLUE].x, followSoulPos_[SOUL_TYPE::BLUE].y, followSoulPos_[SOUL_TYPE::BLUE].z);
		cnt++;
	if (cnt <= 1)
	{
		effectSoulPlayId_[SOUL_TYPE::BLUE] = PlayEffekseer3DEffect(effectSoulResId_[SOUL_TYPE::BLUE]);
	}
}

void Soul::StopEffect(void)
{
	StopEffekseer3DEffect(effectSoulPlayId_[SOUL_TYPE::BLUE]);
}

void Soul::UpdateInhale(void)
{
	
	const float deltaTime_ = SceneManager::GetInstance().GetDeltaTime();
	soulMoveCnt_ += deltaTime_;
	
	// �G�l�~�[����v���C���[�܂ł̃x�N�g�����v�Z
	VECTOR direction = VSub(playerPos_, followSoulPos_[SOUL_TYPE::BLUE]);
	// ��]���v�Z
	Quaternion rotation = Quaternion::LookRotation(direction);

	// ���K��
	direction = VNorm(direction);

	// �����̓����v�Z
	float distanceSquared = direction.x * direction.x + direction.y * direction.y + direction.z * direction.z;
	
	// ��Ƃ̋�������������΃G�t�F�N�g���I�t�ɂ���
	if (VSize(VSub(followSoulPos_[SOUL_TYPE::BLUE], playerPos_)) < SOUL_STOP_POS)
	{
		StopEffect();
	}

	// �ړ��ʂ��v�Z
	VECTOR movePow;
	movePow.x = direction.x * moveSpeed_ * deltaTime_;
	movePow.y = direction.y * moveSpeed_ * deltaTime_;
	movePow.z = direction.z * moveSpeed_ * deltaTime_;

	// �����ړ�
	followSoulPos_[SOUL_TYPE::BLUE].x += movePow.x * SOUL_INHALE_SPEED;
	followSoulPos_[SOUL_TYPE::BLUE].y += movePow.y * SOUL_INHALE_SPEED;
	followSoulPos_[SOUL_TYPE::BLUE].z += movePow.z * SOUL_INHALE_SPEED;
	
	
	// ��]��ݒ�
	SetRotationPlayingEffekseer3DEffect(effectSoulPlayId_[SOUL_TYPE::BLUE], rotation.x, rotation.y, rotation.z);

	// �ʒu��ݒ�
	SetPosPlayingEffekseer3DEffect(
		effectSoulPlayId_[SOUL_TYPE::BLUE],
		followSoulPos_[SOUL_TYPE::BLUE].x,
		followSoulPos_[SOUL_TYPE::BLUE].y,
		followSoulPos_[SOUL_TYPE::BLUE].z);
}

void Soul::UpdateFloating(void)
{
	const float deltaTime_ = SceneManager::GetInstance().GetDeltaTime();
	soulMoveCnt_ += deltaTime_;

	// �O�p�֐����g�p���ĕ��V������
	preFollowSoulPos_[SOUL_TYPE::BLUE] = followSoulPos_[SOUL_TYPE::BLUE];
	followSoulPos_[SOUL_TYPE::BLUE].x += sinf(soulMoveCnt_ * 0.7f) * moveSpeed_ * deltaTime_;
	followSoulPos_[SOUL_TYPE::BLUE].y += cosf(soulMoveCnt_ * 1.0f) * moveSpeed_ * deltaTime_;
	followSoulPos_[SOUL_TYPE::BLUE].z += sinf(soulMoveCnt_ * 0.7f) * moveSpeed_ * deltaTime_;

	// �����������邽�߂Ɍ��̍��W���Ƃ�
	VECTOR movePow = VSub(followSoulPos_[SOUL_TYPE::BLUE], preFollowSoulPos_[SOUL_TYPE::BLUE]);
	VECTOR dir = VNorm(movePow);
	Quaternion moveRot = Quaternion::LookRotation(dir);
	VECTOR m = moveRot.ToEuler();

	// ���Ԍo�߂ŉ�]
	effectRot_ += deltaTime_ * SOUL_ROT_SPEED;

	SetRotationPlayingEffekseer3DEffect(effectSoulPlayId_[SOUL_TYPE::BLUE], m.x, m.y, m.z);
	SetPosPlayingEffekseer3DEffect(effectSoulPlayId_[SOUL_TYPE::BLUE], followSoulPos_[SOUL_TYPE::BLUE].x,
		followSoulPos_[SOUL_TYPE::BLUE].y, followSoulPos_[SOUL_TYPE::BLUE].z);
}