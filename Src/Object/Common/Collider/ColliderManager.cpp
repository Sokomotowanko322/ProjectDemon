
#include "../Collider/Collider.h"
#include "ColliderManager.h"
#include <algorithm>


using namespace std;

const int INT_ZERO = 0;

const int INT_EMPTY = -1;

const int COLS_COUNT_ZERO = 0;

//�R���C�_�[���z��̃������m�ێ��̃o�b�t�@�[
const int COLS_VECTOR_BUFF = 6;

//�R���C�_�[�`�掞�̕�����
const int COL_DIV_DRAW = 4;

//�R���C�_�[�`�掞�̐F
const unsigned int COL_COLOR_DIF = 0x00ffff;
const unsigned int COL_COLOR_SPEC = 0xffffff;
const unsigned int COL_LINE_COLOR = 0xffffff;


ColliderManager::ColliderManager(void)
	:
	cols_{},
	colsNum_{}
{
	
	//�z��̃������m��
	cols_.reserve((size_t)((int)(OBJECT_TYPE::MAX) + COLS_VECTOR_BUFF));

	ColsNumDef();

}

ColliderManager::~ColliderManager(void)
{

	DelCollider();

}

void ColliderManager::Update(void)
{

	for (const auto& obj : cols_)
	{
		if (!obj)
		{
			//�R���C�_����������Ă��Ȃ�
			continue;
		}

		if (obj->GetFrameIdx() != INT_EMPTY)
		{
			//�t���[���ɒǏ]
			VECTOR Fpos = MV1GetFramePosition(obj->GetModelId(), obj->GetFrameIdx());
			obj->SetPosDown(Fpos);
			obj->SetPosTop(Fpos);
			obj->SetPosCenter(Fpos);
		}
		else
		{
			//���S���W���X�V
			obj->SetPosCenter(obj->GetPos());
		}

		if (obj->GetColType() != COL_TYPE::MODEL)
		{
			//���f�����g�p���Ă��Ȃ�
			continue;
		}
		
		//�R���C�_�X�V
		MV1RefreshCollInfo(obj->GetModelId());
	}

}

void ColliderManager::DrawCollider(OBJECT_TYPE type) const
{

	for (const auto& col : cols_)
	{
		if (col->GetColType() == COL_TYPE::SPHERE&&col->GetObjType()== type)
		{
			DrawSphere3D(
				col->GetPos(),
				col->GetRadius(), 
				COL_DIV_DRAW, 
				COL_COLOR_DIF,
				COL_COLOR_SPEC,
				false
			);
		}

		if (col->GetColType() == COL_TYPE::CAPSULE && col->GetObjType() == type)
		{
			DrawCapsule3D(
				col->GetPosTop(),
				col->GetPosDown(),
				col->GetRadius(),
				COL_DIV_DRAW,
				COL_COLOR_DIF,
				COL_COLOR_SPEC,
				false
			);
		}

		if (col->GetColType() == COL_TYPE::LINE && col->GetObjType() == type)
		{
			DrawLine3D(
				col->GetPosTop(),
				col->GetPosDown(), 
				COL_LINE_COLOR
			);
		}
	}

}

void ColliderManager::DrawColliderColor(OBJECT_TYPE type, int color) const
{

	for (const auto& col : cols_)
	{
		if (col->GetColType() == COL_TYPE::SPHERE && col->GetObjType() == type)
		{
			DrawSphere3D(
				col->GetPos(), 
				col->GetRadius(),
				COL_DIV_DRAW, 
				color,
				color,
				false
			);
		}

		if (col->GetColType() == COL_TYPE::CAPSULE && col->GetObjType() == type)
		{
			DrawCapsule3D(
				col->GetPosTop(),
				col->GetPosDown(),
				col->GetRadius(),
				COL_DIV_DRAW,
				color,
				color,
				false
			);
		}

	}

}


void ColliderManager::AddCollider(
	OBJECT_TYPE objType, COL_TYPE colType, const Transform& trans, 
	bool ableCol, int modelId, int frameIdx, ENEMY_SUBTYPE subType, float radius,
	const VECTOR& localPosTop,
	const VECTOR& localPosBottom
)
{

	shared_ptr<Collider> ret =
		make_shared<Collider>(trans, objType, subType,colType, ableCol, modelId, frameIdx);

	//���a
	ret->SetRadius(radius);

	//�t���[��
	ret->SetFrameIndex(frameIdx);

	if (frameIdx != INT_EMPTY)
	{
		ret->SetPosCenter(MV1GetFramePosition(trans.modelId, frameIdx));
	}
	else
	{
		//�J�v�Z���ネ�[�J�����W
		ret->SetPosTop(localPosTop);

		//�J�v�Z�������[�J�����W
		ret->SetPosDown(localPosBottom);

		ret->SetPosCenter(trans.pos);
	}

	if (colsNum_.contains(objType))
	{
		auto& num = colsNum_.at(objType);
		num++;
	}
	
	cols_.emplace_back(ret);

}

void ColliderManager::DelCollider(void)
{

	cols_.clear();

	for (auto& colNum : colsNum_)
	{
		colNum.second = COLS_COUNT_ZERO;
	}

}

void ColliderManager::DelCollider(OBJECT_TYPE tag)
{

	if (cols_.empty()) { return; }

	for (auto& info : cols_)
	{
		erase_if(
			cols_, [&](shared_ptr<Collider> col) { return tag == col->GetObjType(); }
		);
	}

	if (colsNum_.contains(tag))
	{
		auto& num = colsNum_.at(tag);
		if (num == COLS_COUNT_ZERO) { return; }
		num = COLS_COUNT_ZERO;
	}

}

const int ColliderManager::GetColsNum(OBJECT_TYPE tag) const
{

	if (!colsNum_.contains(tag))
	{
		//�R���C�_�[���z��Ɋi�[����Ă��Ȃ�
		return COLS_COUNT_ZERO;
	}

	return colsNum_.at(tag);

}

const std::vector<std::shared_ptr<Collider>>& ColliderManager::GetColliders(void)
{

	return cols_;

}

void ColliderManager::ColsNumDef(void)
{

	for (int tag = INT_ZERO; tag < (int)OBJECT_TYPE::MAX; tag++)
	{
		colsNum_.emplace((OBJECT_TYPE)tag, COLS_COUNT_ZERO);
	}

}
