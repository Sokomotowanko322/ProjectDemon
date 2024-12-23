#include <EffekseerForDXLib.h>
#include <DxLib.h>
#include <cmath>
#include <utility>
#include <cassert>
#include <deque>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <array>
#include <vector>
#include "../Utility/Utility.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Resource.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Renderer/ModelMaterial.h"
#include "../Renderer/ModelRenderer.h"
#include "../Object/Common/AnimationController.h"
#include "../Object/Player.h"
#include "../Application.h"
#include "Common/Transform.h"
#include "TestEnemy.h"

const char PATH_VERTEXSHADER[] = "Data/Shader/VertexShader.vso";
const char PATH_PIXELSHADER[] = "Data/Shader/PixelShader.pso";
const char PATH_POSTEFFECT[] = "Data/Shader/PostEffect.pso";
const char DATA_DISSOLVE[] = "Data/Image/Dissolve.png";

const char IDLE[] = "IDLE";
const char WALK[] = "WALK";
const char NORMAL_ATTACK[] = "NORMALATTACK";
const char PATH_TEST_ENEMY[] = "Enemy/TestEnemyAnim";
;

// ���W�n
const VECTOR DEFAULT_POS = { 0.0f, 0.0f, 0.0f };
const VECTOR MODEL_SCALE = { 1.0f, 1.0f, 1.0f };

const float VIEW_ANGLE = 30.0f;
const float VIEW_RANGE = 3000.0f;
const float HEARING_RANGE = 3000.0f;
const float MELEE_RANGE = 20.0f;
const float FAR_RANGE = 300.0f;
const float DEVIDE_STEPCOUNT = 4.0f;
const float DOT_MIN = 0.99f;

const float WALK_SPEED = 4.0f;
const float NORMAL_ANIM_SPEED = 60.0f;
const float ROTATION_MIN = 0.001f;

//bool searching_ = false;


TestEnemy::TestEnemy(std::weak_ptr<Player> player)
/*: state_(STATE::IDLE)*/
{
	player_ = player;

	//�֐��|�C���^�̏�����
	stateChange_.emplace(STATE::IDLE, std::bind(&TestEnemy::ChangeIdle, this));
	stateChange_.emplace(STATE::WALK, std::bind(&TestEnemy::ChangeWalk, this));
	stateChange_.emplace(STATE::ATTACK, std::bind(&TestEnemy::ChangeAttack, this));
}

TestEnemy::~TestEnemy(void)
{
}

void TestEnemy::Init(void)
{
	// ���f���̊�{�ݒ�
	enmyTransform_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::NORMAL_ENEMY));
	enmyTransform_.scl = MODEL_SCALE;
	enmyTransform_.quaRot = Quaternion::Euler({ Utility::Deg2RadF(0.0f),Utility::Deg2RadF(180.0f),Utility::Deg2RadF(0.0f) });
	enmyTransform_.pos = DEFAULT_POS;
	diff_ = Utility::VECTOR_ZERO;
	enmyTransform_.Update();

	deltaTime_ = SceneManager::GetInstance().GetDeltaTime();

	isEndMove_ = false;
	atkFlag_ = false;
	isCatchPlayerPos_ = false;
	cntDelay_ = 2.0f;
	attackDelay_ = 0.0f;
	dot_ = 0.0f;
	camera_ = std::make_unique<Camera>();

	InitAnimation();
	ChangeState(STATE::IDLE);
}

void TestEnemy::InitAnimation(void)
{
	std::string path = Application::PATH_MODEL + PATH_TEST_ENEMY;
	animationController_ = std::make_unique<AnimationController>(enmyTransform_.modelId);

	// �ҋ@�A�j���[�V����
	animationController_->Add(IDLE, path + "Idle.mv1",
		0.0f, NORMAL_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::NORMAL_ENEMY_IDLE), true, 0, false);
	
	// �ړ��A�j���[�V����
	animationController_->Add(WALK, path + "Walk.mv1",
		0.0f, NORMAL_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::NORMAL_ENEMY_WALK), true, 0, false);
	/*animationController_->Add("RUN", path + "Idle.mv1",
		0.0f, NORMAL_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::NORMAL_ENEMY), true, 0, false);*/
	
	// �U���A�j���[�V����
	animationController_->Add(NORMAL_ATTACK, path + "NormalAttack.mv1",
		0.0f, NORMAL_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::NORMAL_ENEMY_NORMALATTACK), false, 0, false);
}

