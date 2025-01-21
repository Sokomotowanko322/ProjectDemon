#include <DxLib.h>
#include <vector>
#include <functional>
#include <map>
#include <string>
#include <memory>
#include "../Common/Quaternion.h"
#include "EnemyBase.h"

class Player;
class AnimationController;
class Collider;
class Capsule;
class Camera;
class Player;
class Weapon;
class Soul;

class TestEnemy : public EnemyBase
{
public:
	/*struct Data
	{
		float dissolveHeight_;
		float disMinY_;
		float disMaxY_;
		float dummy;
		float clickedU;
		float clickedV;
		float dummy2[2];
	};*/

	enum class STATE
	{
		IDLE,
		WALK,
		ATTACK,
		DEATH
	};

	// �A�j���[�V�������
	enum class ANIM_TYPE
	{
		IDLE,
		WALK,
		ATTACK,
		DEATH,
		ALL
	};

	//�A�j���[�V����KEY
	std::string ANIM_DATA_KEY[(int)ANIM_TYPE::ALL] =
	{
		"IDLE",
		"WALK",
		"NORMALATTACK",
		"DEATH",
	};


	//// �z��p�Ƀ����_�[�^�[�Q�b�g�����ꂼ��p��
	//enum SHADER_RT_TYPE
	//{
	//	SECOND_RENDERTARGET,
	//	OUTLINE,
	//	FIRST_RENDERTARGET,
	//	NORMAL,
	//	BLUR,
	//	SHRINK,
	//	DEPTH,
	//	MAX
	//};


	// �R���X�g���N�^
	TestEnemy(std::weak_ptr<Player> player);

	// �f�X�g���N�^
	~TestEnemy(void);

	// ������
	void Init(void) override;
	void InitAnimation(void);

	// �X�V
	void Update(void) override;

	// �`��
	void Draw(void) override;

	// �G�̈ʒu��ݒ�
	void SetPos(VECTOR pos);

	// ��ԑJ��
	void ChangeState(STATE state);

	// �_���[�W����
	void SetDamage(int damage);

	// �Փ˔���ɗp������R���C�_����
	void AddCollider(std::weak_ptr<Collider> collider);
	void ClearCollider(void);
	bool CheckCollision(const VECTOR& p1, float r1, const VECTOR& p2, float r2);
	void ResolveCollision(VECTOR& p1, float r1, VECTOR& p2, float r2);
	bool isAlive_;
private:

	// �R���W�����R���g���[���[
	std::unique_ptr<ColliderController> colliderController_;

	std::shared_ptr<Weapon> weapon_;
	 
	// �A�j���[�V�����J�ڗp
	STATE state_;
	STATE preState_;

	// STATE���Ɋi�[����L�[
	std::string animationKey_;
	std::string preAnimationKey_;

	// ���߂̏�Ԏ擾
	std::vector<std::string> currentState_;

	// STATE�̕ύX�A�֐����œ�����UPDATE�ƃA�j���[�V�������Ăяo��
	std::unordered_map<STATE, std::function<void(void)>> stateChange_;
	void ChangeIdle(void);
	void ChangeWalk(void);
	void ChangeAttack(void);
	void ChangeDeath(void);

	// �X�V
	std::function<void(void)> stateUpdate_;
	void UpdateIdle(void);
	void UpdateWalk(void);
	void UpdateAttack(void);
	void UpdateDeath(void);

	void AddCollider(void);

	void JudgeAct(void);

	void ApproachToPlayer(void);
	//// ���f���`��p�p
	//std::unique_ptr<ModelMaterial> material_;
	//std::unique_ptr<ModelRenderer> renderer_;

	// �A�j���[�V��������p
	std::unique_ptr<AnimationController> animationController_;

	// �R���C�_
	std::weak_ptr<Collider> collider_;
	
	// �J�������̎擾
	std::unique_ptr<Camera> camera_;

	//// �f�[�^�����擾�p
	//Data* data_;

	//// �o�b�t�@�i�[�ϐ�
	//int buff_;

	//// �����_�[�^�[�Q�b�g�ݒ�p�ϐ�
	//int renderTarget_[SHADER_RT_TYPE::MAX];

	//// �f�B�]���u���ɌĂяo���e�N�X�`��
	//int dissolveTex_;

	//// �V�F�[�_�t�@�C���i�[
	//int pixelShader_;
	//int vertexShader_;
	//int postEffect_;

	int hp_;

	bool isCatchPlayerPos_;
	bool isEndMove_;
	bool atkFlag_;
	

	// �f���^�^�C��
	float deltaTime_;

	// ��]�ɂ����鎞��
	float rotationStep_;

	// ���̍U���܂ł̃f�B���C
	float attackDelay_;
	float cntDelay_;

	float sinTime_;
	float offset_;
};

