#include <string>
#include <EffekseerForDXLib.h>
#include "../Application.h"
#include "../Utility/Utility.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Camera.h"
#include "../Renderer/ModelMaterial.h"
#include "../Renderer/ModelRenderer.h"
#include "Common/AnimationController.h"
#include "Common/Capsule.h"
#include "../Object/Common/Collider/ColliderManager.h"
#include "../Object/Common/Collider/Collider.h"
#include "../Object/Common/Collider/ColliderController.h"
#include "Common/Weapon.h"
#include "TestEnemy.h"
#include "Soul.h"
#include "Player.h"

// 追従対象からエフェクトまでの相対座標(完全追従)
const VECTOR LOCAL_POS = { 0.0f, -5.0f,0.0f };
const VECTOR PLAYER_CAPSULE_TOP = { 0.0f, 250.0f,0.0f };
const VECTOR PLAYER_CAPSULE_BOTTOM = { 0.0f, 90.0f,0.0f };
const VECTOR PLAYER_FOOT_TOP = { 0.0f, 0.0f,0.0f };
const VECTOR PLAYER_FOOT_BOTTOM = { 0.0f, -2.0f,0.0f };
const VECTOR PLAYER_WEAPON_TOP = { 0.0f, 300.0f,0.0f };
const VECTOR PLAYER_WEAPON_BOTTOM = { 0.0f, 40.0f,0.0f };

// キーの登録
const char FRAME_LEFTHAND[] = "mixamorig:LeftHandMiddle1";

// アニメーションスピード
const float NORMAL_ANIM_SPEED = 60.0f;
const float FAST_ANIM_SPEED = 75.0f;

// 重力量
const float GRAVITY_POW = 10.0f;

// 武器のコライダ
const float WEAPON_CAPSULE_RADIUS = 10.0f;

// スピード
static constexpr float SPEED_MOVE = 6.0f;
static constexpr float SPEED_RUN = 6.0f;
static constexpr float PLAYER_CAPSULE_RADIUS = 50.0f;
//static constexpr float SPEED_RUN = 6.0f;

// 回転完了までの時間
static constexpr float TIME_ROT = 1.0f;

// ジャンプ力
static constexpr float POW_JUMP = 35.0f;

// ジャンプ受付時間
static constexpr float TIME_JUMP_IN = 0.5f;

const int DEFAULT_ZERO = 0;

// コントローラー制御
const float	STICK_VALUE_MAX = 1000.0f;

const float	REVERSE_VALUE = -1.0f;
const float	TIMEROT_NORMAL = 0.5f;
const float	TIMEROT_FAST = 0.1f;
const float INPUT_RECEPTION_TIME = 1.0f;

// アニメーションの遷移フレーム
const float FIRSTCOL_FRAME = 36.0f;
const float SECONDCOL_FRAME = 18.0f;
const float THIRDCOL_FRAME = 60.0f;
const float THIRDCOL_SECONDSTEP_FRAME = 96.0f;
const float FIRSTCOMBO_STEP = 62.0f;
const float SECONDCOMBO_STEP = 56.0f;
const float THIRDCOMBO_STEP = 170.0f;
const float INHALE_STOPSTEP = 20.0f;
const float INHALE_STARTSTEP = 30.0f;

Player::Player(void) :
	colliderController_(std::make_unique<ColliderController>()),
	playerFootCollsion_(std::make_unique<ColliderController>()),
	isCollisionStage_(false),
	hitNormal_({})
{
	animationController_ = nullptr;
	state_ = STATE::PLAY;
	
	// 手に持つ武器
	weapon_ = std::make_shared<Weapon>();
	weapon_->Init();
	
	// 衝突判定のセット
	colliderController_->SetCollision(OBJECT_TYPE::PLAYER);
	playerFootCollsion_->SetCollision(OBJECT_TYPE::PLAYER_FOOT);

	// 移動座標系
	moveSpeed_ = 0.0f;
	moveDir_ = Utility::VECTOR_ZERO;
	movePow_ = Utility::VECTOR_ZERO;
	movedPos_ = Utility::VECTOR_ZERO;
	effectInhalePos_ = Utility::VECTOR_ZERO;

	playerRotY_ = Quaternion();
	goalQuaRot_ = Quaternion();
	stepRotTime_ = 0.0f;

	jumpPow_ = Utility::VECTOR_ZERO;
	stepJump_ = 0.0f;
	comboInputTime_ = INPUT_RECEPTION_TIME;

	canMove_ = true;
	isAttack_ = false;
	isJump_ = false;
	isInhale_ = false;
	stopAnim_ = false;
	onCol_ = false;

	// 衝突チェック
	gravHitPosDown_ = Utility::VECTOR_ZERO;
	gravHitPosUp_ = Utility::VECTOR_ZERO;

	imgShadow_ = -1;

	capsule_ = nullptr;

	speedBuffRate_ = 0.0f;


	// 状態管理
	stateChanges_.emplace(STATE::NONE, std::bind(&Player::ChangeStateNone, this));
	stateChanges_.emplace(STATE::PLAY, std::bind(&Player::ChangeStatePlay, this));
	stateChanges_.emplace(STATE::INHALE, std::bind(&Player::ChangeStateInhale, this));
	
}

