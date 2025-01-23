#pragma once
#include <memory>
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
class SceneManager;
class SkyDome;

class TitleScene : public SceneBase
{

public:

	// コンストラクタ
	TitleScene(void);

	// デストラクタ
	~TitleScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:
	// シングルトン参照
	ResourceManager& resMng_;

	//表示を点滅させるカウント
	int blinkCnt_;
	int count_;

	//
	int titleImg_;
	int startImg_;
};
