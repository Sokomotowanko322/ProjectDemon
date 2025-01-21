#pragma once
#include <unordered_map>
#include <memory>
#include <functional>
#include <DxLib.h>
#include "ColliderTag.h"
#include "../Transform.h"

class Collider;
class Capsule;

class Collision
{

public:

	//モデル以外との判定時に返す情報
	struct HitColInfo
	{
		//VECTOR hitPos;	//衝突した座標(中心)
		VECTOR hitDir;		//衝突した方向(視点終点ともに中心)
		bool isHit;			//衝突したか

		HitColInfo() {
			hitDir = {};
			isHit = false;
		}
	};

	Collision(void);

	~Collision(void);

	/// <summary>
	/// 衝突判定結果を返す : モデル用
	/// </summary>
	/// <param name="colA">コライダー</param>
	/// <param name="colB">コライダー</param>
	/// <returns>MV1_COLL_RESULT_POLY_DIM</returns>
	const MV1_COLL_RESULT_POLY_DIM& GetCollResultPolyDim(
		std::weak_ptr<Collider> colA, std::weak_ptr<Collider> colB
	);

	/// <summary>
	/// 衝突判定結果を返す : モデル用
	/// </summary>
	/// <param name="objA">オブジェクト名</param>
	/// <param name="objB">オブジェクト名</param>
	/// <returns></returns>
	const MV1_COLL_RESULT_POLY GetCollResultPoly_Line(
		std::weak_ptr<Collider> colA, std::weak_ptr<Collider> colB);


	/// <summary>
	/// 球とカプセルの当たり判定
	/// </summary>
	/// <param name="colSphere">球コライダー</param>
	/// <param name="colCapsule">カプセルコライダー</param>
	/// <returns>コリジョン結果 HitColInfo</returns>
	const HitColInfo IsHitSphereCapsuleResult(
		std::weak_ptr<Collider> colA, 
		std::weak_ptr<Collider> colB
	);

	/// <summary>
	/// 球とカプセルの当たり判定
	/// </summary>
	/// <param name="colSphere">球コライダー</param>
	/// <param name="colCapsule">カプセルコライダー</param>
	/// <returns>コリジョン結果 HitColInfo</returns>
	const HitColInfo& IsHitCapsuleModelResult(
		std::weak_ptr<Collider> colA, 
		std::weak_ptr<Collider> colB
	);


	//ポリゴン情報の削除
	void CollResultPolyDimTerminate(MV1_COLL_RESULT_POLY_DIM& hitPoly);


private:

	//判定結果を格納 : 配列の解放が必要
	MV1_COLL_RESULT_POLY_DIM resultPolyDim_;

	//判定結果を格納しているか
	bool isStoreResult_;

	/// <summary>
	/// モデルと球の当たり判定
	/// </summary>
	/// <param name="colModel">モデル</param>
	/// <param name="colSphere">球</param>
	/// <returns>コリジョン結果 : MV1_COLL_RESULT_POLY_DIM</returns>
	const MV1_COLL_RESULT_POLY_DIM GetCollResultPolyDim_Sphere(
		std::weak_ptr<Collider> colModel,
		std::weak_ptr<Collider> colSphere
	) const;

	/// <summary>
	/// モデルとカプセルの当たり判定
	/// </summary>
	/// <param name="colModel">モデル</param>
	/// <param name="colSphere">カプセル</param>
	/// <returns>コリジョン結果 : MV1_COLL_RESULT_POLY_DIM</returns>
	const MV1_COLL_RESULT_POLY_DIM GetCollResultPolyDim_Capsule(
		std::weak_ptr<Collider> colModel,
		std::weak_ptr<Collider> colCapsule
	) const;

	/// <summary>
	/// 球とカプセルの当たり判定
	/// </summary>
	/// <param name="colModel">球</param>
	/// <param name="colSphere">カプセル</param>
	/// <returns>コリジョン結果 : MV1_COLL_RESULT_POLY_DIM</returns>
	const HitColInfo CollisionSphereCapsule(
		std::weak_ptr<Collider> colSphere,
		std::weak_ptr<Collider> colCapsule
	);

	/// <summary>
	/// モデルの衝突判定
	/// </summary>
	/// <param name="colModel">モデル</param>
	/// <param name="colA">衝突対象</param>
	/// <returns>コリジョン結果 : MV1_COLL_RESULT_POLY_DIM</returns>
	const MV1_COLL_RESULT_POLY_DIM& CollisionModel(
		std::weak_ptr<Collider> colModel,
		std::weak_ptr<Collider> colA
	);


	/// <summary>
	/// モデルの衝突判定
	/// </summary>
	/// <param name="colModel">線分</param>
	/// <param name="colA">モデル</param>
	/// <returns>コリジョン結果 : MV1_COLL_RESULT_POLY</returns>
	const MV1_COLL_RESULT_POLY CollisionLine(
		std::weak_ptr<Collider> colLine,
		std::weak_ptr<Collider> colModel
	) const;

	/// <summary>
	/// モデルの衝突用の関数を取得
	/// </summary>
	/// <param name="colModel">モデル</param>
	/// <param name="colA">衝突対象</param>
	/// <returns>衝突判定用の関数</returns>
	const std::function<MV1_COLL_RESULT_POLY_DIM(void)>GetCollModelFunc(
		std::weak_ptr<Collider> colModel,
		std::weak_ptr<Collider> colA
	) const;

	//衝突判定がカプセル型か判定
	const bool IsCapsule(std::weak_ptr<Collider> col) const;

	//衝突判定がモデル型か判定
	const bool IsModel(std::weak_ptr<Collider> col) const;

	//衝突判定が球型か判定
	const bool IsSphere(std::weak_ptr<Collider> col) const;

	//衝突判定が線分型か判定
	const bool IsLine(std::weak_ptr<Collider> col) const;

};

