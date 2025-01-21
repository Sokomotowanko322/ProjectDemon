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

	//���f���ȊO�Ƃ̔��莞�ɕԂ����
	struct HitColInfo
	{
		//VECTOR hitPos;	//�Փ˂������W(���S)
		VECTOR hitDir;		//�Փ˂�������(���_�I�_�Ƃ��ɒ��S)
		bool isHit;			//�Փ˂�����

		HitColInfo() {
			hitDir = {};
			isHit = false;
		}
	};

	Collision(void);

	~Collision(void);

	/// <summary>
	/// �Փ˔��茋�ʂ�Ԃ� : ���f���p
	/// </summary>
	/// <param name="colA">�R���C�_�[</param>
	/// <param name="colB">�R���C�_�[</param>
	/// <returns>MV1_COLL_RESULT_POLY_DIM</returns>
	const MV1_COLL_RESULT_POLY_DIM& GetCollResultPolyDim(
		std::weak_ptr<Collider> colA, std::weak_ptr<Collider> colB
	);

	/// <summary>
	/// �Փ˔��茋�ʂ�Ԃ� : ���f���p
	/// </summary>
	/// <param name="objA">�I�u�W�F�N�g��</param>
	/// <param name="objB">�I�u�W�F�N�g��</param>
	/// <returns></returns>
	const MV1_COLL_RESULT_POLY GetCollResultPoly_Line(
		std::weak_ptr<Collider> colA, std::weak_ptr<Collider> colB);


	/// <summary>
	/// ���ƃJ�v�Z���̓����蔻��
	/// </summary>
	/// <param name="colSphere">���R���C�_�[</param>
	/// <param name="colCapsule">�J�v�Z���R���C�_�[</param>
	/// <returns>�R���W�������� HitColInfo</returns>
	const HitColInfo IsHitSphereCapsuleResult(
		std::weak_ptr<Collider> colA, 
		std::weak_ptr<Collider> colB
	);

	/// <summary>
	/// ���ƃJ�v�Z���̓����蔻��
	/// </summary>
	/// <param name="colSphere">���R���C�_�[</param>
	/// <param name="colCapsule">�J�v�Z���R���C�_�[</param>
	/// <returns>�R���W�������� HitColInfo</returns>
	const HitColInfo& IsHitCapsuleModelResult(
		std::weak_ptr<Collider> colA, 
		std::weak_ptr<Collider> colB
	);


	//�|���S�����̍폜
	void CollResultPolyDimTerminate(MV1_COLL_RESULT_POLY_DIM& hitPoly);


private:

	//���茋�ʂ��i�[ : �z��̉�����K�v
	MV1_COLL_RESULT_POLY_DIM resultPolyDim_;

	//���茋�ʂ��i�[���Ă��邩
	bool isStoreResult_;

	/// <summary>
	/// ���f���Ƌ��̓����蔻��
	/// </summary>
	/// <param name="colModel">���f��</param>
	/// <param name="colSphere">��</param>
	/// <returns>�R���W�������� : MV1_COLL_RESULT_POLY_DIM</returns>
	const MV1_COLL_RESULT_POLY_DIM GetCollResultPolyDim_Sphere(
		std::weak_ptr<Collider> colModel,
		std::weak_ptr<Collider> colSphere
	) const;

	/// <summary>
	/// ���f���ƃJ�v�Z���̓����蔻��
	/// </summary>
	/// <param name="colModel">���f��</param>
	/// <param name="colSphere">�J�v�Z��</param>
	/// <returns>�R���W�������� : MV1_COLL_RESULT_POLY_DIM</returns>
	const MV1_COLL_RESULT_POLY_DIM GetCollResultPolyDim_Capsule(
		std::weak_ptr<Collider> colModel,
		std::weak_ptr<Collider> colCapsule
	) const;

	/// <summary>
	/// ���ƃJ�v�Z���̓����蔻��
	/// </summary>
	/// <param name="colModel">��</param>
	/// <param name="colSphere">�J�v�Z��</param>
	/// <returns>�R���W�������� : MV1_COLL_RESULT_POLY_DIM</returns>
	const HitColInfo CollisionSphereCapsule(
		std::weak_ptr<Collider> colSphere,
		std::weak_ptr<Collider> colCapsule
	);

	/// <summary>
	/// ���f���̏Փ˔���
	/// </summary>
	/// <param name="colModel">���f��</param>
	/// <param name="colA">�ՓˑΏ�</param>
	/// <returns>�R���W�������� : MV1_COLL_RESULT_POLY_DIM</returns>
	const MV1_COLL_RESULT_POLY_DIM& CollisionModel(
		std::weak_ptr<Collider> colModel,
		std::weak_ptr<Collider> colA
	);


	/// <summary>
	/// ���f���̏Փ˔���
	/// </summary>
	/// <param name="colModel">����</param>
	/// <param name="colA">���f��</param>
	/// <returns>�R���W�������� : MV1_COLL_RESULT_POLY</returns>
	const MV1_COLL_RESULT_POLY CollisionLine(
		std::weak_ptr<Collider> colLine,
		std::weak_ptr<Collider> colModel
	) const;

	/// <summary>
	/// ���f���̏Փ˗p�̊֐����擾
	/// </summary>
	/// <param name="colModel">���f��</param>
	/// <param name="colA">�ՓˑΏ�</param>
	/// <returns>�Փ˔���p�̊֐�</returns>
	const std::function<MV1_COLL_RESULT_POLY_DIM(void)>GetCollModelFunc(
		std::weak_ptr<Collider> colModel,
		std::weak_ptr<Collider> colA
	) const;

	//�Փ˔��肪�J�v�Z���^������
	const bool IsCapsule(std::weak_ptr<Collider> col) const;

	//�Փ˔��肪���f���^������
	const bool IsModel(std::weak_ptr<Collider> col) const;

	//�Փ˔��肪���^������
	const bool IsSphere(std::weak_ptr<Collider> col) const;

	//�Փ˔��肪�����^������
	const bool IsLine(std::weak_ptr<Collider> col) const;

};

