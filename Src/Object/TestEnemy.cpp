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
#include "../Object/Common/Collider/ColliderManager.h"
#include "../Object/Common/Collider/Collider.h"
#include "../Object/Common/Collider/ColliderController.h"
#include "../Object/Common/AnimationController.h"
#include "../Object/Common/Weapon.h"
#include "../Object/Player.h"
#include "../Application.h"
#include "Common/Transform.h"
#include "TestEnemy.h"

const char PATH_VERTEXSHADER[] = "Data/Shader/VertexShader.vso";
const char PATH_PIXELSHADER[] = "Data/Shader/PixelShader.pso";
const char PATH_POSTEFFECT[] = "Data/Shader/PostEffect.pso";
const char DATA_DISSOLVE[] = "Data/Image/Dissolve.png";

const char MODEL_FRAMEARM[] = "mixamorig:LeftHand";

const char IDLE[] = "IDLE";
const char WALK[] = "WALK";
const char NORMAL_ATTACK[] = "NORMALATTACK";
const char DEATH[] = "DEATH";
const char PATH_TEST_ENEMY[] = "Enemy/NormalEnemyAnim";
;

// ���W�n
const VECTOR DEFAULT_POS = { 0.0f, 0.0f, 0.0f };
const VECTOR MODEL_SCALE = { 1.0f, 1.0f, 1.0f };

const VECTOR ENEMY_CAPSULE_TOP = { 0.0f, 90.0f,0.0f };
const VECTOR ENEMY_CAPSULE_BOTTOM = { 0.0f, 0.0f,0.0f };

const int DAMAGE = 5;
const int L_FRAME_NUM = 10;

const float ATTACK_COLFRAME = 76.0f;

const float VIEW_ANGLE = 30.0f;
const float VIEW_RANGE = 3000.0f;
const float HEARING_RANGE = 3000.0f;
const float MELEE_RANGE = 20.0f;
const float FAR_RANGE = 300.0f;
const float DEVIDE_STEPCOUNT = 4.0f;
const float DOT_MIN = 0.99f;

// �d�͗�
const float GRAVITY_POW = 10.0f;

const float WALK_SPEED = 4.0f;
const float NORMAL_ANIM_SPEED = 60.0f;
const float ROTATION_MIN = 0.001f;

const float POLI_HIT_MOVE = 1.0f;
const float ENEMY_CAPSULE_RADIUS = 100.0f;
const float ATTACK_COL_SCALE = 30.0f;
const float ARM_COL_RADIUS = 30.0f;
const VECTOR ARM_COL_VSUB = Utility::VECTOR_ZERO;
const VECTOR ARM_COL_VADD = { 0.0,50.0f,0.0f };

const int MAX_HP = 300;

TestEnemy::TestEnemy(std::weak_ptr<Player> player)
{
	player_ = player;
	hp_ = MAX_HP;

	//�֐��|�C���^�̏�����
	stateChange_.emplace(STATE::IDLE, std::bind(&TestEnemy::ChangeIdle, this));
	stateChange_.emplace(STATE::WALK, std::bind(&TestEnemy::ChangeWalk, this));
	stateChange_.emplace(STATE::ATTACK, std::bind(&TestEnemy::ChangeAttack, this));
	stateChange_.emplace(STATE::DEATH, std::bind(&TestEnemy::ChangeDeath, this));
}

TestEnemy::~TestEnemy(void)
{
	MV1DeleteModel(enemyTransform_.modelId);
}

void TestEnemy::Init(void)
{
	// ���f���̊�{�ݒ�
	enemyTransform_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::NORMAL_ENEMY));
	enemyTransform_.scl = MODEL_SCALE;
	enemyTransform_.quaRot = Quaternion::Euler({ Utility::Deg2RadF(0.0f),Utility::Deg2RadF(180.0f),Utility::Deg2RadF(0.0f) });
	enemyTransform_.pos = DEFAULT_POS;
	diff_ = Utility::VECTOR_ZERO;
	enemyTransform_.Update();

	deltaTime_ = SceneManager::GetInstance().GetDeltaTime();

	weapon_ = std::make_shared<Weapon>();

	armFrame_ = MV1SearchFrame(enemyTransform_.modelId, MODEL_FRAMEARM);

	isAlive_ = true;
	isEndMove_ = false;
	atkFlag_ = false;
	wallCollFlag_ = false;
	isCatchPlayerPos_ = false;
	cntDelay_ = 2.0f;
	attackDelay_ = 0.0f;
	dot_ = 0.0f;
	camera_ = std::make_unique<Camera>();
	
	colliderController_ = std::make_unique<ColliderController>();
	colliderController_->SetCollision(OBJECT_TYPE::ENEMY_MODEL);

	colliderStage_ = std::make_unique<ColliderController>();
	colliderStage_->SetCollision(OBJECT_TYPE::ENEMY_MODEL);


	InitAnimation();
	ChangeState(STATE::IDLE);
}

