#pragma once
#include <memory>
#include <vector>
#include <map>
#include <functional>
#include <DxLib.h>
#include "../Object/Common/Collider/Collision.h"
#include "ActorBase.h"

class AnimationController;
class ColliderController;
class Collider;
class Capsule;
class ModelMaterial;
class ModelRenderer;
class TestEnemy;
class Weapon;

class Player : public ActorBase
{

public:

	// �v���C���̑J�ڏ��
	enum class STATE
	{
		NONE,
		PLAY,
		INHALE,
		HIT,
		WARP_RESERVE,
		WARP_MOVE,
		DEAD,
		VICTORY,
		END
	};

	// �A�j���[�V�������
	enum class ANIM_TYPE
	{
		IDLE,
		WALK,
		RUN,
		COMBO_ONE,
		COMBO_TWO,
		COMBO_THREE,
		INHALE,
		COUNTER,
		WARP_PAUSE,
		FLY,
		FALLING,
		VICTORY,
		ALL
	};

	//�A�j���[�V����KEY
	std::string ANIM_DATA_KEY[(int)ANIM_TYPE::ALL] =
	{
		"IDLE",
		"WALK",
		"RUN",
		"COMBO_ONE",
		"COMBO_TWO",
		"COMBO_THREE",
		"INHALE",
		"COUNTER",
	};

	enum COMBOCOUNT
	{
		NONE,
		FIRSTSTEP,
		SECONDSTEP,
		THIRDSTEP
	};

	// �R���X�g���N�^
	Player(void);

	// �f�X�g���N�^
	~Player(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	// �_���[�W����
	void SetDamage(int damage);

	// �U�����Ă��邩
	bool IsAttack(void);

	// �Փ˔���ɗp������R���C�_����
	void AddCollider(void);
	void SetCollider(void);
	void ClearCollider(void);

	// �q�b�g���G�t�F�N�g
	void HitEffect(VECTOR pos);

	int GetHp(void);
	int GetMaxHp(void);

	// �Փ˗p�J�v�Z���̎擾
	const Capsule& GetCapsule(void) const;
	
	// �Q�b�^�[
	const ANIM_TYPE GetNowAnim(void) const;
	VECTOR& GetLeftHandPos(void);
	VECTOR& GetPos(void);
	
	// ���G��Ԃ̐ݒ�
	void SetInvincibility(bool invincibility);
	bool GetInvincibility(void);

private:

	// �A�j���[�V����
	std::unique_ptr<AnimationController> animationController_;
	// �u�����h�p
	std::string animationKey_;
	std::string preAnimationKey_;

	// ����
	std::shared_ptr<Weapon> weapon_;
	
	// �G
	std::weak_ptr<TestEnemy> enemy_;
	
	// �z�����G�t�F�N�g
	VECTOR effectInhalePos_;
	int effectInhaleResId_;
	int effectInhalePlayId_;
	
	// �q�b�g���G�t�F�N�g
	int hitEfResId_;
	int hitEfPlayId_;
	float effectHitScale_;
	
	// ����̒Ǐ]�ʒu
	int rightHandBoneIndex_;
	int leftHandBoneFrame_;
	
	// �J�����̒Ǐ]�ʒu
	int hipsBoneIndex_;

	// �u�����h�p�t���[���ԍ�
	int frameNo_;

	// ���߂̏�Ԏ擾
	std::vector<std::string> currentState_;

	// ��ԊǗ�
	STATE state_;
	STATE preState_;
	
	// �A�j���[�V�������
	ANIM_TYPE animType_;

	// ��ԊǗ�(��ԑJ�ڎ���������)
	std::map<STATE, std::function<void(void)>> stateChanges_;
	// ��ԊǗ�(�X�V�X�e�b�v)
	std::function<void(void)> stateUpdate_;

	// ���f���`��p�p
	std::unique_ptr<ModelMaterial> material_;
	std::unique_ptr<ModelRenderer> renderer_;

	// �ړ�����
	VECTOR moveDir_;

	// �ړ���
	VECTOR movePow_;

	// �ړ���̍��W
	VECTOR movedPos_;

	// ����̍��W
	VECTOR leftHandPos_;

	// �ړ��X�s�[�h
	float moveSpeed_;
	float speedBuffRate_;
	int hp_;
	int maxHp_;
	;
	bool hitGround_;
	bool invincibility_;
	
	//�����Փ˂��Ă���n�ʃ|���S���̖@��
	VECTOR hitNormal_;

	//�����Փ˂��Ă���n�ʃ|���S���̈ʒu
	VECTOR hitPos_;

	// ��]
	Quaternion playerRotY_;
	Quaternion goalQuaRot_;
	float stepRotTime_;
	float rotRad_;

	// �W�����v��
	VECTOR jumpPow_;

	// �W�����v����
	bool isJump_;
	// �z������
	bool isInhale_;

	// �U������
	bool canMove_;
	bool isAttack_;
	bool onCol_;
	bool firstComboTriggered_;
	bool secondComboTriggered_;
	bool thirdComboTriggered_;

	// �U���{��
	float atkRate_;			

	// �A�j���[�V��������
	bool stopAnim_;

	// �X�e�[�W�Ƃ̏Փ�
	bool isCollisionStage_;

	// ���͎�t����
	float stepJump_;
	float comboInputTime_;
	float deltaTime_;
	int comboStep_;

	// �Փ˔���ɗp������R���C�_
	std::vector<std::weak_ptr<Collider>> collider_;
	std::unique_ptr<ColliderController> colliderController_;
	std::unique_ptr<ColliderController> playerFootCollsion_;
	std::unique_ptr<Capsule> capsule_;

	// �Փ˃`�F�b�N
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

	// �ۉe
	int imgShadow_;

	// �g�D�[���}�b�v
	int imgToonMap_;

	// �d��
	float gravity_;

	// �A�j���[�V��������
	void InitAnimation(void);
	void BlendAnimation(void);
	
	//�A�j���[�V��������
	void ChangeAnim(ANIM_TYPE state);

	// �A�j���[�V�����ړ��n������
	void DisableAnimMovePow(void);

	// ��ԑJ��
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStatePlay(void);
	void ChangeStateInhale(void);
	
	// �X�V�X�e�b�v
	void UpdateNone(void);
	void UpdatePlay(void);
	void UpdateInhale(void);
	
	// �e�`��
	void DrawShadow(void);

	// ����
	void ProcessMove(void);
	void ProcessInhale(void);
	void ProcessCounter(void);
	void CheckCounter(void);
	void MoveControll(void);

	// �U��
	void ProcessAttack(void);
	void ResetCombo(void);
	void OnColliderFrame(void);

	// �z���G�t�F�N�g
	void SyncInhaleEffect(void);

	// �s������
	bool IsEndDodge(void);
	bool IsEndGuard(void);
	
	// ��]
	void SetGoalRotate(double rotRad);
	void Rotate(void);

	// �Փ˔���
	void Collision(void);
	void SimpleGravity(void);
	void CollisionCapsule(void);
	void TestCollisionCapsule(void);

	// �ړ��ʂ̌v�Z
	void CalcGravityPow(void);

	// ���n���[�V�����I��
	bool IsEndLanding(void);
};
