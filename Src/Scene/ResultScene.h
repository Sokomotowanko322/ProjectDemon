#include "SceneBase.h"
#pragma once

class ResultScene : public SceneBase
{
public:

	// �R���X�g���N�^
	ResultScene(void);

	// �f�X�g���N�^
	~ResultScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:
	int resultImg_;
};

