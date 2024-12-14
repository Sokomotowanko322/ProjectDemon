#include <EffekseerForDXLib.h>
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Utility/Utility.h"
#include "../../Manager/InputManager.h"
#include "../../Common/Quaternion.h"
#include "../../Application.h"
#include "Weapon.h"

// ���f���A�G�t�F�N�g�̑傫��
const VECTOR MODEL_SCALE = { 2.0f,3.3f,2.0f };
const float EFFECT_SCALE = 10.0f;

// �G�t�F�N�g���t�F�[�h������
const int FADE_SPEED = 4;
const int ALPHA_MAX = 255;

Weapon::Weapon(void)
	:
	effectPos_{ Utility::VECTOR_ZERO },
	handLPos_{ 0.0f, 10.0f, -2.0f },
	backLPos_{ -15.0f, 100.0f, 20.0f },
	SwordLength_{ 0.0f, 180.0f, 0.0f },
	handLocalquarot_{ Quaternion::Euler(Utility::Deg2RadF(-90.0f), Utility::Deg2RadF(18.0f), Utility::Deg2RadF(-98.0)) }, 
	//backLocalquarot_{ Quaternion::Euler(Utility::Deg2RadF(180.0f), Utility::Deg2RadF(0.0f), Utility::Deg2RadF(-10.0f)) },
	OldCol_{ false },
	isFire_{ false },
	isDrawed_{ false },
	isHit_{ false },
	isCharge_{ false },
	isChargeOld_{ false },
	isRainbow_{ false },
	fading_{ false },
	handAttatchFrame_{ 35 },
	backattatchfram_{ 4 },
	collisionSize_{ 30 },
	isEffect_{ static_cast<int>(EFFECT::NONE) },
	lineAlpha_{ ALPHA_MAX },
	colortimeCnt_{ 0 }
{
	// ���탂�f���ݒ�
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::PLAYER_WEAPON));

	transform_.scl = MODEL_SCALE;
	transform_.pos = Utility::VECTOR_ZERO;
	transform_.localPos = handLPos_;
	transform_.quaRotLocal = handLocalquarot_;
	transform_.quaRot = {};
	transform_.Update();
	
	StopEffekseer3DEffect(effectLightPlayId_);
	StopEffekseer3DEffect(effectLinePlayId_);
	StopEffekseer3DEffect(effectSlashPlayId_);

	InitEffect();

	SetEffectAfterFade();
}

Weapon::~Weapon(void)
{
}

void Weapon::Init(void)
{

}

void Weapon::Update(void)
{
}

void Weapon::WeaponUpdate(Transform follow, bool isAtkcol, bool isDrawed)
{
	// �C���X�^���X����
	auto& ins = InputManager::GetInstance();

	// �`�悳��Ă��邩
	isDrawed_ = isDrawed;

	// �U�����Ă��邩
	isAtkOld_ = isAtk_;
	isAtk_ = isAtkcol;

	// ��]
	MATRIX mat;
	
	// ����̈ʒu����	
	mat = MV1GetFrameLocalWorldMatrix(follow.modelId, handAttatchFrame_);
	transform_.localPos = handLPos_;
	transform_.quaRotLocal = handLocalquarot_;
	auto rot = MGetRotElem(mat);
	effectPos_ = MGetTranslateElem(mat);

	//	�s�񂩂�N�I�[�^�j�I��
	Quaternion qua = Quaternion::GetRotation(rot);
	Quaternion followRot = qua;

	// ���̈ʒu
	VECTOR swordPos = followRot.PosAxis(transform_.localPos);
	swordTopPos_ = followRot.PosAxis(SwordLength_);
	swordTopPos_ = VAdd(effectPos_, swordTopPos_);
	transform_.pos = VAdd(effectPos_, swordPos);

	// ��]
	transform_.quaRot = qua;
	transform_.Update();

	// EF
	PlayEffect();
	SyncEffect();

	// �t�F�[�h
	if (fading_)
	{
		Fade();
	}

	colortimeCnt_++;
}

void Weapon::Draw(void)
{
	MV1DrawModel(transform_.modelId);
	DrawFormatString(0, 60, GetColor(255, 255, 255), "weaponPosition: (%0.2f,%0.2f,%0.2f)", transform_.pos.x, transform_.pos.y, transform_.pos.z);// ���_�F�J�������W


}

void Weapon::DrawDebug(void)
{
}

const int Weapon::GetIsEffect(void) const
{
	return isEffect_;
}

const VECTOR Weapon::GetTopPos(void) const
{
	return swordTopPos_;
}

void Weapon::SetEffect(int effect)
{
	//�G�t�F�N�g�t�F�C�h�A�E�g
	fading_ = true;
	
	//�G�t�F�N�g��ސݒ�
	isEffect_ = effect;
}

void Weapon::Fade(void)
{
	if (lineAlpha_ > FADE_SPEED)
	{
		lineAlpha_ -= FADE_SPEED;
	}
	else
	{
		SetEffectAfterFade();
		fading_ = false;
		lineAlpha_ = ALPHA_MAX;
	}
	SyncEffect();
}

void Weapon::SetIsHit(bool ishit)
{
}

void Weapon::InitEffect(void)
{
}

void Weapon::SyncEffect(void)
{
}

void Weapon::PlayEffect(void)
{
}

void Weapon::SetEffectAfterFade(void)
{
}