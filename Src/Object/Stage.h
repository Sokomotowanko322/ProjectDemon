#pragma once
#include <memory>
#include <map>
#include "Common/Transform.h"
class ResourceManager;
class Planet;
class NormalEnemy;
class Player;
class Destroyer;

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
	Stage(Player& player);

	// �f�X�g���N�^
	~Stage(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	// �X�e�[�W�ύX
	void ChangeStage(NAME type);

	// �ΏۃX�e�[�W���擾
	std::weak_ptr<Planet> GetPlanet(NAME type);

private:

	// �V���O���g���Q��
	ResourceManager& resMng_;
	Player& player_;
	
	// �X�e�[�W�A�N�e�B�u�ɂȂ��Ă���f���̏��
	NAME activeName_;
	std::weak_ptr<Planet> activePlanet_;

	// �f��
	std::map<NAME, std::shared_ptr<Planet>> planets_;

	// �G�I�u�W�F�N�g
	std::vector<std::shared_ptr<NormalEnemy>> normalEnemy_;

	// ���Planet
	std::shared_ptr<Planet> nullPlanet = nullptr;

	float step_;

	// �ŏ��̘f��
	void MakeMainStage(void);

	// ���I�u�W�F�N�g
	void MakeEnvironment(void);

};
