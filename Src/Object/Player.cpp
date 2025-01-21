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

// �Ǐ]�Ώۂ���G�t�F�N�g�܂ł̑��΍��W(���S�Ǐ])
const VECTOR LOCAL_POS = { 0.0f, -5.0f,0.0f };
const VECTOR PLAYER_CAPSULE_TOP = { 0.0f, 250.0f,0.0f };
const VECTOR PLAYER_CAPSULE_BOTTOM = { 0.0f, 90.0f,0.0f };
const VECTOR PLAYER_FOOT_TOP = { 0.0f, 0.0f,0.0f };
const VECTOR PLAYER_FOOT_BOTTOM = { 0.0f, -2.0f,0.0f };
const VECTOR PLAYER_WEAPON_TOP = { 0.0f, 300.0f,0.0f };
const VECTOR PLAYER_WEAPON_BOTTOM = { 0.0f, 40.0f,0.0f };

// �L�[�̓o�^
const char FRAME_LEFTHAND[] = "mixamorig:LeftHandMiddle1";

// �A�j���[�V�����X�s�[�h
const float NORMAL_ANIM_SPEED = 60.0f;
const float FAST_ANIM_SPEED = 75.0f;

// �d�͗�
const float GRAVITY_POW = 10.0f;

// ����̃R���C�_
const float WEAPON_CAPSULE_RADIUS = 10.0f;

// �X�s�[�h
static constexpr float SPEED_MOVE = 6.0f;
static constexpr float SPEED_RUN = 6.0f;
static constexpr float PLAYER_CAPSULE_RADIUS = 50.0f;
//static constexpr float SPEED_RUN = 6.0f;

// ��]�����܂ł̎���
static constexpr float TIME_ROT = 1.0f;

// �W�����v��
static constexpr float POW_JUMP = 35.0f;

// �W�����v��t����
static constexpr float TIME_JUMP_IN = 0.5f;

const int DEFAULT_ZERO = 0;

// �R���g���[���[����
const float	STICK_VALUE_MAX = 1000.0f;

const float	REVERSE_VALUE = -1.0f;
const float	TIMEROT_NORMAL = 0.5f;
const float	TIMEROT_FAST = 0.1f;
const float INPUT_RECEPTION_TIME = 1.0f;

// �A�j���[�V�����̑J�ڃt���[��
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
	
	// ��Ɏ�����
	weapon_ = std::make_shared<Weapon>();
	weapon_->Init();
	
	// �Փ˔���̃Z�b�g
	colliderController_->SetCollision(OBJECT_TYPE::PLAYER);
	playerFootCollsion_->SetCollision(OBJECT_TYPE::PLAYER_FOOT);

	// �ړ����W�n
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

	// �Փ˃`�F�b�N
	gravHitPosDown_ = Utility::VECTOR_ZERO;
	gravHitPosUp_ = Utility::VECTOR_ZERO;

	imgShadow_ = -1;

	capsule_ = nullptr;

	speedBuffRate_ = 0.0f;


	// ��ԊǗ�
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
	// ���f���̊�{�ݒ�
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::PLAYER));
	transform_.scl = Utility::VECTOR_ONE;
	transform_.pos = { 0.0f, 0.0f, 0.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, Utility::Deg2RadF(180.0f), 0.0f });

	// �Ǐ]�̂��߂Ƀt���[���̎q����ԍ����擾
	hipsBoneIndex_ = MV1GetFrameChild(transform_.modelId, 1);
	leftHandBoneFrame_ = MV1SearchFrame(transform_.modelId, FRAME_LEFTHAND);
	frameNo_ = MV1SearchFrame(transform_.modelId, "mixamorig:Hips");
	transform_.Update();

	// �A�j���[�V�����̐ݒ�
	InitAnimation();
	
	// �J�v�Z���R���C�_
	capsule_ = std::make_unique<Capsule>(transform_);
	capsule_->SetLocalPosTop({ 0.0f, 110.0f, 0.0f });
	capsule_->SetLocalPosDown({ 0.0f, 30.0f, 0.0f });
	capsule_->SetRadius(20.0f);

	isCollisionStage_ = false;
	firstComboTriggered_ = false;
	secondComboTriggered_ = false;
	thirdComboTriggered_ = false;
	AddCollider();

	// �d��
	gravity_ = GRAVITY_POW;

	// �ۉe�摜
	imgShadow_ = resMng_.Load(ResourceManager::SRC::PLAYER_SHADOW).handleId_;

	// �g�D�[���V�F�[�_�p�摜
	imgToonMap_ = resMng_.Load(ResourceManager::SRC::TEX_TOON).handleId_;

	// ���f���`��p
	material_ = std::make_unique<ModelMaterial>(
		"ToonModelVS6.cso", 2,
		"ToonModelPS6.cso", 3
		);
	material_->SetTextureBuf(3, imgToonMap_);

	// �g�U��
	material_->AddConstBufPS({ 1.0f, 1.0f, 1.0f, 1.0f });

	// ����
	material_->AddConstBufPS({ 0.2f, 0.2f, 0.2f, 1.0f });

	// ���_�@�F�J�������W
	auto& camera = SceneManager::GetInstance().GetCamera();
	auto cPos = camera.GetPos();
	material_->AddConstBufVS({ cPos.x, cPos.y, cPos.z, 0.0f });

	// ���_�A�F�t�H�O�J�n�E�I������
	float fogStart;
	float fogEnd;
	GetFogStartEnd(&fogStart, &fogEnd);
	material_->AddConstBufVS({ fogStart, fogEnd, 0.0f, 0.0f });

	// ���C�g�̕���
	VECTOR worldLightDirection = GetLightDirection();
	material_->AddConstBufPS(
		{ worldLightDirection.x, worldLightDirection.y, worldLightDirection.z, 0.0f });

	renderer_ = std::make_unique<ModelRenderer>(transform_.modelId, *material_);

	hitEfResId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::EFFECT_HIT).handleId_;
	
	effectInhaleResId_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::EFFECT_INHALE).handleId_;

	stepRotTime_ = TIMEROT_NORMAL;
	effectHitScale_ = 50.0f;
	// �������
	ChangeAnim(ANIM_TYPE::IDLE);
	ChangeState(STATE::PLAY);
}

