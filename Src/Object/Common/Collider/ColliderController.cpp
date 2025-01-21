

#include <Dxlib.h>
#include "../../../Manager/SceneManager.h"
#include "Collision.h"
#include "ColliderManager.h"
#include "ColliderController.h"

using namespace std;

const int INT_NOT_HIT = 0;

ColliderController::ColliderController(void)
	:
	objectType_(OBJECT_TYPE::NONE),
	subType_(ENEMY_SUBTYPE::NONE),
	collision_(nullptr),
	colMng_(SceneManager::GetInstance().GetColManager())
{

	collision_ = make_unique<Collision>();

}

ColliderController::~ColliderController(void)
{

}

void ColliderController::SetCollision(OBJECT_TYPE obj)
{
	objectType_ = obj;
}

void ColliderController::SetCollisionSub(ENEMY_SUBTYPE subobj)
{
	subType_ = subobj;
}

void ColliderController::SetCollisionAble(OBJECT_TYPE obj, bool val)
{

	std::weak_ptr<Collider> colA;

	for (const auto& col : colMng_.GetColliders())
	{
		if (col->GetObjType() == objectType_ && col->GetObjSubType() == subType_)//í«â¡
		{
			colA = col;
			colA.lock()->SetColAble(val);
			continue;
		}
	}
}

const bool ColliderController::OnCollision(OBJECT_TYPE obj)
{

	//ï°êîÇÃèÍçáÅAobjBë§Ç™ï°êîÇ…Ç»ÇÈÇÊÇ§Ç…

	std::weak_ptr<Collider> colA;
	std::vector<std::weak_ptr<Collider>> colB;

	for (const auto& col : colMng_.GetColliders())
	{
		if (!colA.expired() && (int)colB.size() == colMng_.GetColsNum(obj))
		{
			break;
		}

		if (col->GetObjType() == objectType_ && col->GetObjSubType() == subType_)
		{
			colA = col;
			continue;
		}

		if (col->GetObjType() == obj)
		{
			colB.emplace_back(col);
			continue;
		}

	}

	if (!colA.lock()->GetAbleCol())
	{
		colB.clear();

		return false;
	}

	int size = (int)colB.size();
	for (int b = 0 ; b < size ; b++)
	{
		if (!colB[b].lock()->GetAbleCol()) { continue; }

		if (IsHit(colA, colB[b]))
		{
			colB.clear();
			return true;
		}
	}

	colB.clear();

	return false;

}

const MV1_COLL_RESULT_POLY_DIM ColliderController::OnCollisionResultPoly(OBJECT_TYPE obj)
{

	std::weak_ptr<Collider> colA;
	std::weak_ptr<Collider> colB;

	for (const auto& col : colMng_.GetColliders())
	{
		if (!colA.expired() && !colB.expired())
		{
			break;
		}

		if (col->GetObjType() == obj)
		{
			colA = col;
			continue;
		}

		if (col->GetObjType() == objectType_ && col->GetObjSubType() == subType_)//í«â¡
		{
			colB = col;
			continue;
		}
	}

	if (!colA.lock()->GetAbleCol() || !colB.lock()->GetAbleCol()) 
	{
		return MV1_COLL_RESULT_POLY_DIM(); 
	}

	auto& hit = collision_->GetCollResultPolyDim(colA, colB);

	if (hit.HitNum > INT_NOT_HIT) { return hit; }

	
	return MV1_COLL_RESULT_POLY_DIM();
	
}

const MV1_COLL_RESULT_POLY ColliderController::OnCollisionResultPoly_Line(OBJECT_TYPE obj)
{

	std::weak_ptr<Collider> colA;
	std::weak_ptr<Collider> colB;

	for (const auto& col : colMng_.GetColliders())
	{
		if (!colA.expired() && !colB.expired())
		{
			break;
		}

		if (col->GetObjType() == obj)
		{
			colA = col;
			continue;
		}

		if (col->GetObjType() == objectType_ && col->GetObjSubType() == subType_)//í«â¡
		{
			colB = col;
			continue;
		}
	}

	auto& hit = collision_->GetCollResultPoly_Line(colA, colB);

	if (hit.HitFlag)
	{
		return hit;
	}

	return MV1_COLL_RESULT_POLY();

}

const VECTOR ColliderController::OnCollisionResultHitDir(OBJECT_TYPE obj)
{

	std::weak_ptr<Collider> colA;
	std::vector<std::weak_ptr<Collider>> colB;

	for (const auto& col : colMng_.GetColliders())
	{
		if (!colA.expired() && (int)colB.size() == colMng_.GetColsNum(obj))
		{
			break;
		}

		if (col->GetObjType() == objectType_ && col->GetObjSubType() == subType_)//í«â¡
		{
			colA = col;
			continue;
		}

		if (col->GetObjType() == obj)
		{
			colB.emplace_back(col);
			continue;
		}

	}

	int size = (int)colB.size();
	for (int b = 0; b < size; b++)
	{
		if (!colB[b].lock()->GetAbleCol()) { continue; }

		if (IsHit(colA, colB[b]))
		{
			return collision_->IsHitSphereCapsuleResult(colA, colB[b]).hitDir;
		}
	}


	colB.clear();
	return VECTOR();

}

const int ColliderController::OnCollisionResultPolyTriangle(const MV1_COLL_RESULT_POLY& poly)
{
	std::weak_ptr<Collider> colA;
	std::weak_ptr<Collider> colB;

	for (const auto& col : colMng_.GetColliders())
	{
		if (!colA.expired() && !colB.expired())
		{
			break;
		}

		if (col->GetObjType() == objectType_ && col->GetObjSubType() == subType_)//í«â¡
		{
			colA = col;
			continue;
		}

	}

	return HitCheck_Capsule_Triangle(
		colA.lock()->GetPosTop(),
		colA.lock()->GetPosDown(),
		colA.lock()->GetRadius(),
		poly.Position[0], poly.Position[1], poly.Position[2]
	);
}

void ColliderController::CollResultPolyDimTerminate(MV1_COLL_RESULT_POLY_DIM& poly)
{
	MV1CollResultPolyDimTerminate(poly);
}

const bool ColliderController::IsHit(std::weak_ptr<Collider> colA, std::weak_ptr<Collider> colB)
{

	if (!IsModel(colA) && !IsModel(colB))
	{
		auto& hit = collision_->IsHitSphereCapsuleResult(colA, colB);

		if (hit.isHit)
		{
			return true;
		}
	}
	

	if (IsModel(colA) || IsModel(colB))
	{
		auto& hit = collision_->GetCollResultPolyDim(colA, colB);

		if (hit.HitNum > 0)
		{
			return true;
		}
	}

	return false;

}

const bool ColliderController::IsModel(std::weak_ptr<Collider> col) const
{
	return col.lock()->GetColType() == COL_TYPE::MODEL;
}

const bool ColliderController::IsSphere(std::weak_ptr<Collider> col) const
{
	return col.lock()->GetColType() == COL_TYPE::SPHERE;
}

const bool ColliderController::IsCapsule(std::weak_ptr<Collider> col) const
{
	return col.lock()->GetColType() == COL_TYPE::CAPSULE;
}

const bool ColliderController::IsLine(std::weak_ptr<Collider> col) const
{
	return col.lock()->GetColType() == COL_TYPE::LINE;
}