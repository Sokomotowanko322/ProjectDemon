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

	// 敵の座標情報をとるため生成
	normalEnemy_ = std::make_shared<TestEnemy>(player_);

	// 浮遊速度などの初期化
	soulMoveCnt_ = 0.0f;
	rotationStep_ = 0.0f;
	effectRot_ = 0.0f;
	moveSpeed_ = SOUL_MOVE_SPEED;

	// エフェクト初期化
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
	// プレイヤーの座標を取得
	playerPos_ = player_.lock()->GetLeftHandPos();

	// それぞれの位置を取得
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
	
	// エネミーからプレイヤーまでのベクトルを計算
	VECTOR direction = VSub(playerPos_, followSoulPos_[SOUL_TYPE::BLUE]);
	// 回転を計算
	Quaternion rotation = Quaternion::LookRotation(direction);

	// 正規化
	direction = VNorm(direction);

	// 距離の二乗を計算
	float distanceSquared = direction.x * direction.x + direction.y * direction.y + direction.z * direction.z;
	
	// 手との距離が小さければエフェクトをオフにする
	if (VSize(VSub(followSoulPos_[SOUL_TYPE::BLUE], playerPos_)) < SOUL_STOP_POS)
	{
		StopEffect();
	}

	// 移動量を計算
	VECTOR movePow;
	movePow.x = direction.x * moveSpeed_ * deltaTime_;
	movePow.y = direction.y * moveSpeed_ * deltaTime_;
	movePow.z = direction.z * moveSpeed_ * deltaTime_;

	// 魂を移動
	followSoulPos_[SOUL_TYPE::BLUE].x += movePow.x * SOUL_INHALE_SPEED;
	followSoulPos_[SOUL_TYPE::BLUE].y += movePow.y * SOUL_INHALE_SPEED;
	followSoulPos_[SOUL_TYPE::BLUE].z += movePow.z * SOUL_INHALE_SPEED;
	
	
	// 回転を設定
	SetRotationPlayingEffekseer3DEffect(effectSoulPlayId_[SOUL_TYPE::BLUE], rotation.x, rotation.y, rotation.z);

	// 位置を設定
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

	// 三角関数を使用して浮遊させる
	preFollowSoulPos_[SOUL_TYPE::BLUE] = followSoulPos_[SOUL_TYPE::BLUE];
	followSoulPos_[SOUL_TYPE::BLUE].x += sinf(soulMoveCnt_ * 0.7f) * moveSpeed_ * deltaTime_;
	followSoulPos_[SOUL_TYPE::BLUE].y += cosf(soulMoveCnt_ * 1.0f) * moveSpeed_ * deltaTime_;
	followSoulPos_[SOUL_TYPE::BLUE].z += sinf(soulMoveCnt_ * 0.7f) * moveSpeed_ * deltaTime_;

	// 尾を引かせるために後ろの座標をとる
	VECTOR movePow = VSub(followSoulPos_[SOUL_TYPE::BLUE], preFollowSoulPos_[SOUL_TYPE::BLUE]);
	VECTOR dir = VNorm(movePow);
	Quaternion moveRot = Quaternion::LookRotation(dir);
	VECTOR m = moveRot.ToEuler();

	// 時間経過で回転
	effectRot_ += deltaTime_ * SOUL_ROT_SPEED;

	SetRotationPlayingEffekseer3DEffect(effectSoulPlayId_[SOUL_TYPE::BLUE], m.x, m.y, m.z);
	SetPosPlayingEffekseer3DEffect(effectSoulPlayId_[SOUL_TYPE::BLUE], followSoulPos_[SOUL_TYPE::BLUE].x,
		followSoulPos_[SOUL_TYPE::BLUE].y, followSoulPos_[SOUL_TYPE::BLUE].z);
}