void Player::Update(void)
{
	// �X�V�X�e�b�v
	stateUpdate_();

	// ���f������X�V
	transform_.Update();

	// �ړ��l������
	DisableAnimMovePow();

	// ����̃t���[���ʒu���擾��������
	leftHandPos_ = MV1GetFramePosition(transform_.modelId, leftHandBoneFrame_);

	// �A�j���[�V�����Đ�
	animationController_->Update();

	// ����ʒu����
	weapon_->WeaponUpdate(transform_, true, true);
}

void Player::Draw(void)
{
	// ���_�F�J�������W
	auto& camera = SceneManager::GetInstance().GetCamera();
	auto cPos = camera.GetPos();
	material_->SetConstBufVS(0, { cPos.x, cPos.y, cPos.z, 0.0f });
	auto lHand = MV1GetFramePosition(transform_.modelId, leftHandBoneFrame_);
	DrawFormatString(0, 0, GetColor(255, 255, 255), "Position: (%0.2f,%0.2f,%0.2f)", transform_.pos.x, transform_.pos.y, transform_.pos.z);// ���_�F�J�������W
	DrawFormatString(0, 40, GetColor(255, 255, 255), "handPosition: (%0.2f,%0.2f,%0.2f)", lHand.x, lHand.y, lHand.z);// ���_�F�J�������W

	// ���f���̕`��
	renderer_->Draw();
	
	// ����̕`��
	weapon_->Draw();
	
	// �ۉe�`��
	DrawShadow();
}

bool Player::IsAttack(void)
{
	return onCol_;
}

