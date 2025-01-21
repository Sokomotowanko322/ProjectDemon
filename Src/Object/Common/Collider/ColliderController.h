#pragma once

#include "Collider.h"

class SceneManager;
class ColliderManager;
class Collision;


class ColliderController
{

public:

	ColliderController(void);
	~ColliderController(void);

	/// <summary>
	/// 衝突判定の登録
	/// </summary>
	/// <param name="obj">自身のオブジェクト名</param>
	void SetCollision(OBJECT_TYPE obj);
	void SetCollisionSub(ENEMY_SUBTYPE subobj);

	//消失判定のフラグを更新
	void SetCollisionAble(OBJECT_TYPE obj,bool val);



	/// <summary>
	/// 登録されたオブジェクトと衝突しているか
	/// </summary>
	/// <param name="obj">衝突判定を行うオブジェクト</param>
	/// <returns></returns>
	const bool OnCollision(OBJECT_TYPE obj);
	
	/// <summary>
	/// コリジョン
	/// </summary>
	/// <param name="obj">衝突判定を行うオブジェクト</param>
	/// <returns>衝突している場合 : MV1_COLL_RESULT_POLY</returns>
	const MV1_COLL_RESULT_POLY_DIM OnCollisionResultPoly(OBJECT_TYPE obj);

	const MV1_COLL_RESULT_POLY OnCollisionResultPoly_Line(OBJECT_TYPE obj);
	
	
	const VECTOR OnCollisionResultHitDir(OBJECT_TYPE obj);

	//ポリゴンとの判定を取る
	const int OnCollisionResultPolyTriangle(const MV1_COLL_RESULT_POLY& poly);

	void CollResultPolyDimTerminate(MV1_COLL_RESULT_POLY_DIM& poly);


private:

	ColliderManager& colMng_;

	//登録する衝突判定
	OBJECT_TYPE objectType_;

	//追従フレーム
	ENEMY_SUBTYPE subType_;

	//衝突判定を行うクラス
	std::unique_ptr<Collision> collision_;

	const bool IsHit(std::weak_ptr<Collider> colA, std::weak_ptr<Collider> colB);

	const bool IsModel(std::weak_ptr<Collider> col) const;
	const bool IsSphere(std::weak_ptr<Collider> col) const;
	const bool IsCapsule(std::weak_ptr<Collider> col) const;
	const bool IsLine(std::weak_ptr<Collider> col) const;

};

