
//#include <Dxlib.h>
//#include "Collider.h"
#include "../Transform.h"
#include "Collider.h"

Collider::Collider(const Transform& transform) 
	:
	transform_(transform),
	colData_(ColliderData())
{

}

Collider::Collider(
	const Transform& transform,
	OBJECT_TYPE objType,
	COL_TYPE colType,
	bool ableCol,
	int modelId,
	int frameIdx
)
	:
	transform_(transform),
	colData_(ColliderData())
{

	colData_.objType_ = objType;
	colData_.colType_ = colType;
	colData_.ableCol_ = ableCol;
	colData_.colModelId_ = transform.modelId;
	colData_.colModelFrameIdx_ = frameIdx;

}

Collider::Collider(
	const Transform& transform,
	OBJECT_TYPE objType, 
	ENEMY_SUBTYPE subType, 
	COL_TYPE colType, 
	bool ableCol, 
	int modelId, 
	int frameIdx
) 
	:
	transform_(transform),
	colData_(ColliderData())
{

	colData_.objType_ = objType;
	colData_.subType_ = subType;
	colData_.colType_ = colType;
	colData_.ableCol_ = ableCol;
	colData_.colModelId_ = transform.modelId;
	colData_.colModelFrameIdx_ = frameIdx;

}

Collider::~Collider(void)
{

}


ENEMY_SUBTYPE Collider::GetObjSubType(void)
{

	return colData_.subType_;

}

VECTOR Collider::GetPosTop(void) const
{

	VECTOR ret = GetRotPos(colData_.localPosTop_);

	return ret;

}

VECTOR Collider::GetPosDown(void) const
{

	VECTOR ret = GetRotPos(colData_.localPosBottom_);

	return ret;

}

const VECTOR& Collider::GetPos(void) const
{

	if (colData_.colModelFrameIdx_ != -1)
	{
		return colData_.localPosCenter_;
	}

	return transform_.pos;

}

const VECTOR& Collider::GetCenterPos(void) const
{

	return colData_.localPosCenter_;

}

const float Collider::GetRadius(void) const
{

	return colData_.radius_;

}

void Collider::SetPosTop(const VECTOR& val)
{

	colData_.localPosTop_ = val;

}

void Collider::SetPosDown(const VECTOR& val)
{

	colData_.localPosBottom_ = val;

}

void Collider::SetPosCenter(const VECTOR& val)
{

	colData_.localPosCenter_ = val;

}

void Collider::SetRadius(const float& val)
{

	colData_.radius_ = val;

}

void Collider::SetFrameIndex(const int& val)
{

	colData_.colModelFrameIdx_ = val;

}

void Collider::SetColAble(const bool val)
{

	colData_.ableCol_ = val;

}

VECTOR Collider::GetRotPos(const VECTOR& localPos) const
{

	MATRIX mat = MGetIdent();
	mat = MMult(mat, transform_.matScl);

	Quaternion q = transform_.quaRot.Mult(transform_.quaRotLocal);
	mat = MMult(mat, q.ToMatrix());

	VECTOR localRotPos = q.PosAxis(localPos);

	return VAdd(transform_.pos, localRotPos);

}

