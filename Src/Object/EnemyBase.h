#pragma once
#include "Common/Transform.h"

class Player;
class ResourceManager;
class SceneManager;
class AnimationController;

class EnemyBase
{
public:
	// �R���X�g���N�^
	EnemyBase(void);

	// �f�X�g���N�^
	~EnemyBase();

	virtual void Init(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	const Transform& GetEnemyTransform(void) const;
	virtual VECTOR GetPos(void) const;

private:

protected:

	// �v���C���[
	std::weak_ptr<Player> player_;

	// �V���O���g���Q��
	ResourceManager& resMng_;
	SceneManager& scnMng_;

	// �A�j���[�V��������
	std::unique_ptr<AnimationController> animationController_;

	// ���f������̊�{���
	Transform enmyTransform_;

	// ��]�̍���
	VECTOR diff_;
	
	// �N�H�[�^�j�I����]
	Quaternion quaRot_;
	// �N�H�[�^�j�I����]�l����p
	Quaternion rot_;


	// ��]�̓���
	float dot_;

};