Player::~Player(void)
{
	DeleteEffekseerEffect(effectInhaleResId_);
}

void Player::Init(void)
{
	// モデルの基本設定
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::PLAYER));
	transform_.scl = Utility::VECTOR_ONE;
	transform_.pos = { 0.0f, 0.0f, 0.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, Utility::Deg2RadF(180.0f), 0.0f });

	// 追従のためにフレームの子から番号を取得
	hipsBoneIndex_ = MV1GetFrameChild(transform_.modelId, 1);
	leftHandBoneFrame_ = MV1SearchFrame(transform_.modelId, FRAME_LEFTHAND);
	frameNo_ = MV1SearchFrame(transform_.modelId, "mixamorig:Hips");
	transform_.Update();

	// アニメーションの設定
	InitAnimation();
	
	// カプセルコライダ
	capsule_ = std::make_unique<Capsule>(transform_);
	capsule_->SetLocalPosTop({ 0.0f, 110.0f, 0.0f });
	capsule_->SetLocalPosDown({ 0.0f, 30.0f, 0.0f });
	capsule_->SetRadius(20.0f);

	isCollisionStage_ = false;
	firstComboTriggered_ = false;
	secondComboTriggered_ = false;
	thirdComboTriggered_ = false;
	AddCollider();

	// 重力
	gravity_ = GRAVITY_POW;

	// 丸影画像
	imgShadow_ = resMng_.Load(ResourceManager::SRC::PLAYER_SHADOW).handleId_;

	// トゥーンシェーダ用画像
	imgToonMap_ = resMng_.Load(ResourceManager::SRC::TEX_TOON).handleId_;

	// モデル描画用
	material_ = std::make_unique<ModelMaterial>(
		"ToonModelVS6.cso", 2,
		"ToonModelPS6.cso", 3
		);
	material_->SetTextureBuf(3, imgToonMap_);

	// 拡散光
	material_->AddConstBufPS({ 1.0f, 1.0f, 1.0f, 1.0f });

	// 環境光
	material_->AddConstBufPS({ 0.2f, 0.2f, 0.2f, 1.0f });

	// 頂点①：カメラ座標
	auto& camera = SceneManager::GetInstance().GetCamera();
	auto cPos = camera.GetPos();
	material_->AddConstBufVS({ cPos.x, cPos.y, cPos.z, 0.0f });

	// 頂点②：フォグ開始・終了距離
	float fogStart;
	float fogEnd;
	GetFogStartEnd(&fogStart, &fogEnd);
	material_->AddConstBufVS({ fogStart, fogEnd, 0.0f, 0.0f });

	// ライトの方向
	VECTOR worldLightDirection = GetLightDirection();
	material_->AddConstBufPS(
		{ worldLightDirection.x, worldLightDirection.y, worldLightDirection.z, 0.0f });

	renderer_ = std::make_unique<ModelRenderer>(transform_.modelId, *material_);

	hitEfResId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::EFFECT_HIT).handleId_;
	
	effectInhaleResId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::EFFECT_INHALE).handleId_;

	stepRotTime_ = TIMEROT_NORMAL;
	effectHitScale_ = 50.0f;
	// 初期状態
	ChangeAnim(ANIM_TYPE::IDLE);
	ChangeState(STATE::PLAY);
}

void Player::Update(void)
{
	// 更新ステップ
	stateUpdate_();

	// モデル制御更新
	transform_.Update();

	// 移動値無効化
	DisableAnimMovePow();

	// 左手のフレーム位置を取得し続ける
	leftHandPos_ = MV1GetFramePosition(transform_.modelId, leftHandBoneFrame_);

	// アニメーション再生
	animationController_->Update();

	// 武器位置同期
	weapon_->WeaponUpdate(transform_, true, true);
}

void Player::Draw(void)
{
	// 頂点：カメラ座標
	auto& camera = SceneManager::GetInstance().GetCamera();
	auto cPos = camera.GetPos();
	material_->SetConstBufVS(0, { cPos.x, cPos.y, cPos.z, 0.0f });
	auto lHand = MV1GetFramePosition(transform_.modelId, leftHandBoneFrame_);
	DrawFormatString(0, 0, GetColor(255, 255, 255), "Position: (%0.2f,%0.2f,%0.2f)", transform_.pos.x, transform_.pos.y, transform_.pos.z);// 頂点：カメラ座標
	DrawFormatString(0, 40, GetColor(255, 255, 255), "handPosition: (%0.2f,%0.2f,%0.2f)", lHand.x, lHand.y, lHand.z);// 頂点：カメラ座標

	// モデルの描画
	renderer_->Draw();
	
	// 武器の描画
	weapon_->Draw();
	
	// 丸影描画
	DrawShadow();
}

