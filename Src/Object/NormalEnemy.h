#pragma once
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
public :

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

	// アニメーション種別
	enum class STATE
	{
		IDLE,
		WALK,
		SLASH,
		DEATH,
	};

	// コンストラクタ
	NormalEnemy(void);

	// デストラクタ
	~NormalEnemy(void);

	void Init(void) override;
	void InitAnim(void);

	void Update(void) override;
	void Draw(void) override;
	
	// ディゾルブエフェクト作成
	void InitDissolve(void);
	void MakeDissolve(void);

	void SetPos(VECTOR pos);
	
private:

	// モデル描画用用
	std::unique_ptr<ModelMaterial> material_;
	std::unique_ptr<ModelRenderer> renderer_;

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