#pragma once

#include <vector>
#include "../Transform.h"
#include "Collider.h"

class ColliderManager
{

public:

	ColliderManager(void);
	~ColliderManager(void);

	//コライダ情報更新
	void Update(void);

	//デバッグ描画
	void DrawCollider(OBJECT_TYPE type) const;
	void DrawColliderColor(OBJECT_TYPE type,int color) const;

	/// <summary>
	/// コライダ情報追加
	/// </summary>
	/// <param name="objType">オブジェクトの種別</param>
	/// <param name="colType">衝突判定の種別</param>
	/// <param name="trans">オブジェクトのモデル制御情報</param>
	/// <param name="ableCol">コライダー有効化 : 要確認</param>
	/// <param name="modelId"></param>
	/// <param name="frameIdx">モデルフレーム</param>
	/// <param name="radius">半径</param>
	/// <param name="localPosTop">カプセル用 : ローカル座標上</param>
	/// <param name="localPosBottom">カプセル用 : ローカル座標下</param>
	void AddCollider(
		OBJECT_TYPE objType,COL_TYPE colType, const Transform& trans,
		bool ableCol, int modelId = -1, int frameIdx = -1, 
		ENEMY_SUBTYPE objtype = ENEMY_SUBTYPE::NONE,
		float radius = 1.0f,
		const VECTOR& localPosTop = { 0.0f,0.0f,0.0f },
		const VECTOR& localPosBottom = { 0.0f,0.0f,0.0f }
	);

	//コライダ削除
	void DelCollider(void);

	/// <summary>
	/// コライダ削除
	/// </summary>
	/// <param name="tag">オブジェクトの種別</param>
	void DelCollider(OBJECT_TYPE tag);

	/// <summary>
	/// 格納しているコライダの数取得
	/// </summary>
	/// <param name="tag">オブジェクトの種別</param>
	/// <returns>格納しているコライダの数 : int</returns>
	const int GetColsNum(OBJECT_TYPE tag) const;

	// コライダ情報の配列取得
	const std::vector<std::shared_ptr<Collider>>& GetColliders(void);
	

private:

	//コライダー情報格納
	std::vector<std::shared_ptr<Collider>> cols_;

	//タグの設定されている数
	std::unordered_map<OBJECT_TYPE,int> colsNum_;

	//コライダの数初期化
	void ColsNumDef(void);

};