bool Player::IsAttack(void)
{
	return onCol_;
}

void Player::AddCollider(void)
{
	//プレイヤー
	colMng_.AddCollider(
		OBJECT_TYPE::PLAYER,
		COL_TYPE::CAPSULE,
		transform_,
		true,
		0, 0,
		ENEMY_SUBTYPE::NONE,
		PLAYER_CAPSULE_RADIUS,
		PLAYER_CAPSULE_TOP,
		PLAYER_CAPSULE_BOTTOM
	);

	colMng_.AddCollider(
		OBJECT_TYPE::PLAYER_FOOT,
		COL_TYPE::LINE,
		transform_,
		true,
		0, 0,
		ENEMY_SUBTYPE::NONE, 0.0f,
		PLAYER_FOOT_TOP,
		PLAYER_FOOT_BOTTOM
	);

	//武器
	colMng_.AddCollider(
		OBJECT_TYPE::WEAPON, COL_TYPE::CAPSULE,
		weapon_->GetTransform(),
		true,
		0, 0,
		ENEMY_SUBTYPE::NONE, WEAPON_CAPSULE_RADIUS,
		PLAYER_WEAPON_TOP, PLAYER_WEAPON_BOTTOM);

}

void Player::SetCollider(void)
{
}

void Player::ClearCollider(void)
{
	collider_.clear();
}

const Capsule& Player::GetCapsule(void) const
{
	return *capsule_;
}

VECTOR& Player::GetLeftHandPos(void)
{
	return leftHandPos_;
}

VECTOR& Player::GetPos(void)
{
	return transform_.pos;
}

const Player::ANIM_TYPE Player::GetNowAnim(void) const
{
	return animType_;
}

void Player::InitAnimation(void)
{
	std::string path = Application::PATH_MODEL + "Player/";
	animationController_ = std::make_unique<AnimationController>(transform_.modelId);
	
	// 待機アニメーション
	animationController_->Add("IDLE", path + "Idle.mv1",
		0.0f, NORMAL_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER), true, 0, false);

	// 移動系アニメーション
	animationController_->Add("WALK", path + "Walk.mv1",
		0.0f, NORMAL_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_WALK), true, 0, false);
	animationController_->Add("RUN", path + "Run.mv1",
		0.0f, FAST_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_RUN), true, 0, false);
	
	// 攻撃系アニメーション
	animationController_->Add("COMBO_ONE", path + "ComboFirst.mv1",
		0.0f, NORMAL_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_FIRSTCOMBO), false, 0, false);
	animationController_->Add("COMBO_TWO", path + "ComboSecond.mv1",
		0.0f, NORMAL_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_SECONDCOMBO), false, 0, false);
	animationController_->Add("COMBO_THREE", path + "ComboThird.mv1",
		0.0f, NORMAL_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_THIRDCOMBO), false, 0, false);

	// 魂吸収アニメーション
	animationController_->Add("INHALE", path + "Inhale.mv1",
		0.0f, NORMAL_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_INHALE), false, 0, false);

}

void Player::BlendAnimation(void)
{
	
}

void Player::ChangeAnim(ANIM_TYPE state)
{
	if (animType_ != state)
	{
		animType_ = state;

		currentState_.emplace_back(ANIM_DATA_KEY[(int)state]);

		preAnimationKey_ = animationKey_;

		animationKey_ = ANIM_DATA_KEY[(int)state];

		animationController_->ChangeAnimation(animationKey_);

		canMove_ = true;
	}
}

void Player::DisableAnimMovePow(void)
{
	// 対象フレームのローカル行列を初期値にリセット
	MV1ResetFrameUserLocalMatrix(transform_.modelId, frameNo_);

	// 対象フレームのローカル行列(大きさ、回転、位置)を取得
	auto mat = MV1GetFrameLocalMatrix(transform_.modelId, frameNo_);
	auto mScl = MGetSize(mat);
	auto mRot = MGetRotElem(mat);
	auto mPos = MGetTranslateElem(mat);

	// 大きさ、回転、位置をローカル座標に戻す
	MATRIX mix = MGetIdent();
	mix = MMult(mix, MGetScale(mScl));	// 大きさ
	mix = MMult(mix, mRot);				// 回転

	// 調整したローカル座標を行列に設定
	mix = MMult(mix, MGetTranslate({ 0.0f,79.0f,0.0f }));

	// 移動値を無効化
	MV1SetFrameUserLocalMatrix(transform_.modelId, frameNo_, mix);

}

void Player::ChangeState(STATE state)
{
	// 以前の状態を入れる
	preState_ = state_;

	// 状態変更
	state_ = state;

	// 各状態遷移の初期処理
	stateChanges_[state_]();

	// アニメーションキーに登録
	preAnimationKey_ = animationKey_;
	animationKey_ = ANIM_DATA_KEY[(int)state];

	// アニメーションを変更
	animationController_->ChangeAnimation(animationKey_);
}

