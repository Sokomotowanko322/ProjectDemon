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

	// �U��
	void Vibration(void);

private:

	// �Փ˔���
	void SetPlayerAttackCollision(void);
	void SetCollision(void);

	// �G�̐���
	void MakeEnemies(void);
	
	// ���̐���
	void MakeSouls(void);

	void DrawHp(float hp, float maxHp, int x, int y, int width, int height);

	
	// �X�e�[�W
	std::unique_ptr<Stage> stage_;
	
	// ��
	std::vector<std::shared_ptr<Soul>> soul_;

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
	
	float soulAliveCnt_;
};
