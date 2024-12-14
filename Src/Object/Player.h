#pragma once
#include <memory>
#include <vector>
#include <map>
#include <functional>
#include <DxLib.h>
#include "ActorBase.h"

class AnimationController;
class Collider;
class Capsule;
class ModelMaterial;
class ModelRenderer;
class Soul;
class Weapon;

class Player : public ActorBase
{

public:

	// プレイ中の遷移状態
	enum class STATE
	{
		NONE,
		PLAY,
		INHALE,
		WARP_RESERVE,
		WARP_MOVE,
		DEAD,
		VICTORY,
		END
	};

	// アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		WALK,
		RUN,
		COMBO_ONE,
		COMBO_TWO,
		COMBO_THREE,
		INHALE,
		WARP_PAUSE,
		FLY,
		FALLING,
		VICTORY,
		ALL
	};

	//アニメーションKEY
	std::string ANIM_DATA_KEY[(int)ANIM_TYPE::ALL] =
	{
		"IDLE",
		"WALK",
		"RUN",
		"COMBO_ONE",
		"COMBO_TWO",
		"COMBO_THREE",
		"INHALE",
	};

	enum COMBOCOUNT
	{
		NONE,
		FIRSTSTEP,
		SECONDSTEP,
		THIRDSTEP
	};

	// コンストラクタ
	Player(void);

	// デストラクタ
	~Player(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	// 衝突判定に用いられるコライダ制御
	void AddCollider(std::weak_ptr<Collider> collider);
	void ClearCollider(void);

	// 衝突用カプセルの取得
	const Capsule& GetCapsule(void) const;
	const Transform& GetWeapon(void) const;
	const Transform& GetHipsTransform(void) const;
	VECTOR& GetLeftHandPos(void);
	const ANIM_TYPE GetNowAnim(void) const;

private:

	// アニメーション
	std::unique_ptr<AnimationController> animationController_;
	// ブレンド用
	std::string animationKey_;
	std::string preAnimationKey_;

	// 武器
	std::shared_ptr<Weapon> weapon_;

	// 魂
	std::shared_ptr<Soul> soul_;

	// 吸収時エフェクト
	VECTOR effectInhalePos_;
	int effectInhaleResId_;
	int effectInhalePlayId_;
	
	// 武器の追従位置
	int rightHandBoneIndex_;
	int leftHandBoneFrame_;
	// カメラの追従位置
	int hipsBoneIndex_;

	// ブレンド用フレーム番号
	int frameNo_;

	// 直近の状態取得
	std::vector<std::string> currentState_;

	// 状態管理
	STATE state_;
	STATE preState_;
	
	// アニメーション種別
	ANIM_TYPE animType_;

	// 状態管理(状態遷移時初期処理)
	std::map<STATE, std::function<void(void)>> stateChanges_;
	// 状態管理(更新ステップ)
	std::function<void(void)> stateUpdate_;

	// モデル描画用用
	std::unique_ptr<ModelMaterial> material_;
	std::unique_ptr<ModelRenderer> renderer_;
	// 移動方向
	VECTOR moveDir_;

	// 移動量
	VECTOR movePow_;

	// 移動後の座標
	VECTOR movedPos_;

	// 左手の座標
	VECTOR leftHandPos_;

	// 移動スピード
	float moveSpeed_;
	
	// 回転
	Quaternion playerRotY_;
	Quaternion goalQuaRot_;
	float stepRotTime_;
	float rotRad_;

	// ジャンプ量
	VECTOR jumpPow_;

	// ジャンプ判定
	bool isJump_;
	// 吸収判定
	bool isInhale_;

	// 攻撃判定
	bool canMove_;
	bool isAttack_;
	bool atkCol_;
	bool hasCol_;

	// アニメーション制御
	bool stopAnim_;

	// 入力受付時間
	float stepJump_;
	float comboInputTime_;
	int comboStep_;

	// 衝突判定に用いられるコライダ
	std::vector<std::weak_ptr<Collider>> colliders_;
	std::unique_ptr<Capsule> capsule_;

	// 衝突チェック
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

	// 丸影
	int imgShadow_;

	// トゥーンマップ
	int imgToonMap_;

	// アニメーション制御
	void InitAnimation(void);
	void BlendAnimation(void);
	
	//アニメーション制御
	void ChangeAnim(ANIM_TYPE state);

	// アニメーション移動地無効化
	void DisableAnimMovePow(void);

	// 状態遷移
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStatePlay(void);
	void ChangeStateInhale(void);
	
	// 更新ステップ
	void UpdateNone(void);
	void UpdatePlay(void);
	void UpdateInhale(void);
	
	// 影描画
	void DrawShadow(void);

	// 操作
	void ProcessMove(void);
	void ProcessInhale(void);
	void ProcessJump(void);
	void MoveControll(void);

	// 攻撃
	void ProcessAttack(void);
	void ResetCombo(void);

	// 吸収エフェクト
	void SyncInhaleEffect(void);

	// 行動制御
	bool IsEndDodge(void);
	bool IsEndGuard(void);
	
	// 回転
	void SetGoalRotate(double rotRad);
	void Rotate(void);

	// 衝突判定
	void Collision(void);
	void CollisionGravity(void);
	void CollisionCapsule(void);

	// 移動量の計算
	void CalcGravityPow(void);

	// 着地モーション終了
	bool IsEndLanding(void);
};
