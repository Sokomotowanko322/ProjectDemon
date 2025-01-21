#pragma once

#include <chrono>
#include <string>
#include "../Singleton.h"

/// <summary>
/// 時間管理
/// </summary>
class TimeManager 
	: public Singleton<TimeManager>
{
public:

	/// <summary>
	/// Dxlib_End()前のリソース破棄
	/// </summary>
	void Destroy(void) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void);

	/// <summary>
	/// デルタタイムのリセット
	/// </summary>
	void ResetDeltaTime(void);

	/// <summary>
	/// 時間停止状態の切替
	/// </summary>
	/// <param name="isStop">
	/// true : 停止中
	/// false : 停止していない
	/// </param>
	void SetStop(bool isStop);

	/// <summary>
	/// 時間停止中か判定
	/// </summary>
	/// <returns>
	/// true : 停止中
	/// false : 停止していない
	/// </returns>
	const bool GetIsStop(void);

	/// <summary>
	/// 総経過時間の取得
	/// </summary>
	/// <returns>time_</returns>
	const float GetTime(void);

	/// <summary>
	/// デルタタイムの取得
	/// </summary>
	/// <returns>deltaTime_</returns>
	const float GetDeltaTime(void);

private:

	friend class Singleton<TimeManager>;

	// デルタタイム
	std::chrono::system_clock::time_point preTime_;
	
	//総経過時間
	float time_;

	//デルタタイム
	float deltaTime_;

	//時間停止中か
	bool isStop_;

	TimeManager(void);
	//TimeManager(const TimeManager&);
	~TimeManager() = default;

};

