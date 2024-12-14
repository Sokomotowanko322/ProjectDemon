#pragma once
#include <string>
#include <map>
class SceneManager;

class AnimationController
{

public:

	// アニメーションデータ
	struct AnimationData
	{

		// アニメーションハンドル
		int animHandle = -1;

		// アニメーションタイプ
		std::string state = "";

		// アタッチNo
		int attachNo = -1;

		// 優先されるアニメーション
		bool isPriority = false;

		// アニメーションの総再生時間
		float animTotalTime = 0.0f;

		// アニメーション速度
		float speedAnim = 0.0f;

		// 再生中のアニメーション時間
		float stepAnim = 0.0f;

		// アニメーションが始まる時間
		float startTime = 0.0f;

		// アニメーションが終わる時間
		float endStep = 0.0f;

		// ブレンドレート
		float blendRate = 0.0f;

		// ブレンドタイム
		float blendTime = 0.0f;

		// ループするかしないか
		bool isLoop = false;

		// ストップするかしないか
		bool isStop = false;

		// 逆再生
		bool isReverse = false;

		// アニメーション番号
		int animIndex = 0;

	};

	// コンストラクタ
	AnimationController(int modelId);

	// デストラクタ
	~AnimationController(void);

	// アニメーション追加
	void Add(const std::string state, const std::string& path, float startStep,
		float speed, int animHandle, bool isLoop, int animIndex, bool isReverse);

	// アニメーションの更新
	void Update(void);

	// 再生中のアニメーション
	std::string GetPlayType(void) const;

	// アタッチされている数を取得
	int GetAttachNum(void) const;

	// 状態遷移
	void ChangeAnimation(std::string state);

	// アニメーションデータの取得
	AnimationData GetAnimData(const std::string& state);

	// すべてのアニメーションデータを取得
	const std::map <std::string, AnimationData>& GetAnimDatas(void) const;

	// アニメーションが始まる時間を設定
	void SetStartStepAnim(std::string state, int stepAnim);

	// 優先されているアニメーションタイプを取得
	bool GetIsPriority(void);

	// 今のアニメーションの再生が終了しているか判定
	bool IsEndPlayAnimation(void);

	// 再生しているアニメーションがブレンドされているかどうか
	bool IsBlendPlay(std::string state);

	// ブレンドしているアニメーションが再生終了しているか判定
	bool IsEndBlendingPlayAnimation(std::string state);

	// ストップさせる
	void Stop(bool stop);

	// スローさせる
	void Slow(bool isSlow);

	// アニメーションスピードを変える
	void SpeedChange(float speed);

private:

	// モデルのハンドルID
	int modelId_;

	// モデルを読み込んだかのフラグ
	bool LoadModel_;

	// 種類別のアニメーションデータ
	std::map <std::string, AnimationData> animData_;

	// 今の状態
	std::string state_;

	// 一個前の状態
	std::string preState_;

	// アタッチしている数
	int AttachNum_;

	// ヒットストップ
	bool isStop_;

	// スピードコントロールする用
	bool isSlow_;

	// ゲーム全体のスピード
	float gameSpeed_;

	// アタッチ
	void Attatch(std::string state);

	// デタッチ
	void Dettach(int attachNo);
};