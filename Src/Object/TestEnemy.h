#include <DxLib.h>
#include <vector>
#include <functional>
#include <map>
#include <string>
#include <memory>
#include "../Common/Quaternion.h"
#include "EnemyBase.h"

class Player;
class ColliderController;
class AnimationController;
class Collider;
class Capsule;
class Camera;
class Player;
class Weapon;

class TestEnemy : public EnemyBase
{
public:
	/*struct Data
	{
		float dissolveHeight_;
		float disMinY_;
		float disMaxY_;
		float dummy;
		float clickedU;
		float clickedV;
		float dummy2[2];
	};*/

	enum class STATE
	{
		IDLE,
		WALK,
		ATTACK,
		DEATH
	};

	// アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		WALK,
		ATTACK,
		DEATH,
		ALL
	};

	//アニメーションKEY
	std::string ANIM_DATA_KEY[(int)ANIM_TYPE::ALL] =
	{
		"IDLE",
		"WALK",
		"NORMALATTACK",
		"DEATH",
	};


	//// 配列用にレンダーターゲットをそれぞれ用意
	//enum SHADER_RT_TYPE
	//{
	//	SECOND_RENDERTARGET,
	//	OUTLINE,
	//	FIRST_RENDERTARGET,
	//	NORMAL,
	//	BLUR,
	//	SHRINK,
	//	DEPTH,
	//	MAX
	//};


	// コンストラクタ
	TestEnemy(std::weak_ptr<Player> player);

	// デストラクタ
	~TestEnemy(void);

	// 初期化
	void Init(void) override;
	void InitAnimation(void);

	// 更新
	void Update(void) override;

	// 描画
	void Draw(void) override;

	// 敵の位置を設定
	void SetPos(VECTOR pos);
	VECTOR GetPos(void);

	// 状態遷移
	void ChangeState(STATE state);

	// ダメージ処理
	void SetDamage(int damage);

	
	bool IsAttack(void);
	bool IsAlive(void);
	bool isAlive_;

	// 衝突判定に用いられるコライダ制御
	bool CheckCollision(const VECTOR& p1, float r1, const VECTOR& p2, float r2);
	int GetHp(void);
	void ResolveCollision(VECTOR& p1, float r1, VECTOR& p2, float r2);
	
private:

	VECTOR movePos_;

	// コリジョンコントローラー
	std::unique_ptr<ColliderController> colliderController_;

	std::shared_ptr<Weapon> weapon_;
	 
	// アニメーション遷移用
	STATE state_;
	STATE preState_;

	// STATE内に格納するキー
	std::string animationKey_;
	std::string preAnimationKey_;

	// 直近の状態取得
	std::vector<std::string> currentState_;

	// STATEの変更、関数内で同時にUPDATEとアニメーションを呼び出す
	std::unordered_map<STATE, std::function<void(void)>> stateChange_;
	void ChangeIdle(void);
	void ChangeWalk(void);
	void ChangeAttack(void);
	void ChangeDeath(void);

	// 更新
	std::function<void(void)> stateUpdate_;
	void UpdateIdle(void);
	void UpdateWalk(void);
	void UpdateAttack(void);
	void UpdateDeath(void);

	// 衝突判定
	void AddCollider(void);

	// ステージとの衝突判定
	void CollisionStage(void);
	void SimpleGravity(void);

	// 状態判定
	void JudgeAct(void);

	// 遠ければプレイヤーに接近
	void ApproachToPlayer(void);
	
	// アニメーション制御用
	std::unique_ptr<AnimationController> animationController_;

	// コライダ
	std::weak_ptr<Collider> collider_;

	// ステージのコリジョンコントローラー
	std::unique_ptr<ColliderController> colliderStage_;
	
	// カメラ情報の取得
	std::unique_ptr<Camera> camera_;

	//// データ内情報取得用
	//Data* data_;

	//// バッファ格納変数
	//int buff_;

	//// レンダーターゲット設定用変数
	//int renderTarget_[SHADER_RT_TYPE::MAX];

	//// ディゾルブ時に呼び出すテクスチャ
	//int dissolveTex_;

	//// シェーダファイル格納
	//int pixelShader_;
	//int vertexShader_;
	//int postEffect_;

	// パラメータ
	int hp_;

	// フレーム(骨格)
	int armFrame_;

	bool isCatchPlayerPos_;
	bool isEndMove_;
	bool atkFlag_;
	bool onColFlag_;;
	bool wallCollFlag_;
	
	// デルタタイム
	float deltaTime_;

	// 回転にかかる時間
	float rotationStep_;
	
	//重力
	float gravity_;

	// 次の攻撃までのディレイ
	float attackDelay_;
	float cntDelay_;

	float sinTime_;
	float offset_;
};

