#include "SceneBase.h"
#pragma once

class ResultScene : public SceneBase
{
public:

	// コンストラクタ
	ResultScene(void);

	// デストラクタ
	~ResultScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:
	int resultImg_;
};

