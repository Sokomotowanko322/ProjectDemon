#pragma once
#include <memory>
#include "SceneBase.h"
class Stage;
class SkyDome;
class Player;
class Soul;
class NormalEnemy;

class GameScene : public SceneBase
{

public:
	
	// �R���X�g���N�^
	GameScene(void);

	// �f�X�g���N�^
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:

	// �X�e�[�W
	std::unique_ptr<Stage> stage_;
	
	// ��
	std::shared_ptr<Soul> soul_;

	// �G
	std::unique_ptr<NormalEnemy> normalEnemy_;

	// �X�J�C�h�[��
	std::unique_ptr<SkyDome> skyDome_;

	// �v���C���[
	std::shared_ptr<Player> player_;
};