void Player::AddCollider(void)
{
	//�v���C���[
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

	//����
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
	
	// �ҋ@�A�j���[�V����
	animationController_->Add("IDLE", path + "Idle.mv1",
		0.0f, NORMAL_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER), true, 0, false);

	// �ړ��n�A�j���[�V����
	animationController_->Add("WALK", path + "Walk.mv1",
		0.0f, NORMAL_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_WALK), true, 0, false);
	animationController_->Add("RUN", path + "Run.mv1",
		0.0f, FAST_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_RUN), true, 0, false);
	
	// �U���n�A�j���[�V����
	animationController_->Add("COMBO_ONE", path + "ComboFirst.mv1",
		0.0f, NORMAL_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_FIRSTCOMBO), false, 0, false);
	animationController_->Add("COMBO_TWO", path + "ComboSecond.mv1",
		0.0f, NORMAL_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_SECONDCOMBO), false, 0, false);
	animationController_->Add("COMBO_THREE", path + "ComboThird.mv1",
		0.0f, NORMAL_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER_THIRDCOMBO), false, 0, false);

	// ���z���A�j���[�V����
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
	// �Ώۃt���[���̃��[�J���s��������l�Ƀ��Z�b�g
	MV1ResetFrameUserLocalMatrix(transform_.modelId, frameNo_);

	// �Ώۃt���[���̃��[�J���s��(�傫���A��]�A�ʒu)���擾
	auto mat = MV1GetFrameLocalMatrix(transform_.modelId, frameNo_);
	auto mScl = MGetSize(mat);
	auto mRot = MGetRotElem(mat);
	auto mPos = MGetTranslateElem(mat);

	// �傫���A��]�A�ʒu�����[�J�����W�ɖ߂�
	MATRIX mix = MGetIdent();
	mix = MMult(mix, MGetScale(mScl));	// �傫��
	mix = MMult(mix, mRot);				// ��]

	// �����������[�J�����W���s��ɐݒ�
	mix = MMult(mix, MGetTranslate({ 0.0f,79.0f,0.0f }));

	// �ړ��l�𖳌���
	MV1SetFrameUserLocalMatrix(transform_.modelId, frameNo_, mix);

}

void Player::ChangeState(STATE state)
{
	// �ȑO�̏�Ԃ�����
	preState_ = state_;

	// ��ԕύX
	state_ = state;

	// �e��ԑJ�ڂ̏�������
	stateChanges_[state_]();

	// �A�j���[�V�����L�[�ɓo�^
	preAnimationKey_ = animationKey_;
	animationKey_ = ANIM_DATA_KEY[(int)state];

	// �A�j���[�V������ύX
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
	//�ړ��ʃ[��
	movePow_ = Utility::VECTOR_ZERO;
	moveDir_ = Utility::VECTOR_ZERO;
	
	// �Փ˔���t���[��
	OnColliderFrame();
	
	if (!isAttack_ && canMove_)
	{
		// �ړ�����
		ProcessMove();

		// �ړ������ɉ�������]
		Rotate();

		// ���̋z������
		isInhale_ = false;

		// �A�j���[�V�������Đ�������
		stopAnim_ = true;
	}
	
	// �U������
	ProcessAttack();

	// ���̋z��
	ProcessInhale();

	// �d�͂ɂ��ړ���
	CalcGravityPow();

	// �Փ˔���
	Collision();
	
	// ��]������
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

	// ���C�e�B���O�𖳌��ɂ���
	SetUseLighting(FALSE);

	// �y�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(TRUE);

	// �e�N�X�`���A�h���X���[�h�� CLAMP �ɂ���( �e�N�X�`���̒[����͒[�̃h�b�g�����X���� )
	SetTextureAddressMode(DX_TEXADDRESS_CLAMP);

	//// �e�𗎂Ƃ����f���̐������J��Ԃ�
	//for (const auto c : collider_)
	//{
	//	// �`�F�b�N���郂�f���́Aj��0�̎��̓X�e�[�W���f���A1�ȏ�̏ꍇ�̓R���W�������f��
	//	ModelHandle = c.lock()->modelId_;

	//	// �v���C���[�̒����ɑ��݂���n�ʂ̃|���S�����擾
	//	HitResDim = MV1CollCheck_Capsule(
	//		ModelHandle, -1,
	//		transform_.pos, VAdd(transform_.pos, { 0.0f, -PLAYER_SHADOW_HEIGHT, 0.0f }), PLAYER_SHADOW_SIZE);

	//	// ���_�f�[�^�ŕω��������������Z�b�g
	//	Vertex[0].dif = GetColorU8(255, 255, 255, 255);
	//	Vertex[0].spc = GetColorU8(0, 0, 0, 0);
	//	Vertex[0].su = 0.0f;
	//	Vertex[0].sv = 0.0f;
	//	Vertex[1] = Vertex[0];
	//	Vertex[2] = Vertex[0];

	//	// ���̒����ɑ��݂���|���S���̐������J��Ԃ�
	//	HitRes = HitResDim.Dim;
	//	for (i = 0; i < HitResDim.HitNum; i++, HitRes++)
	//	{
	//		// �|���S���̍��W�͒n�ʃ|���S���̍��W
	//		Vertex[0].pos = HitRes->Position[0];
	//		Vertex[1].pos = HitRes->Position[1];
	//		Vertex[2].pos = HitRes->Position[2];

	//		// ������Ǝ����グ�ďd�Ȃ�Ȃ��悤�ɂ���
	//		SlideVec = VScale(HitRes->Normal, 0.5f);
	//		Vertex[0].pos = VAdd(Vertex[0].pos, SlideVec);
	//		Vertex[1].pos = VAdd(Vertex[1].pos, SlideVec);
	//		Vertex[2].pos = VAdd(Vertex[2].pos, SlideVec);

	//		// �|���S���̕s�����x��ݒ肷��
	//		Vertex[0].dif.a = 0;
	//		Vertex[1].dif.a = 0;
	//		Vertex[2].dif.a = 0;
	//		if (HitRes->Position[0].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
	//			Vertex[0].dif.a = static_cast<int>(roundf(128.0f * (1.0f - fabs(HitRes->Position[0].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT)));

	//		if (HitRes->Position[1].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
	//			Vertex[1].dif.a = static_cast<int>(roundf(128.0f * (1.0f - fabs(HitRes->Position[1].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT)));

	//		if (HitRes->Position[2].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
	//			Vertex[2].dif.a = static_cast<int>(roundf(128.0f * (1.0f - fabs(HitRes->Position[2].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT)));

	//		// �t�u�l�͒n�ʃ|���S���ƃv���C���[�̑��΍��W���犄��o��
	//		Vertex[0].u = (HitRes->Position[0].x - transform_.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
	//		Vertex[0].v = (HitRes->Position[0].z - transform_.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
	//		Vertex[1].u = (HitRes->Position[1].x - transform_.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
	//		Vertex[1].v = (HitRes->Position[1].z - transform_.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
	//		Vertex[2].u = (HitRes->Position[2].x - transform_.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
	//		Vertex[2].v = (HitRes->Position[2].z - transform_.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;

	//		// �e�|���S����`��
	//		DrawPolygon3D(Vertex, 1, imgShadow_, TRUE);
	//	}

	//	// ���o�����n�ʃ|���S�����̌�n��
	//	MV1CollResultPolyDimTerminate(HitResDim);
	//}

	// ���C�e�B���O��L���ɂ���
	SetUseLighting(TRUE);

	// �y�o�b�t�@�𖳌��ɂ���
	SetUseZBuffer3D(FALSE);
}

