
#include <cassert>
//#include "Capsule.h"
#include "../../../Utility/Utility.h"
#include "Collision.h"

using namespace std;

Collision::Collision(void)
	:
	resultPolyDim_{},
	isStoreResult_(false)
{

}

Collision::~Collision(void)
{

	if (isStoreResult_)
	{
		//�Փ˔���̌��ʂ̌�n��
		MV1CollResultPolyDimTerminate(resultPolyDim_);
		isStoreResult_ = false;
	}

}

const MV1_COLL_RESULT_POLY_DIM& Collision::GetCollResultPolyDim(
	std::weak_ptr<Collider> colA, std::weak_ptr<Collider> colB)
{

	//�Е������f���^�łȂ��Ȃ�A�����Е������f���^
	return IsModel(colA) ?
		CollisionModel(colA, colB) : CollisionModel(colB, colA);

}

const MV1_COLL_RESULT_POLY Collision::GetCollResultPoly_Line(
	std::weak_ptr<Collider> colA, std::weak_ptr<Collider> colB)
{

	//�Е��������^�łȂ��Ȃ�A�����Е��������^
	return IsLine(colA) ? 
		CollisionLine(colA, colB) : CollisionLine(colB, colA);

}

const MV1_COLL_RESULT_POLY_DIM Collision::GetCollResultPolyDim_Capsule(
	std::weak_ptr<Collider> colModel, std::weak_ptr<Collider> colCapsule)const
{

	MV1_COLL_RESULT_POLY_DIM hitDim = MV1CollCheck_Capsule(
		colModel.lock()->GetModelId(),
		colModel.lock()->GetFrameIdx(),
		colCapsule.lock()->GetPosTop(),
		colCapsule.lock()->GetPosDown(),
		colCapsule.lock()->GetRadius()
	);

	if (hitDim.HitNum > 0)
	{
		return hitDim;
	}

	return MV1_COLL_RESULT_POLY_DIM();

}

const Collision::HitColInfo Collision::IsHitSphereCapsuleResult(
	std::weak_ptr<Collider> colA, std::weak_ptr<Collider> colB)
{

	//�Е������^�łȂ��Ȃ�A�����Е������^
	return IsSphere(colA) ? 
		CollisionSphereCapsule(colA, colB) : CollisionSphereCapsule(colB, colA);

}

void Collision::CollResultPolyDimTerminate(MV1_COLL_RESULT_POLY_DIM& hitPoly)
{

	// ���o�����n�ʃ|���S�����̌�n��
	MV1CollResultPolyDimTerminate(hitPoly);

}

const MV1_COLL_RESULT_POLY_DIM Collision::GetCollResultPolyDim_Sphere(
	std::weak_ptr<Collider> colModel, std::weak_ptr<Collider> colCapsule)const
{

	MV1_COLL_RESULT_POLY_DIM hitDim = MV1CollCheck_Sphere(
		colModel.lock()->GetModelId(),
		colModel.lock()->GetFrameIdx(),
		colCapsule.lock()->GetPos(),
		colCapsule.lock()->GetRadius()
	);

	if (hitDim.HitNum > 0)
	{
		return hitDim;
	}

	return MV1_COLL_RESULT_POLY_DIM();

}

const Collision::HitColInfo Collision::CollisionSphereCapsule(
	std::weak_ptr<Collider> colSphere, std::weak_ptr<Collider> colCapsule)
{

	HitColInfo hitCol;

	bool ret = false;

	//VECTOR hitpos;
	VECTOR hitDir = {};

	// ���̓��m�̓����蔻��
	if (Utility::IsHitSphereCapsule(
		colSphere.lock()->GetPos(),
		colSphere.lock()->GetRadius(),
		colCapsule.lock()->GetPosTop(),
		colCapsule.lock()->GetPosDown(),
		colCapsule.lock()->GetRadius()
	))
	{
		hitDir = VSub(
			colSphere.lock()->GetPos(), colCapsule.lock()->GetPos());
		hitDir = VNorm(hitDir);
		ret = true;
	}
	else
	{
		ret = false;
	}

	hitCol.hitDir = hitDir;
	hitCol.isHit = ret;

	return hitCol;

}

const MV1_COLL_RESULT_POLY_DIM& Collision::CollisionModel(
	std::weak_ptr<Collider> colModel, std::weak_ptr<Collider> colA)
{

	if (isStoreResult_)
	{
		//�Փ˔���̌��ʂ̌�n��
		MV1CollResultPolyDimTerminate(resultPolyDim_);

		//�Փ˔���
		resultPolyDim_ = GetCollModelFunc(colModel, colA)();

		return resultPolyDim_;
	}

	isStoreResult_ = true;

	std::function<MV1_COLL_RESULT_POLY_DIM(void)> func = GetCollModelFunc(colModel, colA);
	//�Փ˔���
	resultPolyDim_ = func();

	return resultPolyDim_;

}

const MV1_COLL_RESULT_POLY Collision::CollisionLine(
	std::weak_ptr<Collider> colLine,
	std::weak_ptr<Collider> colModel
)const
{

	MV1_COLL_RESULT_POLY hitPoly = MV1CollCheck_Line(
		colModel.lock()->GetModelId(), colModel.lock()->GetFrameIdx(),
		colLine.lock()->GetPosTop(), colLine.lock()->GetPosDown()
	);

	if (hitPoly.HitFlag)
	{
		return hitPoly;
	}

	return MV1_COLL_RESULT_POLY();

}

const std::function<MV1_COLL_RESULT_POLY_DIM(void)> Collision::GetCollModelFunc(
	std::weak_ptr<Collider> colModel, std::weak_ptr<Collider> colA)const
{

	std::function<MV1_COLL_RESULT_POLY_DIM(void)> ret;

	if (IsSphere(colA))
	{
		//���^�̏Փ˔���
		ret = [&]() {
			return GetCollResultPolyDim_Sphere(colModel, colA);
		};
	}
	else if (IsCapsule(colA))
	{
		//�J�v�Z���^�̏Փ˔���
		ret = [&]() {
			return GetCollResultPolyDim_Capsule(colModel, colA);
		};
	}

	return ret;

}

const bool Collision::IsCapsule(std::weak_ptr<Collider> col) const
{

	return col.lock()->GetColType() == COL_TYPE::CAPSULE;

}

const bool Collision::IsModel(std::weak_ptr<Collider> col) const
{

	return col.lock()->GetColType() == COL_TYPE::MODEL;

}

const bool Collision::IsSphere(std::weak_ptr<Collider> col) const
{

	return col.lock()->GetColType() == COL_TYPE::SPHERE;

}

const bool Collision::IsLine(std::weak_ptr<Collider> col) const
{

	return col.lock()->GetColType() == COL_TYPE::LINE;

}





