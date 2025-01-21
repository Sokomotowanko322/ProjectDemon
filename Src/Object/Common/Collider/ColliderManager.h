#pragma once

#include <vector>
#include "../Transform.h"
#include "Collider.h"

class ColliderManager
{

public:

	ColliderManager(void);
	~ColliderManager(void);

	//�R���C�_���X�V
	void Update(void);

	//�f�o�b�O�`��
	void DrawCollider(OBJECT_TYPE type) const;
	void DrawColliderColor(OBJECT_TYPE type,int color) const;

	/// <summary>
	/// �R���C�_���ǉ�
	/// </summary>
	/// <param name="objType">�I�u�W�F�N�g�̎��</param>
	/// <param name="colType">�Փ˔���̎��</param>
	/// <param name="trans">�I�u�W�F�N�g�̃��f��������</param>
	/// <param name="ableCol">�R���C�_�[�L���� : �v�m�F</param>
	/// <param name="modelId"></param>
	/// <param name="frameIdx">���f���t���[��</param>
	/// <param name="radius">���a</param>
	/// <param name="localPosTop">�J�v�Z���p : ���[�J�����W��</param>
	/// <param name="localPosBottom">�J�v�Z���p : ���[�J�����W��</param>
	void AddCollider(
		OBJECT_TYPE objType,COL_TYPE colType, const Transform& trans,
		bool ableCol, int modelId = -1, int frameIdx = -1, 
		ENEMY_SUBTYPE objtype = ENEMY_SUBTYPE::NONE,
		float radius = 1.0f,
		const VECTOR& localPosTop = { 0.0f,0.0f,0.0f },
		const VECTOR& localPosBottom = { 0.0f,0.0f,0.0f }
	);

	//�R���C�_�폜
	void DelCollider(void);

	/// <summary>
	/// �R���C�_�폜
	/// </summary>
	/// <param name="tag">�I�u�W�F�N�g�̎��</param>
	void DelCollider(OBJECT_TYPE tag);

	/// <summary>
	/// �i�[���Ă���R���C�_�̐��擾
	/// </summary>
	/// <param name="tag">�I�u�W�F�N�g�̎��</param>
	/// <returns>�i�[���Ă���R���C�_�̐� : int</returns>
	const int GetColsNum(OBJECT_TYPE tag) const;

	// �R���C�_���̔z��擾
	const std::vector<std::shared_ptr<Collider>>& GetColliders(void);
	

private:

	//�R���C�_�[���i�[
	std::vector<std::shared_ptr<Collider>> cols_;

	//�^�O�̐ݒ肳��Ă��鐔
	std::unordered_map<OBJECT_TYPE,int> colsNum_;

	//�R���C�_�̐�������
	void ColsNumDef(void);

};

