
#pragma once
#include<vector>
#include "../Common/Transform.h"
#include "../ActorBase.h"


enum class EFFECT
{
	FIRE,
	STORM,
	LIGHTNING,
	NONE,
};


class Weapon : public ActorBase
{
public:

	// �R���X�g���N�^
	Weapon(void);

	// �f�X�g���N�^
	~Weapon(void);

	void Init(void) override;

	void Update(void) override;

	void WeaponUpdate(Transform Follow, bool isAtkcol, bool isDrawed);

	void Draw(void)override;
	void DrawDebug(void);

	const int GetIsEffect(void) const;

	const VECTOR GetTopPos(void) const;
	const VECTOR GetPos(void) const;

	void SetEffect(int effect);

	void Fade(void);

	void SetIsHit(bool ishit);

protected:

	Transform weaponTransform_;

	VECTOR pos_;

	VECTOR targetPos_;

	// ���@�̏����
	VECTOR cameraUp_;

	// ���@�̉�]
	Quaternion rot_;

	// �G�t�F�N�g�̍��W
	VECTOR effectPos_;

	// �G�t�F�N�g������
	void InitEffect(void);

	// �G�t�F�N�g����
	void SyncEffect(void);

	void PlayEffect(void);

	void SetEffectAfterFade(void);

	int effectSlashPlayId_;

	int effectSlashResId_;

	int effectLightPlayId_;

	int effectLightResId_;

	//���̋O��
	int effectLinePlayId_;

	int effectNormalLineResId_;
	int effectFireLineResId_;
	int effectStormLineResId_;
	int effectLightningLineResId_;

	int SoundIDFire_;
	int SoundIDLightning_;
	int SoundIDStorm_;

	int handAttatchFrame_;
	Quaternion handLocalquarot_;
	VECTOR handLPos_;

	int backattatchfram_;
	Quaternion backLocalquarot_;
	VECTOR backLPos_;

	float effectScale_;

	float collisionSize_;
	bool isFire_;

	VECTOR SwordLength_;

	VECTOR swordTopPos_;

	bool OldCol_;
	bool isDrawed_;
	bool isHit_;
	bool isCharge_;
	bool isChargeOld_;
	bool isAtk_;
	bool isAtkOld_;

	int isEffect_;

	int lineAlpha_;

	bool fading_;
	bool isRainbow_;

	int colortimeCnt_;
};