void TestEnemy::Update(void)
{
	// ��Ԕ���
	JudgeAct();

	// ���f������X�V
	enmyTransform_.Update();

	// �֐��|�C���^�X�V
	stateUpdate_();

	// �A�j���[�V�����X�V
	animationController_->Update();
}

void TestEnemy::Draw(void)
{
	float deltaTime_ = SceneManager::GetInstance().GetDeltaTime();

	MV1DrawModel(enmyTransform_.modelId);
	// ���݂�STATE��\������
	DrawFormatString(0, 90, GetColor(255, 255, 255), "Current STATE: %d", state_);

}

void TestEnemy::SetPos(VECTOR pos)
{
	enmyTransform_.pos = pos;
	enmyTransform_.Update();
}

void TestEnemy::ChangeState(STATE state)
{
	preState_ = state_;

	state_ = state;

	stateChange_[state_]();

	preAnimationKey_ = animationKey_;

	animationKey_ = ANIM_DATA_KEY[(int)state];

	animationController_->ChangeAnimation(animationKey_);
}

void TestEnemy::ChangeIdle(void)
{
	stateUpdate_ = std::bind(&TestEnemy::UpdateIdle, this);
}

void TestEnemy::ChangeWalk(void)
{
	stateUpdate_ = std::bind(&TestEnemy::UpdateWalk, this);
}

void TestEnemy::ChangeAttack(void)
{
	stateUpdate_ = std::bind(&TestEnemy::UpdateAttack, this);
}

void TestEnemy::UpdateIdle(void)
{
	// ��������]������
	rotationStep_ += ROTATION_MIN;
	attackDelay_ += SceneManager::GetInstance().GetDeltaTime();

	// �v���C���[�̍��W���擾
	VECTOR pPos = player_.lock()->GetPos();

	// �G�l�~�[����v���C���[�܂ł̃x�N�g��
	diff_ = VSub(pPos, enmyTransform_.pos);
	diff_ = VNorm(diff_);
	
	// ��]����
	VECTOR fixedDir = { -diff_.x, diff_.y, -diff_.z };
	
	// �v���C���[�̃x�N�g���Ǝ����̑O�����x�N�g���Ƃ̍���(����)
	dot_ = VDot(diff_, enmyTransform_.GetForward());
	
	// ���ʕ�Ԃ��s��
	enmyTransform_.quaRot.x = 0.0f;
	enmyTransform_.quaRot.z = 0.0f;
	rot_ = Quaternion::Slerp(
		enmyTransform_.quaRot, Quaternion::LookRotation(fixedDir), rotationStep_ / DEVIDE_STEPCOUNT);
	enmyTransform_.quaRot = rot_;
	
	// ����������Ȃ�1�ɋ߂�������WALK���Ȃ�
	if (dot_ <= DOT_MIN)
	{
		// �v���C���[�̕����������܂ł�WALK������
		animationController_->ChangeAnimation(WALK);
	}
	else
	{
		animationController_->ChangeAnimation(IDLE);
	}

	if (attackDelay_ >= cntDelay_)
	{
	/*	onColBreathFlag_ = false;
		onColTackleFlag_ = false;
		onColBiteFlag_ = false;*/
		isCatchPlayerPos_ = false;
		atkFlag_ = false;
		attackDelay_ = 0.0f;
		rotationStep_ = 0.0f;
	}
}

void TestEnemy::UpdateWalk(void)
{
}

