#include <vector>
#include <memory>
#include "../Common/Quaternion.h"
#include "EnemyBase.h"

class AnimationController;
class Collider;
class Capsule;
class Camera;
class Player;
class Soul;
class ModelMaterial;
class ModelRenderer;

class NormalEnemy : public EnemyBase
{
public:

	struct Data
	{
		float dissolveHeight_;
		float disMinY_;
		float disMaxY_;
		float dummy;
		float clickedU;
		float clickedV;
		float dummy2[2];
	};

	enum class STATE
	{
		THINK,
		MOVE,
		ATTACK
	};

	// �A�j���[�V�������
	enum class ANIM_TYPE
	{
		IDLE,
		WALK,
		RUN,
		ATTACK,
		ALL
	};

	//�A�j���[�V����KEY
	std::string ANIM_DATA_KEY[(int)ANIM_TYPE::ALL] =
	{
		"IDLE",
		"WALK",
		"RUN",
		"ATTACK",
	};


	// �z��p�Ƀ����_�[�^�[�Q�b�g�����ꂼ��p��
	enum SHADER_RT_TYPE
	{
		SECOND_RENDERTARGET,
		OUTLINE,
		FIRST_RENDERTARGET,
		NORMAL,
		BLUR,
		SHRINK,
		DEPTH,
		MAX
	};


	// �R���X�g���N�^
	NormalEnemy(void);

	// �f�X�g���N�^
	~NormalEnemy(void);

	// ������
	void Init(void) override;
	void InitAnimation(void);
	void InitDissolve(void);

	// �X�V
	void Update(void) override;

	// �`��
	void Draw(void) override;

	// �f�B�]���u�G�t�F�N�g�쐬
	void MakeDissolve(void);

	// �G�̈ʒu��ݒ�
	void SetPos(VECTOR pos);

	// ��ԑJ��
	void ChangeState(STATE state);

private:

	// �A�j���[�V�����J�ڗp
	STATE state_;
	STATE preState_;

	// STATE���Ɋi�[����L�[
	std::string animationKey_;
	std::string preAnimationKey_;

	// STATE�̕ύX�A�֐����œ�����UPDATE�ƃA�j���[�V�������Ăяo��
	std::unordered_map<STATE, std::function<void(void)>> stateChange_;
	void ChangeThink(void);
	void ChangeMove(void);
	void ChangeAttack(void);

	// �X�V
	std::function<void(void)> stateUpdate_;
	void UpdateThink(void);
	void UpdateMove(void);
	void UpdateAttack(void);

	// ���f���`��p�p
	std::unique_ptr<ModelMaterial> material_;
	std::unique_ptr<ModelRenderer> renderer_;

	// �A�j���[�V��������p
	std::unique_ptr<AnimationController> animationController_;

	// ���̏��擾
	//std::shared_ptr<Soul> soul_;

	// �J�������̎擾
	std::unique_ptr<Camera> camera_;

	// �f�[�^�����擾�p
	Data* data_;

	// �o�b�t�@�i�[�ϐ�
	int buff_;

	// �����_�[�^�[�Q�b�g�ݒ�p�ϐ�
	int renderTarget_[SHADER_RT_TYPE::MAX];

	// �f�B�]���u���ɌĂяo���e�N�X�`��
	int dissolveTex_;

	// �V�F�[�_�t�@�C���i�[
	int pixelShader_;
	int vertexShader_;
	int postEffect_;

	// �f���^�^�C��
	float deltaTime;

	float sinTime_;
	float offset_;
};