void Player::ChangeStateNone(void)
{
	stateUpdate_ = std::bind(&Player::UpdateNone, this);
}

void Player::ChangeStatePlay(void)
{
	stateUpdate_ = std::bind(&Player::UpdatePlay, this);
}

void Player::ChangeStateInhale(void)
{
	stateUpdate_ = std::bind(&Player::UpdateInhale, this);
}

void Player::UpdateNone(void)
{
}

void Player::UpdatePlay(void)
{
	//移動量ゼロ
	movePow_ = Utility::VECTOR_ZERO;
	moveDir_ = Utility::VECTOR_ZERO;
	
	// 衝突判定フレーム
	OnColliderFrame();
	
	if (!isAttack_ && canMove_)
	{
		// 移動処理
		ProcessMove();

		// 移動方向に応じた回転
		Rotate();

		// 魂の吸収判定
		isInhale_ = false;

		// アニメーションを再生させる
		stopAnim_ = true;
	}
	
	// 攻撃処理
	ProcessAttack();

	// 魂の吸収
	ProcessInhale();

	// 重力による移動量
	CalcGravityPow();

	// 衝突判定
	Collision();
	
	// 回転させる
	transform_.quaRot = playerRotY_;
}

void Player::HitEffect(VECTOR pos)
{
	hitEfPlayId_ = PlayEffekseer3DEffect(hitEfResId_);

	SetPosPlayingEffekseer3DEffect(hitEfPlayId_, pos.x, pos.y, pos.z);
	SetScalePlayingEffekseer3DEffect(hitEfPlayId_, effectHitScale_, effectHitScale_, effectHitScale_);
}

void Player::UpdateInhale(void)
{
}

void Player::DrawShadow(void)
{
	float PLAYER_SHADOW_HEIGHT = 300.0f;
	float PLAYER_SHADOW_SIZE = 30.0f;

	int i;
	MV1_COLL_RESULT_POLY_DIM HitResDim;
	MV1_COLL_RESULT_POLY* HitRes;
	VERTEX3D Vertex[3] = { VERTEX3D(), VERTEX3D(), VERTEX3D() };
	VECTOR SlideVec;
	int ModelHandle;

	// ライティングを無効にする
	SetUseLighting(FALSE);

	// Ｚバッファを有効にする
	SetUseZBuffer3D(TRUE);

	// テクスチャアドレスモードを CLAMP にする( テクスチャの端より先は端のドットが延々続く )
	SetTextureAddressMode(DX_TEXADDRESS_CLAMP);

	//// 影を落とすモデルの数だけ繰り返し
	//for (const auto c : collider_)
	//{
	//	// チェックするモデルは、jが0の時はステージモデル、1以上の場合はコリジョンモデル
	//	ModelHandle = c.lock()->modelId_;

	//	// プレイヤーの直下に存在する地面のポリゴンを取得
	//	HitResDim = MV1CollCheck_Capsule(
	//		ModelHandle, -1,
	//		transform_.pos, VAdd(transform_.pos, { 0.0f, -PLAYER_SHADOW_HEIGHT, 0.0f }), PLAYER_SHADOW_SIZE);

	//	// 頂点データで変化が無い部分をセット
	//	Vertex[0].dif = GetColorU8(255, 255, 255, 255);
	//	Vertex[0].spc = GetColorU8(0, 0, 0, 0);
	//	Vertex[0].su = 0.0f;
	//	Vertex[0].sv = 0.0f;
	//	Vertex[1] = Vertex[0];
	//	Vertex[2] = Vertex[0];

	//	// 球の直下に存在するポリゴンの数だけ繰り返し
	//	HitRes = HitResDim.Dim;
	//	for (i = 0; i < HitResDim.HitNum; i++, HitRes++)
	//	{
	//		// ポリゴンの座標は地面ポリゴンの座標
	//		Vertex[0].pos = HitRes->Position[0];
	//		Vertex[1].pos = HitRes->Position[1];
	//		Vertex[2].pos = HitRes->Position[2];

	//		// ちょっと持ち上げて重ならないようにする
	//		SlideVec = VScale(HitRes->Normal, 0.5f);
	//		Vertex[0].pos = VAdd(Vertex[0].pos, SlideVec);
	//		Vertex[1].pos = VAdd(Vertex[1].pos, SlideVec);
	//		Vertex[2].pos = VAdd(Vertex[2].pos, SlideVec);

	//		// ポリゴンの不透明度を設定する
	//		Vertex[0].dif.a = 0;
	//		Vertex[1].dif.a = 0;
	//		Vertex[2].dif.a = 0;
	//		if (HitRes->Position[0].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
	//			Vertex[0].dif.a = static_cast<int>(roundf(128.0f * (1.0f - fabs(HitRes->Position[0].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT)));

	//		if (HitRes->Position[1].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
	//			Vertex[1].dif.a = static_cast<int>(roundf(128.0f * (1.0f - fabs(HitRes->Position[1].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT)));

	//		if (HitRes->Position[2].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
	//			Vertex[2].dif.a = static_cast<int>(roundf(128.0f * (1.0f - fabs(HitRes->Position[2].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT)));

	//		// ＵＶ値は地面ポリゴンとプレイヤーの相対座標から割り出す
	//		Vertex[0].u = (HitRes->Position[0].x - transform_.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
	//		Vertex[0].v = (HitRes->Position[0].z - transform_.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
	//		Vertex[1].u = (HitRes->Position[1].x - transform_.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
	//		Vertex[1].v = (HitRes->Position[1].z - transform_.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
	//		Vertex[2].u = (HitRes->Position[2].x - transform_.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
	//		Vertex[2].v = (HitRes->Position[2].z - transform_.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;

	//		// 影ポリゴンを描画
	//		DrawPolygon3D(Vertex, 1, imgShadow_, TRUE);
	//	}

	//	// 検出した地面ポリゴン情報の後始末
	//	MV1CollResultPolyDimTerminate(HitResDim);
	//}

	// ライティングを有効にする
	SetUseLighting(TRUE);

	// Ｚバッファを無効にする
	SetUseZBuffer3D(FALSE);
}

