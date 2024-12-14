#pragma once
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
public :

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

	// �A�j���[�V�������
	enum class STATE
	{
		IDLE,
		WALK,
		SLASH,
		DEATH,
	};

	// �R���X�g���N�^
	NormalEnemy(void);

	// �f�X�g���N�^
	~NormalEnemy(void);

	void Init(void) override;
	void InitAnim(void);

	void Update(void) override;
	void Draw(void) override;
	
	// �f�B�]���u�G�t�F�N�g�쐬
	void InitDissolve(void);
	void MakeDissolve(void);

	void SetPos(VECTOR pos);
	
private:

	// ���f���`��p�p
	std::unique_ptr<ModelMaterial> material_;
	std::unique_ptr<ModelRenderer> renderer_;

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