void TestEnemy::InitAnimation(void)
{
	std::string path = Application::PATH_MODEL + PATH_TEST_ENEMY;
	animationController_ = std::make_unique<AnimationController>(enemyTransform_.modelId);

	// �ҋ@�A�j���[�V����
	animationController_->Add(IDLE, path + "Idle.mv1",
		0.0f, NORMAL_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::NORMAL_ENEMY_IDLE), true, 0, false);
	
	// �ړ��A�j���[�V����
	animationController_->Add(WALK, path + "Walk.mv1",
		0.0f, NORMAL_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::NORMAL_ENEMY_WALK), true, 0, false);
	
	// �U���A�j���[�V����
	animationController_->Add(NORMAL_ATTACK, path + "NormalAttack.mv1",
		0.0f, NORMAL_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::NORMAL_ENEMY_NORMALATTACK), false, 0, false);// �U���A�j���[�V����
	
	// ���S
	animationController_->Add(DEATH, path + "Death.mv1",
		0.0f, NORMAL_ANIM_SPEED, resMng_.LoadModelDuplicate(ResourceManager::SRC::NORMAL_ENEMY_DEATH), false, 0, false);
}

void TestEnemy::Update(void)
{

	//�����Ă�����d�͂�������
	if (enemyTransform_.pos.y >= 0.0f)
	{
		SimpleGravity();
	}

	if (enemyTransform_.pos.y <= 0.0f)
	{
		enemyTransform_.pos.y = 0.0f;
	}

	// ���f������X�V
	enemyTransform_.Update();

	// �֐��|�C���^�X�V
	stateUpdate_();

	// �A�j���[�V�����X�V
	animationController_->Update();

	// ��Ԕ���
	JudgeAct();

	// �Փ˔���
	AddCollider();
	CollisionStage();

	// �v���C���[�ƓG�̏Փˏ���
	ResolveCollision(player_.lock()->GetPos(), 50.0f, enemyTransform_.pos, 50.0f);

	if (hp_ <= 0)
	{
		isAlive_ = false;
		ChangeState(STATE::DEATH);
	}
}

void TestEnemy::Draw(void)
{
	float deltaTime_ = SceneManager::GetInstance().GetDeltaTime();

	MV1DrawModel(enemyTransform_.modelId);
	
	VECTOR pos = MV1GetFramePosition(enemyTransform_.modelId, armFrame_);
	// ���̕`��
	DrawSphere3D(pos, 50.0f, 8, GetColor(255, 255, 0), GetColor(255, 255, 255), false);
	// ���݂�STATE��\������
	/*DrawFormatString(0, 90, GetColor(255, 255, 255), "Current STATE: %d", state_);
	DrawFormatString(0, 140, GetColor(255, 255, 255), "enemyPosition: (%0.2f,%0.2f,%0.2f)",
		enemyTransform_.pos.x, enemyTransform_.pos.y, enemyTransform_.pos.z);*/

}

void TestEnemy::SetPos(VECTOR pos)
{
	enemyTransform_.pos = pos;
	enemyTransform_.Update();
}

