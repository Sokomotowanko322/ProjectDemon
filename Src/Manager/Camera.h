#pragma once

#include <map>
#include <unordered_map>
#include <functional>
#include <DxLib.h>
#include "../Common/Quaternion.h"
#include "../Common/Geometry2D.h"

class Transform;
class ColliderController;

/// <summary>
/// カメラ
/// </summary>
class Camera
{

public:

	/// <summary>
	/// カメラモード
	/// </summary>
	enum class MODE
	{
		FIXED_POINT,
		FREE,
		FOLLOW,
		FOLLOW_FIX,		//角度変化なし
		FOLLOW_MOUSE,
		SUCCESSFUL,
	};

	Camera(void);
	~Camera(void);

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(void);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void);

	/// <summary>
	/// 描画前のカメラの設定
	/// </summary>
	void SetBeforeDraw(void);

	/// <summary>
	/// カメラ座標の取得
	/// </summary>
	/// <returns>カメラ座標</returns>
	inline const VECTOR& GetPos(void) const
	{
		return pos_;
	}

	/// <summary>
	/// カメラの操作角度
	/// </summary>
	/// <returns>カメラの角度</returns>
	inline const VECTOR& GetAngles(void) const
	{
		return angles_;
	}

	/// <summary>
	/// カメラのデフォルトの角度を設定
	/// </summary>
	/// <param name="qua">カメラのデフォルトの角度 : Quaternion</param>
	void SetDefQuaRot(const Quaternion& qua);

	/// <summary>
	/// カメラの注視点を取得
	/// </summary>
	/// <returns>カメラの注視点 : VECTOR</returns>
	inline const VECTOR& GetTargetPos(void) const
	{
		return targetPos_;
	}

	/// <summary>
	/// カメラの角度を設定
	/// </summary>
	/// <param name="angle">カメラの角度(デグリー角) : VECTOR</param>
	void SetAngle(const VECTOR& angle);

	/// <summary>
	/// カメラ角度の取得
	/// </summary>
	/// <returns>カメラ角度 : Quaternion</returns>
	inline const Quaternion& GetQuaRot(void) const
	{
		return rot_;
	}

	/// <summary>
	/// X回転を抜いたカメラ角度の取得
	/// </summary>
	/// <returns>Quaternion</returns>
	inline const Quaternion& GetQuaRotOutX(void) const
	{
		return rotOutX_;
	}

	/// <summary>
	/// カメラの前方方向を取得
	/// </summary>
	/// <returns>VECTOR</returns>
	const VECTOR GetForward(void) const;

	/// <summary>
	/// カメラモードの変更
	/// </summary>
	/// <param name="mode">変更するカメラモード</param>
	void ChangeMode(const MODE mode);

	/// <summary>
	/// 追従対象の設定
	/// </summary>
	/// <param name="follow">追従対象の制御情報</param>
	void SetFollow(const Transform* follow);

	/// <summary>
	/// 当たり判定をとるステージモデルをセット
	/// </summary>
	/// <param name="modelId">ステージモデルのハンドル</param>
	void SetHitModel(const int modelId);

	/// <summary>
	/// カメラの回転力を設定
	/// </summary>
	/// <param name="pow">縦横方向のカメラの回転力 : Vector2f</param>
	void SetCameraRotPow(const Vector2f& pow);

	/// <summary>
	/// 現在のマウス座標をVECTOR型で取得
	/// </summary>
	/// <returns>VECTOR</returns>
	const VECTOR GetMousePos(void);

	/// <summary>
	/// 成功時カメラ終了時に実行する関数を登録
	/// </summary>
	/// <param name="func">成功時カメラ終了時に実行する関数</param>
	void SetSuccessCameraMoveEndFunc(std::function<void(void)> func);

	/// <summary>
	/// カメラシェイクを設定
	/// </summary>
	/// <param name="time">カメラシェイクの時間</param>
	/// <param name="shakePow">揺らす力</param>
	/// <param name="timeScale">経過時間の加減速</param>
	void SetCameraShake(float time, float shakePow, float timeScale = 10.0f);

	/// <summary>
	/// カメラシェイクを行う
	/// </summary>
	void CameraShake(void);

