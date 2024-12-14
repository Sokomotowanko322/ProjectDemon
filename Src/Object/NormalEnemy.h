#include <vector>
#include <memory>
#include "../Common/Quaternion.h"
#include "EnemyBase.h"

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

	enum class STATE
	{
		THINK,
		MOVE,
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
	NormalEnemy(void);

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
	void ChangeState(STATE state);

private:

	// アニメーション遷移用
	STATE state_;
	STATE preState_;

	// STATE内に格納するキー
	std::string animationKey_;
	std::string preAnimationKey_;

	// STATEの変更、関数内で同時にUPDATEとアニメーションを呼び出す
	std::unordered_map<STATE, std::function<void(void)>> stateChange_;
	void ChangeThink(void);
	void ChangeMove(void);
	void ChangeAttack(void);

	// 更新
	std::function<void(void)> stateUpdate_;
	void UpdateThink(void);
	void UpdateMove(void);
	void UpdateAttack(void);

	// モデル描画用用
	std::unique_ptr<ModelMaterial> material_;
	std::unique_ptr<ModelRenderer> renderer_;

	// アニメーション制御用
	std::unique_ptr<AnimationController> animationController_;

	// 魂の情報取得
	//std::shared_ptr<Soul> soul_;

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
	float deltaTime;

	float sinTime_;
	float offset_;
};