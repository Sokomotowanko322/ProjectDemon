#include <DxLib.h>
#include "../Utility/Utility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Renderer/ModelMaterial.h"
#include "../Renderer/ModelRenderer.h"
#include "Common/Transform.h"
#include "Planet.h"

Planet::Planet(const Stage::NAME& name, const TYPE& type, const Transform& transform)
{
	name_ = name;
	type_ = type;
	transform_ = transform;
}

Planet::~Planet(void)
{
}

void Planet::Init(void)
{

	// ���f���`��p
	material_ = std::make_unique<ModelMaterial>(
		"StdModelVS4.cso", 2,
		"StdModelPS4.cso", 3
		);

	// �g�U��
	material_->AddConstBufPS({ 1.0f, 1.0f, 1.0f, 1.0f });

	// ����
	material_->AddConstBufPS({ 0.2f, 0.2f, 0.2f, 1.0f });
	
	// ���_�@�F�J�������W
	auto& camera = SceneManager::GetInstance().GetCamera();
	auto cPos = camera.GetPos();
	material_->AddConstBufVS({ cPos.x, cPos.y, cPos.z, 0.0f });

	// ���_�A�F�t�H�O�J�n�E�I������
	float fogStart;
	float fogEnd;
	GetFogStartEnd(&fogStart, &fogEnd);
	material_->AddConstBufVS({ fogStart, fogEnd, 0.0f, 0.0f });
	
	// ���C�g�̕���
	VECTOR worldLightDirection = GetLightDirection();
	material_->AddConstBufPS(
		{ worldLightDirection.x, worldLightDirection.y, worldLightDirection.z, 0.0f });

	renderer_ = std::make_unique<ModelRenderer>(transform_.modelId, *material_);

}

void Planet::Update(void)
{
}

void Planet::Draw(void)
{

	// ���_�F�J�������W
	auto& camera = SceneManager::GetInstance().GetCamera();
	auto cPos = camera.GetPos();
	material_->SetConstBufVS(0, { cPos.x, cPos.y, cPos.z, 0.0f });

	renderer_->Draw();

}