private:

	// カメラが追従対象とするTransform
	const Transform* followTransform_;

	// カメラモード
	MODE mode_;

	// カメラの位置
	VECTOR pos_;

	// 一フレーム前のカメラの位置
	VECTOR prePos_;

	// カメラ角度(rad)
	VECTOR angles_;

	// X軸回転が無い角度
	Quaternion rotOutX_;

	// カメラ角度
	Quaternion rot_;

	// 注視点
	VECTOR targetPos_;

	// カメラの上方向
	VECTOR cameraUp_;

	//1f前のカメラが追従するマウス座標
	Vector2 preMousePos_;

	//現在のカメラが追従するマウス座標
	Vector2 nowMousePos_;

	//マウス操作時のカメラの回転角度
	Vector2f radXY_;

	//カメラの回転力
	Vector2f cameraRotPow_;

	//デフォルトのカメラ角度
	Quaternion defSetRot_;

	//カメラシェイクしているか
	bool isShake_;

	//カメラシェイク時の経過時間
	float shakeStep_;

	//カメラシェイク時の移動量
	float shakePow_;

	//経過時間の加減速
	float timeScale_;

	//成功時カメラの経過時間
	float successCameraTimeStep_;

	//クリア時のカメラワークが終わった際に実行する関数
	std::function<void(void)> successCameraEnd_;

	//ステージモデルID
	int stagemodelId_;

	//マウス操作時の注視点のローカル座標
	VECTOR  localPosMouseFollow_;

	//壁衝突検知用の線分の長さ
	float  collitionLineStageCamera_;

	// カメラを初期位置に戻す
	void SetDefault(void);

	// 追従対象との位置同期を取る
	void SyncFollow(void);

	// 追従対象との位置同期を取る
	void SyncFollowFix(void);

	// マウス操作時:追従対象との位置同期を取る
	void SyncMouse(void);

	// クリア時演出:追従対象との位置同期を取る
	void SyncSuccessCamera(void);

	// カメラ操作
	void ProcessRot(void);

	// マウス操作時のカメラ操作
	void ProcessRotMouse(void);

	//クリア時カメラ移動
	void SuccessCameraMove(void);

	//状態遷移
	std::unordered_map<MODE, std::function<void(void)>> modeChanges_;
	void ChangeFixedPoint(void);		//定点
	void ChangeFree(void);		//自由視点
	void ChangeFollow(void);		//自由視点追従
	void ChangeFollowFix(void);		//定点追従
	void ChangeFollowMouse(void);		//マウス追従
	void ChangeSuccessful(void);		//クリア時カメラ演出

	// モード別更新ステップ
	std::function<void(void)> modeUpdate_;
	void UpdateFixedPoint(void);	//定点	
	void UpdateFree(void);		//自由視点
	void UpdateFollow(void);		//自由視点追従
	void UpdateFollowFix(void);		//定点追従
	void UpdateFollowMouse(void);		//マウス追従
	void UpdateSuccessful(void);		//クリア時カメラ演出

	// モード別描画前カメラ設定ステップ
	std::function<void(void)> setBeforeDrawMode_;
	void SetBeforeDrawFixedPoint(void);		//定点
	void SetBeforeDrawFree(void);		//自由視点
	void SetBeforeDrawFollow(void);		//自由視点追従
	void SetBeforeDrawFollowFix(void);			//定点追従	
	void SetBeforeDrawFollowMouse(void);		//マウス追従
	void SetBeforeDrawSuccessful(void);		//クリア時カメラ演出

	//デバッグ用
	void DrawDebug(void);

	//カメラの壁衝突
	void CameraCollision(void);

	//カメラの壁衝突判定用
	bool HitCheck(VECTOR campos, VECTOR Ppos, float size);

};