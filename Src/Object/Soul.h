#include <DxLib.h>

class Player;
//class NormalEnemy;
class TestEnemy;

class Soul
{
public:

	enum SOUL_TYPE
	{
		RED,
		GREEN,
		BLUE,
		ALL
	};

	// �R���X�g���N�^ & �f�X�g���N�^
	Soul(std::weak_ptr<Player> player);
	~Soul(void);

	void Init(void);

	// �X�V����
	void Update(void);
	void UpdateFloating(void);
	void UpdateInhale(void);
	
	void Draw(void);
	void StopEffect(void);

private:

	// �|�C���^
	//std::shared_ptr<NormalEnemy> normalEnemy_;
	std::shared_ptr<TestEnemy> normalEnemy_;
	std::weak_ptr<Player> player_;

	// ���̈ʒu
	VECTOR followSoulPos_[SOUL_TYPE::ALL];
	VECTOR preFollowSoulPos_[SOUL_TYPE::ALL];

	 // �z�����̃S�[���n�_
	VECTOR playerPos_;
	
	// �G�t�F�N�g���
	int effectSoulResId_[SOUL_TYPE::ALL];
	int effectSoulPlayId_[SOUL_TYPE::ALL];

	// ���Ԍo�߂ɂ���č��̐U�ꕝ�Ȃǂ����Z
	float soulMoveCnt_;
	float rotationStep_;

	// ���V���x
	float moveSpeed_;

	// �G�t�F�N�g�̉�]�l
	float effectRot_;

};

