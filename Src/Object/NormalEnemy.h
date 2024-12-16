#include <vector>
#include <memory>
#include "../Common/Quaternion.h"
#include "EnemyBase.h"

class Player;
class AnimationController;
class Collider;
class Capsule;
class Camera;
class Player;
class Soul;
class ModelMaterial;
class ModelRenderer;

class NormalEnemy : public EnemyBase
{
public:

	struct Data
	{
		float dissolveHeight_;
		float disMinY_;
		float disMaxY_;
		float dummy;
		float clickedU;
		float clickedV;
		float dummy2[2];
	};

	enum class MOVE_STATE
	{
		IDLE,
		WALK,
		ATTACK
	};

	// アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		WALK,
		RUN,
		ATTACK,
		ALL
	};

	//アニメーションKEY
	std::string ANIM_DATA_KEY[(int)ANIM_TYPE::ALL] =
	{
		"IDLE",
		"WALK",
		"RUN",
		"ATTACK",
	};


	// 配列用にレンダーターゲットをそれぞれ用意
	enum SHADER_RT_TYPE
	{
		SECOND_RENDERTARGET,
		OUTLINE,
		FIRST_RENDERTARGET,
		NORMAL,
		BLUR,
		SHRINK,
		DEPTH,
		MAX
	};


	// コンストラクタ
	NormalEnemy(std::weak_ptr<Player> player);

	// デストラクタ
	~NormalEnemy(void);

	// 初期化
	void Init(void) override;
	void InitAnimation(void);
	void InitDissolve(void);

	// 更新
	void Update(void) override;

	// 描画
	void Draw(void) override;

	// ディゾルブエフェクト作成
	void MakeDissolve(void);

	// 敵の位置を設定
	void SetPos(VECTOR pos);

	// 状態遷移
	void ChangeState(MOVE_STATE state);

private:

	// アニメーション遷移用
	MOVE_STATE state_;
	MOVE_STATE preState_;

	// STATE内に格納するキー
	std::string animationKey_;
	std::string preAnimationKey_;

	// STATEの変更、関数内で同時にUPDATEとアニメーションを呼び出す
	std::unordered_map<MOVE_STATE, std::function<void(void)>> stateChange_;
	void ChangeIdle(void);
	void ChangeMove(void);
	void ChangeAttack(void);

	// 更新
	std::function<void(void)> stateUpdate_;
	void UpdateIdle(void);
	void UpdateWalk(void);
	void UpdateAttack(void);

	// モデル描画用用
	std::unique_ptr<ModelMaterial> material_;
	std::unique_ptr<ModelRenderer> renderer_;

	// アニメーション制御用
	std::unique_ptr<AnimationController> animationController_;

	// カメラ情報の取得
	std::unique_ptr<Camera> camera_;

	// データ内情報取得用
	Data* data_;

	// バッファ格納変数
	int buff_;

	// レンダーターゲット設定用変数
	int renderTarget_[SHADER_RT_TYPE::MAX];

	// ディゾルブ時に呼び出すテクスチャ
	int dissolveTex_;

	// シェーダファイル格納
	int pixelShader_;
	int vertexShader_;
	int postEffect_;

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