VECTOR TestEnemy::GetPos(void)
{
	return enemyTransform_.pos;
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

void TestEnemy::SetDamage(int damage)
{
	hp_ -= damage;
	if (hp_ <= 0)
	{
		ChangeState(STATE::DEATH);
		return;
	}
}

int TestEnemy::GetHp(void)
{
	return hp_;
}

bool TestEnemy::IsAttack(void)
{
	return atkFlag_;
}

bool TestEnemy::IsAlive(void)
{
	return isAlive_;
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

void TestEnemy::ChangeDeath(void)
{
	stateUpdate_ = std::bind(&TestEnemy::UpdateDeath, this);
}

void TestEnemy::UpdateIdle(void)
{
	// ��������]������
	rotationStep_ += ROTATION_MIN;
	attackDelay_ += SceneManager::GetInstance().GetDeltaTime();

	// �v���C���[�̍��W���擾
	VECTOR pPos = player_.lock()->GetPos();

	// �G�l�~�[����v���C���[�܂ł̃x�N�g��
	diff_ = VSub(pPos, enemyTransform_.pos);
	diff_ = VNorm(diff_);
	
	// ��]����
	VECTOR fixedDir = { -diff_.x, diff_.y, -diff_.z };
	
	// �v���C���[�̃x�N�g���Ǝ����̑O�����x�N�g���Ƃ̍���(����)
	dot_ = VDot(diff_, enemyTransform_.GetForward());
	
	// ���ʕ�Ԃ��s��
	enemyTransform_.quaRot.x = 0.0f;
	enemyTransform_.quaRot.z = 0.0f;
	rot_ = Quaternion::Slerp(
		enemyTransform_.quaRot, Quaternion::LookRotation(fixedDir), rotationStep_ / DEVIDE_STEPCOUNT);
	enemyTransform_.quaRot = rot_;
	
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
	enemyTransform_.quaRot.x = 0.0f;
	enemyTransform_.quaRot.z = 0.0f;
	float stepAnim = animationController_->GetAnimData(NORMAL_ATTACK).stepAnim;
	float totalAtkAnim = animationController_->GetAnimData(NORMAL_ATTACK).animTotalTime;
	if (stepAnim >= ATTACK_COLFRAME)
	{
		onColFlag_ = true;
	}
	if (onColFlag_)
	{
		//�R���W�����Z�b�g
		colliderController_->SetCollision(OBJECT_TYPE::ENEMY_MODEL);
		colliderController_->SetCollisionSub(ENEMY_SUBTYPE::HAND_L);

		VECTOR pos = MV1GetFramePosition(enemyTransform_.modelId, armFrame_);


		const auto& hits = /*colliderController_->OnCollision(OBJECT_TYPE::PLAYER)*/
			HitCheck_Sphere_Capsule(pos, ARM_COL_RADIUS, VSub(player_.lock()->GetPos(), ARM_COL_VSUB),
			VAdd(player_.lock()->GetPos(), ARM_COL_VADD), 50.0f);

		const auto& hitpoly = colliderController_->OnCollisionResultPoly(OBJECT_TYPE::PLAYER);

		auto hitDir = HitCheck_Sphere_Capsule(pos, ARM_COL_RADIUS, VSub(player_.lock()->GetPos(), ARM_COL_VSUB),
			VAdd(player_.lock()->GetPos(), ARM_COL_VADD) ,25.0f);

		bool invincible = player_.lock()->GetInvincibility();

		//�v���C���[�ɍU��������������
		if (hits && !invincible)
		{
			//�_���[�W��^����
			player_.lock()->SetDamage(DAMAGE);
			player_.lock()->SetInvincibility(true);
		}
	}

	if (stepAnim >= totalAtkAnim)
	{
		ChangeState(STATE::IDLE);
	}
}

void TestEnemy::UpdateDeath(void)
{
	isAlive_ = false;

	if(!isAlive_)
	{
		animationController_->ChangeAnimation("DEATH");

		return;
	}
}

void TestEnemy::AddCollider(void)
{
	// �G���f��
	colMng_.AddCollider(
		OBJECT_TYPE::ENEMY_MODEL,
		COL_TYPE::CAPSULE,
		enemyTransform_,
		true,
		0, 0,
		ENEMY_SUBTYPE::NONE,
		ENEMY_CAPSULE_RADIUS,
		ENEMY_CAPSULE_TOP,
		ENEMY_CAPSULE_BOTTOM
	);
	//����
	colMng_.AddCollider(
		OBJECT_TYPE::ENEMY_MODEL, 
		COL_TYPE::SPHERE,
		enemyTransform_,
		true,
		-1,
		L_FRAME_NUM, 
		ENEMY_SUBTYPE::HAND_L, 
		ARM_COL_RADIUS);

}

void TestEnemy::CollisionStage(void)
{
	const auto& hitStage = colliderStage_->OnCollision(OBJECT_TYPE::STAGE);
	wallCollFlag_ = hitStage;
	const auto& hits = colliderStage_->OnCollisionResultPoly(OBJECT_TYPE::STAGE);
	auto& colMng = scnMng_.GetColManager();
	int tgHit = 0;

	if (wallCollFlag_)
	{
		for (int i = 0; i < hits.HitNum; i++)
		{
			auto& hit = hits.Dim[i];
			// �n�ʂƈقȂ�A�Փˉ���ʒu���s���Ȃ��߁A���x���ړ�������
			// ���̎��A�ړ�����������́A�ړ��O���W�Ɍ����������ł�������A
			// �Փ˂����|���S���̖@�������������肷��
			for (int tryCnt = 0; tryCnt < 10; tryCnt++)
			{

				// �ēx�A���f���S�̂ƏՓˌ��o����ɂ́A���������߂���̂ŁA
				// �ŏ��̏Փ˔���Ō��o�����Փ˃|���S��1���ƏՓ˔�������

				for (const auto& col : colMng.GetColliders())
				{
					if (col->GetObjType() != OBJECT_TYPE::ENEMY_MODEL) { continue; }

					tgHit = HitCheck_Sphere_Triangle(
						col->GetPos(),
						col->GetRadius(),
						hit.Position[0], hit.Position[1], hit.Position[2]);
					break;

				}

				if (tgHit)
				{
					// �@���̕����ɂ�����Ƃ����ړ�������
					movePos_ = VAdd(movePos_, VScale(hit.Normal, POLI_HIT_MOVE));
					// �J�v�Z�����ꏏ�Ɉړ�������
					enemyTransform_.pos = VAdd(enemyTransform_.pos, movePos_);
					enemyTransform_.Update();

					continue;
				}
				break;
			}
		}
	}
}

void TestEnemy::SimpleGravity(void)
{
	// �����̏Փ�
	const MV1_COLL_RESULT_POLY& hitFoot =
		colliderController_->OnCollisionResultPoly_Line(OBJECT_TYPE::STAGE);

	// �����̏Փ˔���
	if (hitFoot.HitFlag)
	{
		gravity_ = 0.0f;
		enemyTransform_.pos.y = hitFoot.HitPosition.y;
		return;
	}

	gravity_ += GRAVITY_POW;

	if (enemyTransform_.pos.y >= 0)
	{
		enemyTransform_.pos.y += gravity_;
	}
}

void TestEnemy::JudgeAct(void)
{
	// �v���C���[�̍��W���擾
	VECTOR pPos = player_.lock()->GetPos();

	// �G�l�~�[����v���C���[�܂ł̃x�N�g��
	VECTOR diff = VSub(pPos, enemyTransform_.pos);

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
	VECTOR diff = VSub(pPos, enemyTransform_.pos);

	// XZ����
	float distance = diff.x * diff.x + diff.z * diff.z;

	VECTOR vec;
	vec = VSub(pPos, enemyTransform_.pos);
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
		float viewRad = rad - enemyTransform_.rot.y;
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
	enemyTransform_.pos.x += (movePow.x);
	enemyTransform_.pos.z += (movePow.z);
	enemyTransform_.pos.y = 0.0f;
	
	// ��]����
	VECTOR fixedDirection = { -direction.x, direction.y, -direction.z };
	
	// ���ʕ�Ԃ��s��
	enemyTransform_.quaRot.x = 0.0f;
	enemyTransform_.quaRot.z = 0.0f;
	rot_ = Quaternion::Slerp(
	enemyTransform_.quaRot, Quaternion::LookRotation(diff), rotationStep_ / DEVIDE_STEPCOUNT);
	enemyTransform_.quaRot = rot_;
	enemyTransform_.quaRot = Quaternion::LookRotation(fixedDirection);

}

bool CheckRectCircleCollision(const Vector2& rectPos, float rectWidth, 
	float rectHeight, const Vector2& circlePos, float circleRadius) 
{
	// ��`�̒����ʒu����ł��߂��_���v�Z
	int closestX = std::clamp(static_cast<int>(circlePos.x), 
		static_cast<int>(rectPos.x), static_cast<int>(rectPos.x + rectWidth));
	int closestY = std::clamp(static_cast<int>(circlePos.y)
		, static_cast<int>(rectPos.y), static_cast<int>(rectPos.y + rectHeight));

	// ��`�Ɖ~�̍ŒZ�������v�Z
	float distanceX = closestX - circlePos.x;
	float distanceY = closestY - circlePos.y;

	return (distanceX * distanceX + distanceY * distanceY) <= (circleRadius * circleRadius);
}

bool TestEnemy::CheckCollision(const VECTOR& p1, float r1, const VECTOR& p2, float r2)
{
	// ���W�ŏՓ˂𔻒f����
	float dx = p1.x - p2.x;
	float dy = p1.y - p2.y;
	float dz = p1.z - p2.z;
	float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
	return distance < (r1 + r2);
}

void TestEnemy::ResolveCollision(VECTOR& p1, float r1, VECTOR& p2, float r2)
{
	if (CheckCollision(p1, r1, p2, r2))
	{
		// �Փˉ����̕������v�Z
		float dx = p1.x - p2.x;
		float dy = p1.y - p2.y;
		float dz = p1.z - p2.z;
		float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
		if (distance == 0.0f) return;

		float overlap = (r1 + r2) - distance;
		// ���K�����������x�N�g��
		float nx = dx / distance;
		float ny = dy / distance;
		float nz = dz / distance;

		// �ʒu�𒲐�
		p1.x += nx * (overlap / 1.0f);
		p1.y += ny * (overlap / 1.0f);
		p1.z += nz * (overlap / 1.0f);
		p2.x -= nx * (overlap / 1.0f);
		p2.y -= ny * (overlap / 1.0f);
		p2.z -= nz * (overlap / 1.0f);
	}
}