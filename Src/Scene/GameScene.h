#pragma once
#include <memory>
#include "SceneBase.h"
class Stage;
class SkyDome;
class ColliderController;
class Player;
class Weapon;
class Soul;
class NormalEnemy;
class TestEnemy;

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

	// �Փ˔���
	bool CheckAttackCollisions(const VECTOR& p1, float r1, const VECTOR& p2, float r2);
	void SetCollision(void);

	void MakeEnemies(void);

	// �X�e�[�W
	std::unique_ptr<Stage> stage_;
	
	// ��
	std::shared_ptr<Soul> soul_;

	// �G�I�u�W�F�N�g
	std::vector<std::shared_ptr<TestEnemy>> testEnemy_;

	// ����
	std::shared_ptr<Weapon> weapon_;
	
	// �Փ˔��茟�m
	std::unique_ptr<ColliderController> colliderController_;

	// �X�J�C�h�[��
	std::unique_ptr<SkyDome> skyDome_;

	// �v���C���[
	std::shared_ptr<Player> player_;
};
