#pragma once
#include <memory>
#include <chrono>
#include <unordered_map>
#include <functional>

// 推奨しませんが、どうしても使いたい方は
#define mainCamera SceneManager::GetInstance().GetCamera()

class SceneBase;
class Fader;
class Camera;
class Collision;
class ColliderManager;

class SceneManager
{

public:

	// シーン管理用
	enum class SCENE_ID
	{
		NONE,
		TITLE_IMAGE,
		GAME,
		RESULT
	};
	
	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static SceneManager& GetInstance(void);

	// <summary>
	/// 衝突判定登録情報の取得
	/// </summary>
	/// <returns>ColliderManagerの参照</returns>
	inline ColliderManager& GetColManager(void) const
	{
		return *colMng_;
	}

	/// <summary>
	/// 衝突判定の取得
	/// </summary>
	/// <returns>Collisionの参照</returns>
	inline Collision& GetCollision(void) const
	{
		return *collision_;
	}

	void Init(void);
	void Init3D(void);
	void Update(void);
	void Draw(void);

	// リソースの破棄
	void Destroy(void);

	// 状態遷移
	void ChangeScene(SCENE_ID nextId);

	// シーンIDの取得
	SCENE_ID GetSceneID(void);

	// デルタタイムの取得
	float GetDeltaTime(void) const;

	// カメラの取得
	Camera& GetCamera(void);

private:

	// 静的インスタンス
	static SceneManager* instance_;

	SCENE_ID sceneId_;
	SCENE_ID waitSceneId_;

	// 各種シーン
	std::unique_ptr<SceneBase> scene_;

	// 衝突判定の登録
	std::unique_ptr<ColliderManager> colMng_;

	//衝突判定を行うクラス
	std::unique_ptr<Collision> collision_;

	// フェード
	std::unique_ptr<Fader> fader_;

	// カメラ
	std::unique_ptr<Camera> camera_;

	// シーン遷移中判定
	bool isSceneChanging_;

	// デルタタイム
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;
	
	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	SceneManager(void);
	// コピーコンストラクタも同様
	SceneManager(const SceneManager& manager) = default;
	// デストラクタも同様
	~SceneManager(void) = default;

	// デルタタイムをリセットする
	void ResetDeltaTime(void);

	// シーン遷移
	void DoChangeScene(SCENE_ID sceneId);

	// フェード
	void Fade(void);
};
