#include <DxLib.h>
#include <vector>
#include <functional>
#include <map>
#include <string>
#include <memory>
#include "../Common/Quaternion.h"
#include "EnemyBase.h"

class Player;
class AnimationController;
class Collider;
class Capsule;
class Camera;
class Player;
class Weapon;
class Soul;

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

	// 状態遷移
	void ChangeState(STATE state);

	// ダメージ処理
	void SetDamage(int damage);

	// 衝突判定に用いられるコライダ制御
	void AddCollider(std::weak_ptr<Collider> collider);
	void ClearCollider(void);
	bool CheckCollision(const VECTOR& p1, float r1, const VECTOR& p2, float r2);
	void ResolveCollision(VECTOR& p1, float r1, VECTOR& p2, float r2);
	bool isAlive_;
private:

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

	void AddCollider(void);

	void JudgeAct(void);

	void ApproachToPlayer(void);
	//// モデル描画用用
	//std::unique_ptr<ModelMaterial> material_;
	//std::unique_ptr<ModelRenderer> renderer_;

	// アニメーション制御用
	std::unique_ptr<AnimationController> animationController_;

	// コライダ
	std::weak_ptr<Collider> collider_;
	
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

	int hp_;

	bool isCatchPlayerPos_;
	bool isEndMove_;
	bool atkFlag_;
	

	// デルタタイム
	float deltaTime_;

	// 回転にかかる時間
	float rotationStep_;

	// 次の攻撃までのディレイ
	float attackDelay_;
	float cntDelay_;

	float sinTime_;
	float offset_;
};