void Player::ProcessMove(void)
{
	auto& ins = InputManager::GetInstance();

	// 移動量ゼロ
	movePow_ = Utility::VECTOR_ZERO;
	moveDir_ = Utility::VECTOR_ZERO;

	MoveControll();

	if (!Utility::EqualsVZero(moveDir_))
	{
		movePow_ = VScale(moveDir_, moveSpeed_);

		// 回転処理
		SetGoalRotate(rotRad_);

		ChangeAnim(ANIM_TYPE::RUN);
		moveSpeed_ = SPEED_MOVE;
	}
	else
	{
		ChangeAnim(ANIM_TYPE::IDLE);
	}
}

void Player::ProcessInhale(void)
{
	// 位置の同期
	SyncInhaleEffect();

	auto& ins = InputManager::GetInstance();
	// アニメーションの再生フレーム取得
	float animInhaleStep = animationController_->GetAnimData("INHALE").stepAnim;
	if (ins.IsPressed(InputManager::ACTION::INHALE))
	{ 
		// 再生
		effectInhalePlayId_ = PlayEffekseer3DEffect(effectInhaleResId_);

		// 移動量ゼロ
		movePow_ = Utility::VECTOR_ZERO;
		moveDir_ = Utility::VECTOR_ZERO;
		
		// 動けなくする
		canMove_ = false;
		ChangeAnim(ANIM_TYPE::INHALE);
	}
	// 特定のフレームで停止させ、任意のフレームから再生
	if (animInhaleStep >= INHALE_STOPSTEP)
	{
		stopAnim_ = true;
		animationController_->Stop(stopAnim_);
		animationController_->SetStartStepAnim("INHALE", INHALE_STARTSTEP);
	}
}
	
void Player::ProcessJump(void)
{

	bool isHit = CheckHitKey(KEY_INPUT_BACKSLASH);

	// ジャンプ
	if (isHit && (isJump_ || IsEndLanding()))
	{

		if (!isJump_)
		{
			//// 無理やりアニメーション
			//animationController_->Play((int)ANIM_TYPE::JUMP, true, 13.0f, 25.0f);
			//animationController_->SetEndLoop(23.0f, 25.0f, 5.0f);
		}

		isJump_ = true;

		// ジャンプの入力受付時間をヘラス
		stepJump_ += scnMng_.GetDeltaTime();
		if (stepJump_ < TIME_JUMP_IN)
		{
			jumpPow_ = VScale(Utility::DIR_U, POW_JUMP);
		}
	}

	// ボタンを離したらジャンプ力に加算しない
	if (!isHit)
	{
		stepJump_ = TIME_JUMP_IN;
	}
}

void Player::MoveControll(void)
{
	rotRad_ = 0.0f;
	auto& ins = InputManager::GetInstance();

	auto pad = ins.GetJPadState(JOYPAD_NO::PAD1);
	Quaternion cameraRot = SceneManager::GetInstance().GetCamera().GetQuaRotOutX();

	if (ins.IsPressed(InputManager::ACTION::MOVE_FORWARD))
	{
		moveDir_ = cameraRot.GetForward();
		rotRad_ = static_cast<float>(Utility::Deg2RadD(0.0f));
	}

	if (ins.IsPressed(InputManager::ACTION::MOVE_BACK))
	{
		moveDir_ = cameraRot.GetBack();
		rotRad_ = static_cast<float>(Utility::Deg2RadD(180.0f));
	}

	if (ins.IsPressed(InputManager::ACTION::MOVE_LEFT))
	{
		moveDir_ = cameraRot.GetLeft();
		rotRad_ = static_cast<float>(Utility::Deg2RadD(270.0f));
	}

	if (ins.IsPressed(InputManager::ACTION::MOVE_RIGHT))
	{
		moveDir_ = cameraRot.GetRight();
		rotRad_ = static_cast<float>(Utility::Deg2RadD(30.0f));
	}
	if (pad.AkeyL.x != 0 || pad.AkeyL.y != 0)
	{
		moveDir_ = { static_cast<float>(pad.AkeyL.x) / STICK_VALUE_MAX, 0.0f ,
			static_cast<float>(pad.AkeyL.y) / STICK_VALUE_MAX * REVERSE_VALUE };

		rotRad_ = atan2f(moveDir_.x, moveDir_.z);

		VECTOR CameraAngle = SceneManager::GetInstance().GetCamera().GetAngles();

		MATRIX mat = MGetIdent();
		mat = MMult(mat, MGetRotY(CameraAngle.y));
	
		// 回転行列を使用して、ベクトルを回転させる
		moveDir_ = VTransform(moveDir_, mat);

		stepRotTime_ = TIMEROT_FAST;
	}
	else
	{
		stepRotTime_ = TIMEROT_NORMAL;
	}
}