void TestEnemy::UpdateAttack(void)
{
	atkFlag_ = true;
	enmyTransform_.quaRot.x = 0.0f;
	enmyTransform_.quaRot.z = 0.0f;
	float stepAnim = animationController_->GetAnimData(NORMAL_ATTACK).stepAnim;
	float totalBiteAnim = animationController_->GetAnimData(NORMAL_ATTACK).animTotalTime;
	//if (stepAnim >= BITE_COLFRAME)
	//{
	//	onColBiteFlag_ = true;
	//}
	//if (onColBiteFlag_)
	//{
	//	//�R���W�����Z�b�g
	//	colliderController_->SetCollision(OBJECT_TYPE::ENEMY_VOL);
	//	colliderController_->SetCollisionSub(ENEMY_SUBTYPE::HEAD);

	//	const auto& hits = colliderController_->OnCollision(OBJECT_TYPE::PLAYER);
	//	const auto& hitpoly = colliderController_->OnCollisionResultPoly(OBJECT_TYPE::PLAYER);

	//	auto hitDir = VNorm(VSub(player_.lock()->GetPos(), transform_.pos));
	//	bool muteki = player_.lock()->GetInvincibility();
	//	//�v���C���[�ɍU��������������
	//	if (hits && !muteki)
	//	{
	//		player_.lock()->IsImpact(Player::PATTERN_NUM::TWO);
	//		//�v���C���[���K�[�h���Ȃ�������
	//		if (player_.lock()->GetSTATE() != Player::STATE::BLOCK)
	//		{
	//			//�_���[�W��^����
	//			player_.lock()->Damage(BITING_DAMAGE);
	//			player_.lock()->SetHitDir(hitDir, BITE_HITPOW);
	//			player_.lock()->SetInvincibility(true);
	//		}
	//	}
	//	else if (hits && player_.lock()->GetSTATE() != Player::STATE::HIT)
	//	{
	//		player_.lock()->SetIsSlow(true);
	//	}
	//}

	if (stepAnim >= totalBiteAnim)
	{
		ChangeState(STATE::IDLE);
	}
}

void TestEnemy::JudgeAct(void)
{
	// �v���C���[�̍��W���擾
	VECTOR pPos = player_.lock()->GetPos();

	// �G�l�~�[����v���C���[�܂ł̃x�N�g��
	VECTOR diff = VSub(pPos, enmyTransform_.pos);

	// XZ����
	float distance = diff.x * diff.x + diff.z * diff.z;

	// �������������炱�����֋߂Â��Ă���
	if (distance >= FAR_RANGE * FAR_RANGE && !atkFlag_)
	{
		ApproachToPlayer();
	}
	else if(distance >= MELEE_RANGE * MELEE_RANGE
		&& !atkFlag_)
	{
		ChangeState(STATE::ATTACK);
	}
}

void TestEnemy::ApproachToPlayer(void)
{
	// ���m�t���O���Z�b�g
	isCatchPlayerPos_= false;

	// �v���C���[�̍��W���擾
	VECTOR pPos = player_.lock()->GetPos();

	// �G�l�~�[����v���C���[�܂ł̃x�N�g��
	VECTOR diff = VSub(pPos, enmyTransform_.pos);

	// XZ����
	float distance = diff.x * diff.x + diff.z * diff.z;

	VECTOR vec;
	vec = VSub(pPos, enmyTransform_.pos);
	VECTOR direction = VNorm(vec);

	// ���o
	if (distance <= HEARING_RANGE * HEARING_RANGE)
	{
		isCatchPlayerPos_ = true;
		ChangeState(STATE::WALK);
	}

	// ���o
	if (distance <= VIEW_RANGE * VIEW_RANGE)
	{

		// �������猩���v���C���[�̊p�x�����߂�
		float rad = atan2f(vec.z, vec.x);
		float viewRad = rad - enmyTransform_.rot.y;
		float viewDeg = static_cast<float>(Utility::DegIn360(Utility::Rad2DegF(viewRad)));

		// ����p
		if (viewDeg <= VIEW_ANGLE || viewDeg >= (360.0f - VIEW_ANGLE))
		{
			isCatchPlayerPos_ = true;
		}
	}
	// �ړ�����W���v�Z����
	// �ړ���
	VECTOR movePow;
	movePow.x = direction.x * WALK_SPEED;
	movePow.z = direction.z * WALK_SPEED;

	// �ړ�����(���W+�ړ���)
	enmyTransform_.pos.x += (movePow.x);
	enmyTransform_.pos.z += (movePow.z);
	enmyTransform_.pos.y = 0.0f;
	// ��]����
	VECTOR fixedDirection = { -direction.x, direction.y, -direction.z };
	// ���ʕ�Ԃ��s��
	enmyTransform_.quaRot.x = 0.0f;
	enmyTransform_.quaRot.z = 0.0f;
	rot_ = Quaternion::Slerp(
		enmyTransform_.quaRot, Quaternion::LookRotation(diff), rotationStep_ / DEVIDE_STEPCOUNT);
	enmyTransform_.quaRot = rot_;
	enmyTransform_.quaRot = Quaternion::LookRotation(fixedDirection);

}

void TestEnemy::AddCollider(std::weak_ptr<Collider> collider)
{
	colliders_.push_back(collider);
}

void TestEnemy::ClearCollider(void)
{
	colliders_.clear();
}