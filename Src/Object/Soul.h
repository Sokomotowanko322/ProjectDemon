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

	// コンストラクタ & デストラクタ
	Soul(std::weak_ptr<Player> player);
	~Soul(void);

	void Init(void);

	// 更新処理
	void Update(void);
	void UpdateFloating(void);
	void UpdateInhale(void);
	
	void Draw(void);
	void StopEffect(void);

private:

	// ポインタ
	//std::shared_ptr<NormalEnemy> normalEnemy_;
	std::shared_ptr<TestEnemy> normalEnemy_;
	std::weak_ptr<Player> player_;

	// 魂の位置
	VECTOR followSoulPos_[SOUL_TYPE::ALL];
	VECTOR preFollowSoulPos_[SOUL_TYPE::ALL];

	 // 吸収時のゴール地点
	VECTOR playerPos_;
	
	// エフェクト情報
	int effectSoulResId_[SOUL_TYPE::ALL];
	int effectSoulPlayId_[SOUL_TYPE::ALL];

	// 時間経過によって魂の振れ幅などを加算
	float soulMoveCnt_;
	float rotationStep_;

	// 浮遊速度
	float moveSpeed_;

	// エフェクトの回転値
	float effectRot_;

};