void Player::ProcessAttack(void)
{
	auto& ins = InputManager::GetInstance();
	float deltaTime_ = 1.0;
	
	// コンボタイマーを減らす
	if (comboInputTime_ >= 0.0f)
	{
		comboInputTime_ -= deltaTime_;
	}
	else
	{
		// 攻撃リセット
		comboStep_ = 0;
		isAttack_ = false;
	}

	// 攻撃ボタンが押された場合
	if (ins.IsTriggered(InputManager::ACTION::ATTACK) && !isAttack_)
	{
		// 最初の攻撃
		ChangeAnim(ANIM_TYPE::COMBO_ONE);
		comboInputTime_ = INPUT_RECEPTION_TIME;
		comboStep_ = 1;
		isAttack_ = true;
	}
	else if (comboStep_ == 1 && comboInputTime_ >= 0.0f
		&& ins.IsTriggered(InputManager::ACTION::ATTACK) && animationController_->IsEndPlayAnimation())
	{
		// 二段目攻撃
		ChangeAnim(ANIM_TYPE::COMBO_TWO);
		comboInputTime_ = INPUT_RECEPTION_TIME;
		comboStep_ = 2;
		isAttack_ = true;
	}
 	else if (comboStep_ == 2 && comboInputTime_ >= 0.0f
		&& ins.IsTriggered(InputManager::ACTION::ATTACK) && animationController_->IsEndPlayAnimation())
	{
		// 三段目攻撃
		ChangeAnim(ANIM_TYPE::COMBO_THREE);
		comboStep_ = 3;
		comboInputTime_ = INPUT_RECEPTION_TIME;
		isAttack_ = true;
	}
	else if (comboStep_ == 3 && comboInputTime_ >= 0.0f
		&& !ins.IsTriggered(InputManager::ACTION::ATTACK))
	{
		isAttack_ = false;
	}
	
	// コンボ段階を下げる
	ResetCombo();
	
	if (animType_ == ANIM_TYPE::COMBO_ONE||
		animType_ == ANIM_TYPE::COMBO_TWO||
		animType_ == ANIM_TYPE::COMBO_THREE)
	{
		return;
	}
}

void Player::ResetCombo(void)
{
	// アニメーションの再生フレーム取得
	float firstComboStep = animationController_->GetAnimData("COMBO_ONE").stepAnim;
	float secondComboStep = animationController_->GetAnimData("COMBO_TWO").stepAnim;
	float thirdComboStep = animationController_->GetAnimData("COMBO_THREE").stepAnim;
	float firstComboTotalStep = animationController_->GetAnimData("COMBO_ONE").animTotalTime;
	float secondComboTotalStep = animationController_->GetAnimData("COMBO_TWO").animTotalTime;
	float thirdComboTotalStep = animationController_->GetAnimData("COMBO_THREE").animTotalTime;

	// 判定をリセット
	comboStep_ = COMBOCOUNT::NONE;

	// 入力受付時間のリセットと少し延ばす
	comboInputTime_ = INPUT_RECEPTION_TIME;

	// 判定範囲拡大のための値
	const float judgeComboFrame = 2.0f;

	// 特定のフレーム範囲内で判定されていない場合
	if (!firstComboTriggered_ && firstComboStep >= 
		FIRSTCOMBO_STEP - judgeComboFrame && firstComboStep <= FIRSTCOMBO_STEP + judgeComboFrame)
	{
		comboStep_ = COMBOCOUNT::FIRSTSTEP;
		firstComboTriggered_ = true;
	}
	else if (!secondComboTriggered_ && secondComboStep >= SECONDCOMBO_STEP - 
		judgeComboFrame && secondComboStep <= SECONDCOMBO_STEP + judgeComboFrame)
	{
		comboStep_ = COMBOCOUNT::SECONDSTEP;
		secondComboTriggered_ = true;
	}
	else if (!thirdComboTriggered_ && thirdComboStep >= THIRDCOMBO_STEP - 
		judgeComboFrame && thirdComboStep <= THIRDCOMBO_STEP + judgeComboFrame)
	{
		comboStep_ = COMBOCOUNT::THIRDSTEP;
		thirdComboTriggered_ = true;
	}
	else
	{
		comboStep_ = COMBOCOUNT::NONE;
	}

	// アニメーションのリセット条件
	if (animationController_->IsEndPlayAnimation())
	{
		firstComboTriggered_ = false;
		secondComboTriggered_ = false;
		thirdComboTriggered_ = false;
	}
}