void Player::ProcessMove(void)
{
	auto& ins = InputManager::GetInstance();

	// �ړ��ʃ[��
	movePow_ = Utility::VECTOR_ZERO;
	moveDir_ = Utility::VECTOR_ZERO;

	MoveControll();

	if (!Utility::EqualsVZero(moveDir_))
	{
		movePow_ = VScale(moveDir_, moveSpeed_);

		// ��]����
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
	// �ʒu�̓���
	SyncInhaleEffect();

	auto& ins = InputManager::GetInstance();
	// �A�j���[�V�����̍Đ��t���[���擾
	float animInhaleStep = animationController_->GetAnimData("INHALE").stepAnim;
	if (ins.IsPressed(InputManager::ACTION::INHALE))
	{ 
		// �Đ�
		effectInhalePlayId_ = PlayEffekseer3DEffect(effectInhaleResId_);

		// �ړ��ʃ[��
		movePow_ = Utility::VECTOR_ZERO;
		moveDir_ = Utility::VECTOR_ZERO;
		
		// �����Ȃ�����
		canMove_ = false;
		ChangeAnim(ANIM_TYPE::INHALE);
	}
	// ����̃t���[���Œ�~�����A�C�ӂ̃t���[������Đ�
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

	// �W�����v
	if (isHit && (isJump_ || IsEndLanding()))
	{

		if (!isJump_)
		{
			//// �������A�j���[�V����
			//animationController_->Play((int)ANIM_TYPE::JUMP, true, 13.0f, 25.0f);
			//animationController_->SetEndLoop(23.0f, 25.0f, 5.0f);
		}

		isJump_ = true;

		// �W�����v�̓��͎�t���Ԃ��w���X
		stepJump_ += scnMng_.GetDeltaTime();
		if (stepJump_ < TIME_JUMP_IN)
		{
			jumpPow_ = VScale(Utility::DIR_U, POW_JUMP);
		}
	}

	// �{�^���𗣂�����W�����v�͂ɉ��Z���Ȃ�
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
	
		// ��]�s����g�p���āA�x�N�g������]������
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
	
	// �R���{�^�C�}�[�����炷
	if (comboInputTime_ >= 0.0f)
	{
		comboInputTime_ -= deltaTime_;
	}
	else
	{
		// �U�����Z�b�g
		comboStep_ = 0;
		isAttack_ = false;
	}

	// �U���{�^���������ꂽ�ꍇ
	if (ins.IsTriggered(InputManager::ACTION::ATTACK) && !isAttack_)
	{
		// �ŏ��̍U��
		ChangeAnim(ANIM_TYPE::COMBO_ONE);
		comboInputTime_ = INPUT_RECEPTION_TIME;
		comboStep_ = 1;
		isAttack_ = true;
	}
	else if (comboStep_ == 1 && comboInputTime_ >= 0.0f
		&& ins.IsTriggered(InputManager::ACTION::ATTACK) && animationController_->IsEndPlayAnimation())
	{
		// ��i�ڍU��
		ChangeAnim(ANIM_TYPE::COMBO_TWO);
		comboInputTime_ = INPUT_RECEPTION_TIME;
		comboStep_ = 2;
		isAttack_ = true;
	}
 	else if (comboStep_ == 2 && comboInputTime_ >= 0.0f
		&& ins.IsTriggered(InputManager::ACTION::ATTACK) && animationController_->IsEndPlayAnimation())
	{
		// �O�i�ڍU��
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
	
	// �R���{�i�K��������
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
	// �A�j���[�V�����̍Đ��t���[���擾
	float firstComboStep = animationController_->GetAnimData("COMBO_ONE").stepAnim;
	float secondComboStep = animationController_->GetAnimData("COMBO_TWO").stepAnim;
	float thirdComboStep = animationController_->GetAnimData("COMBO_THREE").stepAnim;
	float firstComboTotalStep = animationController_->GetAnimData("COMBO_ONE").animTotalTime;
	float secondComboTotalStep = animationController_->GetAnimData("COMBO_TWO").animTotalTime;
	float thirdComboTotalStep = animationController_->GetAnimData("COMBO_THREE").animTotalTime;

	// ��������Z�b�g
	comboStep_ = COMBOCOUNT::NONE;

	// ���͎�t���Ԃ̃��Z�b�g�Ə������΂�
	comboInputTime_ = INPUT_RECEPTION_TIME;

	// ����͈͊g��̂��߂̒l
	const float judgeComboFrame = 2.0f;

	// ����̃t���[���͈͓��Ŕ��肳��Ă��Ȃ��ꍇ
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

	// �A�j���[�V�����̃��Z�b�g����
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

	// �ʒu�̍X�V
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

	// ���ݐݒ肳��Ă����]�Ƃ̊p�x�������
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// �������l
	if (angleDiff > 0.1)
	{
		stepRotTime_ = TIME_ROT;
	}

	goalQuaRot_ = axis;
}

void Player::Rotate(void)
{
	stepRotTime_ -= scnMng_.GetDeltaTime();

	// ��]�̋��ʕ��
	playerRotY_ = Quaternion::Slerp(
		playerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}

void Player::Collision(void)
{
	// ���ݍ��W���N�_�Ɉړ�����W�����߂�
	movedPos_ = VAdd(transform_.pos, movePow_);

	// �Փ�(�J�v�Z��)
	//CollisionCapsule();
	TestCollisionCapsule();

	// �Փ�(�d��)
	SimpleGravity();

	// �ړ�
	transform_.pos = movedPos_;
}

void Player::SimpleGravity(void)
{
	// �����̏Փ�
	const MV1_COLL_RESULT_POLY& hitFoot =
		playerFootCollsion_->OnCollisionResultPoly_Line(OBJECT_TYPE::STAGE);

	// �����̏Փ˔���
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
	//// �J�v�Z�����ړ�������
	//Transform trans = Transform(transform_);
	//trans.pos = movedPos_;
	//trans.Update();
	//Capsule cap = Capsule(*capsule_, trans);

	//// �J�v�Z���Ƃ̏Փ˔���
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
	//				// �J�v�Z�����ړ�������
	//				trans.pos = movedPos_;
	//				trans.Update();
	//				continue;
	//			}

	//			break;

	//		}

	//	}
	//	// ���o�����n�ʃ|���S�����̌�n��
	//	MV1CollResultPolyDimTerminate(hits);
	//}
}

void Player::TestCollisionCapsule(void)
{
	// �J�v�Z�����ړ�������
	Transform trans = Transform(transform_);
	trans.pos = movedPos_;
	trans.Update();

	isCollisionStage_ = colliderController_->OnCollision(OBJECT_TYPE::STAGE);

	auto& hits =
		colliderController_->OnCollisionResultPoly(OBJECT_TYPE::STAGE);
	hitNormal_ = Utility::VECTOR_ZERO;

	auto& cols = colMng_.GetColliders();
	int pHit = DEFAULT_ZERO;

	// �Փ˂��������̃|���S���ƏՓˉ������܂ŁA
	// �v���C���[�̈ʒu���ړ�������
	for (int i = DEFAULT_ZERO; i < hits.HitNum; i++)
	{
		auto& hit = hits.Dim[i];
		// �n�ʂƈقȂ�A�Փˉ���ʒu���s���Ȃ��߁A���x���ړ�������
		// ���̎��A�ړ�����������́A�ړ��O���W�Ɍ����������ł�������A
		// �Փ˂����|���S���̖@�������������肷��
		for (int tryCnt = DEFAULT_ZERO; tryCnt < 10; tryCnt++)
		{
			// �ēx�A���f���S�̂ƏՓˌ��o����ɂ́A���������߂���̂ŁA
			// �ŏ��̏Փ˔���Ō��o�����Փ˃|���S��1���ƏՓ˔�������

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
				// �@���̕����ɂ�����Ƃ����ړ�������
				movedPos_ = VAdd(movedPos_, VScale(hit.Normal, pushPow));
				// �J�v�Z�����ꏏ�Ɉړ�������
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
	// �A�j���[�V�����̍Đ��t���[���擾
	float firstComboStep = animationController_->GetAnimData("COMBO_ONE").stepAnim;
	float secondComboStep = animationController_->GetAnimData("COMBO_TWO").stepAnim;
	float thirdComboStep = animationController_->GetAnimData("COMBO_THREE").stepAnim;
	// ����t���O�i�t���[�����ƂɃg���K�[�Ǘ��j
	static bool firstColTriggered = false;
	static bool secondColTriggered = false;
	static bool thirdColTriggered = false;
	static bool thirdColSecondStepTriggered = false;

	// ��������Z�b�g
	onCol_ = false;

	// ����͈͊g��̂��߂̒l
	const float judgeComboFrame = 2.0f;

	// ����t���[���͈͓��ŁA�܂����肳��Ă��Ȃ��ꍇ
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

	// �A�j���[�V�����̃��Z�b�g�����i�K�v�ɉ����Ď����j
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
	// �d�͕���
	VECTOR dirGravity = Utility::DIR_D;

	// �d�͂̋���
	float gravityPow =1.0f/*= Stage::DEFAULT_GRAVITY_POW*/;

	// �d��
	VECTOR gravity = VScale(dirGravity, gravityPow);
	jumpPow_ = VAdd(jumpPow_, gravity);

	// �ŏ��͎������Ȃ��B�n�ʂƓ˂������邱�Ƃ��m�F����B
	// ����
	float dot = VDot(dirGravity, jumpPow_);
	if (dot >= 0.0f)
	{
		// �d�͕����Ɣ��Ε���(�}�C�i�X)�łȂ���΁A�W�����v�͂𖳂���
		jumpPow_ = gravity;
	}
}

bool Player::IsEndLanding(void)
{
	bool ret = true;

	// �A�j���[�V�������W�����v�ł͂Ȃ�
	if (animationController_->GetPlayType() != "JUMP")
	{
		return ret;
	}

	// �A�j���[�V�������I�����Ă��邩
	if (animationController_->IsEndPlayAnimation())
	{
		return ret;
	}

	return false;
}