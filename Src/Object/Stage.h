#pragma once
#include <memory>
#include <map>
#include "Common/Transform.h"

class ResourceManager;
class Collision;
class ColliderController;
class Player;

class Stage
{

public:

	// �X�e�[�W�̐؂�ւ��Ԋu
	static constexpr float TIME_STAGE_CHANGE = 1.0f;

	// �X�e�[�W��
	enum class NAME
	{
		STAGE,
	};

	// �R���X�g���N�^
	Stage(std::weak_ptr<Player> player);

	// �f�X�g���N�^
	~Stage(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	// �R���C�_��ݒ肷��modelId�̎擾
	int GetModelId(void);

private:

	Transform stageTransform_;
	Transform colliderTransform_;

	// �V���O���g���Q��
	ResourceManager& resMng_;
	Collision& collision_;
	SceneManager& scnMng_;

	// �v���C���[
	std::shared_ptr<Player> player_;

	//�Փ˔��茟�m
	std::unique_ptr<ColliderController> colliderController_;

	// ���I�u�W�F�N�g
	void MakeEnvironment(void);

	// �Փ˔���
	void SetCollisionStage(void);

};