void Player::SyncInhaleEffect(void)
{
	VECTOR followInhalePos = GetLeftHandPos();

	Quaternion followRot = transform_.quaRot;
	VECTOR rot = Quaternion::ToEuler(followRot);

	VECTOR effectPos = followRot.PosAxis(LOCAL_POS);

	// 位置の更新
	effectInhalePos_ = VAdd(followInhalePos, effectPos);

	SetPosPlayingEffekseer3DEffect(effectInhalePlayId_, effectInhalePos_.x, effectInhalePos_.y, effectInhalePos_.z);
	SetRotationPlayingEffekseer3DEffect(effectInhalePlayId_, rot.x, rot.y, rot.z);
	SetScalePlayingEffekseer3DEffect(effectInhalePlayId_, 10.0f, 10.0f, 10.0f);

	transform_.Update();
}

bool Player::IsEndDodge(void)
{
	return true;
}

bool Player::IsEndGuard(void)
{
	return true;
}

void Player::SetGoalRotate(double rotRad)
{
	VECTOR cameraRot = mainCamera.GetAngles();
	Quaternion axis = Quaternion::AngleAxis((double)cameraRot.y + rotRad, Utility::AXIS_Y);

	// 現在設定されている回転との角度差を取る
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// しきい値
	if (angleDiff > 0.1)
	{
		stepRotTime_ = TIME_ROT;
	}

	goalQuaRot_ = axis;
}

