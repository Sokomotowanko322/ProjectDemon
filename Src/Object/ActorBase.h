#pragma once
#include "Common/Transform.h"

class ResourceManager;
class SceneManager;

class ActorBase
{
public:
	// �R���X�g���N�^
	ActorBase(void);

	// �f�X�g���N�^
	~ActorBase();

	virtual void Init(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	const Transform& GetTransform(void) const;

private:

protected:

	// �V���O���g���Q��
	ResourceManager& resMng_;
	SceneManager& scnMng_;

	// ���f������̊�{���
	Transform transform_;
};