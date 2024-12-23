#pragma once
#include "../Common/Quaternion.h"
#include "ActorBase.h"
#include "EnemyBase.h"
#include "Stage.h"
class ModelMaterial;
class ModelRenderer;

class Planet : public ActorBase , public EnemyBase
{

public:

	// �d�͂̋���
	static constexpr float DEFAULT_GRAVITY_POW = 25.0f;

	// �d�͂�������͈�
	static constexpr float DEFAULT_GRAVITY_RADIUS = 5000.0f;

	// �Q�[���I�[�o�[�͈�
	static constexpr float DEFAULT_DEAD_LENGTH = 1000.0f;

	// �d�͎��
	enum class TYPE
	{
		GROUND,
		SPHERE,
		TRANS_ROT,
		TRANS_CAMERA_FIXED,
		ROAD,
	};

	// �R���X�g���N�^
	Planet(const Stage::NAME& name, const TYPE& type, const Transform& transform);

	// �f�X�g���N�^
	~Planet(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	
private:

	// �f�����
	TYPE type_;

	// �f����
	Stage::NAME name_;

	// ���f���`��p�p
	std::unique_ptr<ModelMaterial> material_;
	std::unique_ptr<ModelRenderer> renderer_;

};