void Player::Rotate(void)
{
	stepRotTime_ -= scnMng_.GetDeltaTime();

	// 回転の球面補間
	playerRotY_ = Quaternion::Slerp(
		playerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}

void Player::Collision(void)
{
	// 現在座標を起点に移動後座標を決める
	movedPos_ = VAdd(transform_.pos, movePow_);

	// 衝突(カプセル)
	//CollisionCapsule();
	TestCollisionCapsule();

	// 衝突(重力)
	SimpleGravity();

	// 移動
	transform_.pos = movedPos_;
}

void Player::SimpleGravity(void)
{
	// 足元の衝突
	const MV1_COLL_RESULT_POLY& hitFoot =
		playerFootCollsion_->OnCollisionResultPoly_Line(OBJECT_TYPE::STAGE);

	// 足元の衝突判定
	if (hitFoot.HitFlag)
	{
		gravity_ = 0.0f;
		transform_.pos.y = hitFoot.HitPosition.y;
		return;
	}

	if (transform_.pos.y >= 0)
	{
		transform_.pos.y -= gravity_;
	}
}

void Player::CollisionCapsule(void)
{
	//// カプセルを移動させる
	//Transform trans = Transform(transform_);
	//trans.pos = movedPos_;
	//trans.Update();
	//Capsule cap = Capsule(*capsule_, trans);

	//// カプセルとの衝突判定
	//for (const auto c : collider_)
	//{

	//	auto hits = MV1CollCheck_Capsule(
	//		c.lock()->modelId_, -1,
	//		cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius());

	//	for (int i = 0; i < hits.HitNum; i++)
	//	{

	//		auto hit = hits.Dim[i];

	//		for (int tryCnt = 0; tryCnt < 10; tryCnt++)
	//		{

	//			int pHit = HitCheck_Capsule_Triangle(
	//				cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius(),
	//				hit.Position[0], hit.Position[1], hit.Position[2]);

	//			if (pHit)
	//			{
	//				movedPos_ = VAdd(movedPos_, VScale(hit.Normal, 1.0f));
	//				// カプセルを移動させる
	//				trans.pos = movedPos_;
	//				trans.Update();
	//				continue;
	//			}

	//			break;

	//		}

	//	}
	//	// 検出した地面ポリゴン情報の後始末
	//	MV1CollResultPolyDimTerminate(hits);
	//}
}

void Player::TestCollisionCapsule(void)
{
	// カプセルを移動させる
	Transform trans = Transform(transform_);
	trans.pos = movedPos_;
	trans.Update();

	isCollisionStage_ = colliderController_->OnCollision(OBJECT_TYPE::STAGE);

	auto& hits =
		colliderController_->OnCollisionResultPoly(OBJECT_TYPE::STAGE);
	hitNormal_ = Utility::VECTOR_ZERO;

	auto& cols = colMng_.GetColliders();
	int pHit = DEFAULT_ZERO;

	// 衝突した複数のポリゴンと衝突回避するまで、
	// プレイヤーの位置を移動させる
	for (int i = DEFAULT_ZERO; i < hits.HitNum; i++)
	{
		auto& hit = hits.Dim[i];
		// 地面と異なり、衝突回避位置が不明なため、何度か移動させる
		// この時、移動させる方向は、移動前座標に向いた方向であったり、
		// 衝突したポリゴンの法線方向だったりする
		for (int tryCnt = DEFAULT_ZERO; tryCnt < 10; tryCnt++)
		{
			// 再度、モデル全体と衝突検出するには、効率が悪過ぎるので、
			// 最初の衝突判定で検出した衝突ポリゴン1枚と衝突判定を取る

			for (const auto& player : colMng_.GetColliders())
			{
				if (player->GetObjType() != OBJECT_TYPE::PLAYER) { continue; }

				pHit = HitCheck_Capsule_Triangle(
					player->GetPosTop(),
					player->GetPosDown(),
					player->GetRadius(),
					hit.Position[0], hit.Position[1], hit.Position[2]);
				break;

			}

			if (pHit)
			{
				float pushPow = moveSpeed_ / 7 * speedBuffRate_;
				// 法線の方向にちょっとだけ移動させる
				movedPos_ = VAdd(movedPos_, VScale(hit.Normal, pushPow));
				// カプセルも一緒に移動させる
				trans.pos = movedPos_;
				trans.Update();
				continue;
			}
			break;
		}
	}
	auto& hitFoot =
		playerFootCollsion_->OnCollisionResultPoly_Line(OBJECT_TYPE::STAGE);
	if (hitFoot.HitFlag)
	{
		hitNormal_ = hitFoot.Normal;
	}
}

void Player::OnColliderFrame(void)
{
	// アニメーションの再生フレーム取得
	float firstComboStep = animationController_->GetAnimData("COMBO_ONE").stepAnim;
	float secondComboStep = animationController_->GetAnimData("COMBO_TWO").stepAnim;
	float thirdComboStep = animationController_->GetAnimData("COMBO_THREE").stepAnim;
	// 判定フラグ（フレームごとにトリガー管理）
	static bool firstColTriggered = false;
	static bool secondColTriggered = false;
	static bool thirdColTriggered = false;
	static bool thirdColSecondStepTriggered = false;

	// 判定をリセット
	onCol_ = false;

	// 判定範囲拡大のための値
	const float judgeComboFrame = 2.0f;

	// 特定フレーム範囲内で、まだ判定されていない場合
	if (!firstColTriggered &&
		firstComboStep >= FIRSTCOL_FRAME - judgeComboFrame
		&& firstComboStep <= FIRSTCOL_FRAME + judgeComboFrame)
	{
		onCol_ = true;
		firstColTriggered = true;
	}
	else if (!secondColTriggered &&
		secondComboStep >= SECONDCOL_FRAME - judgeComboFrame
		&& secondComboStep <= SECONDCOL_FRAME + judgeComboFrame)
	{
		onCol_ = true;
		secondColTriggered = true;
	}
	else if (!thirdColTriggered &&
		thirdComboStep >= THIRDCOL_FRAME - judgeComboFrame 
		&& thirdComboStep <= THIRDCOL_FRAME + judgeComboFrame)
	{
		onCol_ = true;
		thirdColTriggered = true;
	}
	else if (!thirdColSecondStepTriggered &&
		thirdComboStep >= THIRDCOL_SECONDSTEP_FRAME - judgeComboFrame 
		&& thirdComboStep <= THIRDCOL_SECONDSTEP_FRAME + judgeComboFrame)
	{
		onCol_ = true;
		thirdColSecondStepTriggered = true;
	}

	// アニメーションのリセット条件（必要に応じて実装）
	if (animationController_->IsEndPlayAnimation())
	{
		firstColTriggered = false;
		secondColTriggered = false;
		thirdColTriggered = false;
		thirdColSecondStepTriggered = false;
	}
}

void Player::CalcGravityPow(void)
{
	// 重力方向
	VECTOR dirGravity = Utility::DIR_D;

	// 重力の強さ
	float gravityPow =1.0f/*= Stage::DEFAULT_GRAVITY_POW*/;

	// 重力
	VECTOR gravity = VScale(dirGravity, gravityPow);
	jumpPow_ = VAdd(jumpPow_, gravity);

	// 最初は実装しない。地面と突き抜けることを確認する。
	// 内積
	float dot = VDot(dirGravity, jumpPow_);
	if (dot >= 0.0f)
	{
		// 重力方向と反対方向(マイナス)でなければ、ジャンプ力を無くす
		jumpPow_ = gravity;
	}
}

bool Player::IsEndLanding(void)
{
	bool ret = true;

	// アニメーションがジャンプではない
	if (animationController_->GetPlayType() != "JUMP")
	{
		return ret;
	}

	// アニメーションが終了しているか
	if (animationController_->IsEndPlayAnimation())
	{
		return ret;
	}

	return false;
}