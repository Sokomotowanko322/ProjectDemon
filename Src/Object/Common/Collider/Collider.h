#pragma once

#include <Dxlib.h>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "ColliderTag.h"

class Transform;

/// <summary>
/// �R���C�_����ێ�����N���X
/// </summary>
class Collider
{

public:

	/// <summary>
	/// �R���C�_���̍\����
	/// </summary>
	struct ColliderData
	{
		OBJECT_TYPE objType_;		//�I�u�W�F�N�g��
		ENEMY_SUBTYPE subType_;		//�I�u�W�F�N�g�̃t���[����
		COL_TYPE colType_;			//�Փ˔���̎��
		int colModelId_;			//���f��id
		int colModelFrameIdx_;		//�t���[���ԍ�
		bool ableCol_;				//�Փ˔��肪�L����

		float radius_;				//���a
		VECTOR localPosTop_;		//��[���[�J�����W
		VECTOR localPosBottom_;		//���[���[�J�����W
		VECTOR localPosCenter_;		//���S���W

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
	/// �f�t�H���g�R���X�g���N�^
	/// </summary>
	Collider(const Transform& transform);
	
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="transform">�Ǐ]���郂�f�����</param>
	/// <param name="colType">�����蔻��̎��</param>
	/// <param name="modelId">���f��ID : �K�v�Ȃ��ꍇ-1</param>
	/// <param name="frameIdx">�t���[���ԍ� : �K�v�Ȃ��ꍇ-1</param>
	Collider(
		const Transform& transform,
		OBJECT_TYPE objType, COL_TYPE colType, bool ableCol ,int modelId = -1, int frameIdx = -1);
	

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="transform">�Ǐ]���郂�f�����</param>
	/// <param name="objType">�I�u�W�F�N�g��</param>
	/// <param name="subType">�I�u�W�F�N�g�̃t���[����</param>
	/// <param name="colType">�Փ˔���̎��</param>
	/// <param name="ableCol">�Փ˔�����s�����̃t���O</param>
	/// <param name="modelId">���f��ID : �K�v�Ȃ��ꍇ-1</param>
	/// <param name="frameIdx">�t���[���ԍ� : �K�v�Ȃ��ꍇ-1</param>
	Collider(
		const Transform& transform,
		OBJECT_TYPE objType,
		ENEMY_SUBTYPE subType,
		COL_TYPE colType,
		bool ableCol, int modelId = -1, int frameIdx = -1);
	
	~Collider(void);

	/// <summary>
	/// �����蔻��̎�ނ��擾
	/// </summary>
	/// <returns>COL_TYPE</returns>
	inline const COL_TYPE GetColType(void) const
	{
		return colData_.colType_;
	}

	/// <summary>
	/// ���f���̃n���h��ID���擾
	/// </summary>
	/// <returns>���f���̃n���h��ID</returns>
	inline const int GetModelId(void)
	{
		return colData_.colModelId_;
	}

	/// <summary>
	/// �Ǐ]����t���[���ԍ����擾
	/// </summary>
	/// <returns>�Ǐ]����t���[���ԍ����擾</returns>
	const int GetFrameIdx(void) const
	{
		return colData_.colModelFrameIdx_;
	}


	/// <summary>
	/// �Փ˔�����s�����̃t���O���擾
	/// </summary>
	/// <returns>
	/// true : �s��
	/// false : �s��Ȃ�
	/// </returns>
	const bool GetAbleCol(void) const
	{
		return colData_.ableCol_;
	}

	/// <summary>
	/// �R���C�_�����擾
	/// </summary>
	/// <returns>ColliderData</returns>
	const ColliderData& GetColliderData(void) const
	{
		return colData_;
	}
		

	/// <summary>
	/// �I�u�W�F�N�g�����擾
	/// </summary>
	/// <returns>colData_.objType_</returns>
	inline const OBJECT_TYPE GetObjType(void)
	{
		return colData_.objType_;
	}

	/// <summary>
	/// �I�u�W�F�N�g�̃t���[�������擾
	/// </summary>
	/// <returns>ENEMY_SUBTYPE</returns>
	ENEMY_SUBTYPE GetObjSubType(void);

	/// <summary>
	/// ��[�̃��[���h���W���擾
	/// </summary>
	/// <returns>VECTOR</returns>
	VECTOR GetPosTop(void) const;

	/// <summary>
	/// ���[�̃��[���h���W���擾
	/// </summary>
	/// <returns>VECTOR</returns>
	VECTOR GetPosDown(void) const;
	
	/// <summary>
	/// ���[���h���W���擾
	/// </summary>
	/// <returns>VECTOR</returns>
	const VECTOR& GetPos(void) const;

	/// <summary>
	/// ���S�̃��[���h���W���擾
	/// </summary>
	/// <returns>
	/// �Ǐ]�t���[�������� : colData_.localPosCenter_
	/// �Ǐ]�t���[�������݂��ĂȂ� : colData_.transform_.pos
	/// </returns>
	const VECTOR& GetCenterPos(void) const;

	/// <summary>
	/// ���a���擾
	/// </summary>
	/// <returns>colData_.radius_</returns>
	const float GetRadius(void) const;

	/// <summary>
	/// ��[�̃��[�J�����W��ݒ�
	/// </summary>
	/// <param name="val">��[�̃��[�J�����W</param>
	void SetPosTop(const VECTOR& val);
	
	/// <summary>
	/// ���[�̃��[�J�����W��ݒ�
	/// </summary>
	/// <param name="val">���[�̃��[�J�����W</param>
	void SetPosDown(const VECTOR& val);
	
	/// <summary>
	/// ���S���W��ݒ�
	/// </summary>
	/// <param name="val">���S���W</param>
	void SetPosCenter(const VECTOR& val);

	/// <summary>
	/// ���a��ݒ�
	/// </summary>
	/// <param name="val">���a</param>
	void SetRadius(const float& val);

	/// <summary>
	/// �t���[���ԍ���ݒ�
	/// </summary>
	/// <param name="val">�t���[���ԍ�</param>
	void SetFrameIndex(const int& val);

	/// <summary>
	/// �����蔻���L���ɂ��邩�ݒ�
	/// </summary>
	/// <param name="val">
	/// true : �L��
	/// false : ����
	/// </param>
	void SetColAble(const bool val);
	
	
	/// <summary>
	/// ���΍��W����]�����ă��[���h���W�Ŏ擾����
	/// </summary>
	/// <param name="localPos">���΍��W</param>
	/// <returns>��]��̃��[���h���W</returns>
	VECTOR GetRotPos(const VECTOR& localPos) const;


private:

	//�Ǐ]���郂�f���̊�{���
	const Transform& transform_;

	//�R���C�_���̍\����
	ColliderData colData_;

};

