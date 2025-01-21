#include <EffekseerForDXLib.h>
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Utility/Utility.h"
#include "../../Manager/InputManager.h"
#include "../../Common/Quaternion.h"
#include "../../Application.h"
#include "Weapon.h"

// モデル、エフェクトの大きさ
const VECTOR MODEL_SCALE = { 2.0f,3.3f,2.0f };
const float EFFECT_SCALE = 10.0f;

// エフェクトをフェードさせる
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
	// 武器モデル設定
	weaponTransform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::PLAYER_WEAPON));

	weaponTransform_.scl = MODEL_SCALE;
	weaponTransform_.pos = Utility::VECTOR_ZERO;
	weaponTransform_.localPos = handLPos_;
	weaponTransform_.quaRotLocal = handLocalquarot_;
	weaponTransform_.quaRot = {};
	weaponTransform_.Update();
	
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
	weaponTransform_.Update();
}

void Weapon::Update(void)
{
}

void Weapon::WeaponUpdate(Transform follow, bool isAtkcol, bool isDrawed)
{

	// インスタンス生成
	auto& ins = InputManager::GetInstance();

	// 描画されているか
	isDrawed_ = isDrawed;

	// 攻撃しているか
	isAtkOld_ = isAtk_;
	isAtk_ = isAtkcol;

	// 回転
	MATRIX mat;
	
	// 武器の位置同期	
	mat = MV1GetFrameLocalWorldMatrix(follow.modelId, handAttatchFrame_);
	weaponTransform_.localPos = handLPos_;
	weaponTransform_.quaRotLocal = handLocalquarot_;
	auto rot = MGetRotElem(mat);
	effectPos_ = MGetTranslateElem(mat);

	//	行列からクオータニオン
	Quaternion qua = Quaternion::GetRotation(rot);
	Quaternion followRot = qua;

	// 剣の位置
	VECTOR swordPos = followRot.PosAxis(weaponTransform_.localPos);
	swordTopPos_ = followRot.PosAxis(SwordLength_);
	swordTopPos_ = VAdd(effectPos_, swordTopPos_);
	weaponTransform_.pos = VAdd(effectPos_, swordPos);

	// 回転
	weaponTransform_.quaRot = qua;

	transform_.pos = weaponTransform_.pos;


	// 更新
	transform_.Update();
	weaponTransform_.Update();

	// EF
	PlayEffect();
	SyncEffect();

	// フェード
	if (fading_)
	{
		Fade();
	}

	colortimeCnt_++;
}

void Weapon::Draw(void)
{
	MV1DrawModel(weaponTransform_.modelId);
	DrawFormatString(0, 60, GetColor(255, 255, 255),
		"weaponPosition: (%0.2f,%0.2f,%0.2f)", weaponTransform_.pos.x, weaponTransform_.pos.y, weaponTransform_.pos.z);// 頂点：カメラ座標
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

const VECTOR Weapon::GetPos(void) const
{
	return weaponTransform_.pos;
}

void Weapon::SetEffect(int effect)
{
	//エフェクトフェイドアウト
	fading_ = true;
	
	//エフェクト種類設定
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