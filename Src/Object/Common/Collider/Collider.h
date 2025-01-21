#pragma once

#include <Dxlib.h>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "ColliderTag.h"

class Transform;

/// <summary>
/// コライダ情報を保持するクラス
/// </summary>
class Collider
{

public:

	/// <summary>
	/// コライダ情報の構造体
	/// </summary>
	struct ColliderData
	{
		OBJECT_TYPE objType_;		//オブジェクト名
		ENEMY_SUBTYPE subType_;		//オブジェクトのフレーム名
		COL_TYPE colType_;			//衝突判定の種類
		int colModelId_;			//モデルid
		int colModelFrameIdx_;		//フレーム番号
		bool ableCol_;				//衝突判定が有効か

		float radius_;				//半径
		VECTOR localPosTop_;		//上端ローカル座標
		VECTOR localPosBottom_;		//下端ローカル座標
		VECTOR localPosCenter_;		//中心座標

		ColliderData()
		{
			objType_ = OBJECT_TYPE::MAX;
			subType_ = ENEMY_SUBTYPE::NONE;
			colType_ = COL_TYPE::NONE;
			colModelId_ = -1;
			colModelFrameIdx_ = -1;
			ableCol_ = false;
			radius_ = 1.0f;
			localPosTop_ = {};
			localPosBottom_ = {};
			localPosCenter_ = {};
		}
	};

	/// <summary>
	/// デフォルトコンストラクタ
	/// </summary>
	Collider(const Transform& transform);
	
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="transform">追従するモデル情報</param>
	/// <param name="colType">当たり判定の種類</param>
	/// <param name="modelId">モデルID : 必要ない場合-1</param>
	/// <param name="frameIdx">フレーム番号 : 必要ない場合-1</param>
	Collider(
		const Transform& transform,
		OBJECT_TYPE objType, COL_TYPE colType, bool ableCol ,int modelId = -1, int frameIdx = -1);
	

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="transform">追従するモデル情報</param>
	/// <param name="objType">オブジェクト名</param>
	/// <param name="subType">オブジェクトのフレーム名</param>
	/// <param name="colType">衝突判定の種類</param>
	/// <param name="ableCol">衝突判定を行うかのフラグ</param>
	/// <param name="modelId">モデルID : 必要ない場合-1</param>
	/// <param name="frameIdx">フレーム番号 : 必要ない場合-1</param>
	Collider(
		const Transform& transform,
		OBJECT_TYPE objType,
		ENEMY_SUBTYPE subType,
		COL_TYPE colType,
		bool ableCol, int modelId = -1, int frameIdx = -1);
	
	~Collider(void);

	/// <summary>
	/// 当たり判定の種類を取得
	/// </summary>
	/// <returns>COL_TYPE</returns>
	inline const COL_TYPE GetColType(void) const
	{
		return colData_.colType_;
	}

	/// <summary>
	/// モデルのハンドルIDを取得
	/// </summary>
	/// <returns>モデルのハンドルID</returns>
	inline const int GetModelId(void)
	{
		return colData_.colModelId_;
	}

	/// <summary>
	/// 追従するフレーム番号を取得
	/// </summary>
	/// <returns>追従するフレーム番号を取得</returns>
	const int GetFrameIdx(void) const
	{
		return colData_.colModelFrameIdx_;
	}


	/// <summary>
	/// 衝突判定を行うかのフラグを取得
	/// </summary>
	/// <returns>
	/// true : 行う
	/// false : 行わない
	/// </returns>
	const bool GetAbleCol(void) const
	{
		return colData_.ableCol_;
	}

	/// <summary>
	/// コライダ情報を取得
	/// </summary>
	/// <returns>ColliderData</returns>
	const ColliderData& GetColliderData(void) const
	{
		return colData_;
	}
		

	/// <summary>
	/// オブジェクト名を取得
	/// </summary>
	/// <returns>colData_.objType_</returns>
	inline const OBJECT_TYPE GetObjType(void)
	{
		return colData_.objType_;
	}

	/// <summary>
	/// オブジェクトのフレーム名を取得
	/// </summary>
	/// <returns>ENEMY_SUBTYPE</returns>
	ENEMY_SUBTYPE GetObjSubType(void);

	/// <summary>
	/// 上端のワールド座標を取得
	/// </summary>
	/// <returns>VECTOR</returns>
	VECTOR GetPosTop(void) const;

	/// <summary>
	/// 下端のワールド座標を取得
	/// </summary>
	/// <returns>VECTOR</returns>
	VECTOR GetPosDown(void) const;
	
	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns>VECTOR</returns>
	const VECTOR& GetPos(void) const;

	/// <summary>
	/// 中心のワールド座標を取得
	/// </summary>
	/// <returns>
	/// 追従フレームが存在 : colData_.localPosCenter_
	/// 追従フレームが存在してない : colData_.transform_.pos
	/// </returns>
	const VECTOR& GetCenterPos(void) const;

	/// <summary>
	/// 半径を取得
	/// </summary>
	/// <returns>colData_.radius_</returns>
	const float GetRadius(void) const;

	/// <summary>
	/// 上端のローカル座標を設定
	/// </summary>
	/// <param name="val">上端のローカル座標</param>
	void SetPosTop(const VECTOR& val);
	
	/// <summary>
	/// 下端のローカル座標を設定
	/// </summary>
	/// <param name="val">下端のローカル座標</param>
	void SetPosDown(const VECTOR& val);
	
	/// <summary>
	/// 中心座標を設定
	/// </summary>
	/// <param name="val">中心座標</param>
	void SetPosCenter(const VECTOR& val);

	/// <summary>
	/// 半径を設定
	/// </summary>
	/// <param name="val">半径</param>
	void SetRadius(const float& val);

	/// <summary>
	/// フレーム番号を設定
	/// </summary>
	/// <param name="val">フレーム番号</param>
	void SetFrameIndex(const int& val);

	/// <summary>
	/// 当たり判定を有効にするか設定
	/// </summary>
	/// <param name="val">
	/// true : 有効
	/// false : 無効
	/// </param>
	void SetColAble(const bool val);
	
	
	/// <summary>
	/// 相対座標を回転させてワールド座標で取得する
	/// </summary>
	/// <param name="localPos">相対座標</param>
	/// <returns>回転後のワールド座標</returns>
	VECTOR GetRotPos(const VECTOR& localPos) const;


private:

	//追従するモデルの基本情報
	const Transform& transform_;

	//コライダ情報の構造体
	ColliderData colData_;

};

