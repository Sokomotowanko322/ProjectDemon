#include <EffekseerForDXLib.h>
#include <DxLib.h>
#include <cmath>
#include <utility>
#include <cassert>
#include <deque>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <array>
#include <vector>
#include "../Utility/Utility.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Resource.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Renderer/ModelMaterial.h"
#include "../Renderer/ModelRenderer.h"
#include "../Application.h"
#include "Common/Transform.h"
#include "NormalEnemy.h"

const char PATH_VERTEXSHADER[] = "Data/Shader/VertexShader.vso";
const char PATH_PIXELSHADER[] = "Data/Shader/PixelShader.pso";
const char PATH_POSTEFFECT[] = "Data/Shader/PostEffect.pso";
const char DATA_DISSOLVE[] = "Data/Image/Dissolve.png";

const VECTOR DEFAULT_POS = { 0.0f, -100.0f, 0.0f };
const VECTOR MODEL_SCALE = { 0.01f, 0.01f, 0.01f };

namespace MyEngine {

	int DrawRectGraph(int x, int y, int srcX, int srcY, int srcW, int srcH, int handle, int psH, int secondH = -1, bool trans = false) {
		int w, h;
		GetGraphSize(handle, &w, &h);
		std::array<VERTEX2DSHADER, 4> vertices;
		for (auto& v : vertices) {
			v.dif = GetColorU8(255, 255, 255, 255);
			v.spc = GetColorU8(0, 0, 0, 0);
			v.rhw = 1.0f;
		}

		float uvStartX = (float)srcX / (float)w;
		float uvStartY = (float)srcY / (float)h;
		float uvEndX = (float)(srcX + srcW) / (float)w;
		float uvEndY = (float)(srcY + srcH) / (float)h;

		//矩形になるように並べていきます
		//Zの字になるように並べます
		//左上
		vertices[0].pos = VGet(x, y, 0.0f);
		vertices[0].u = uvStartX;
		vertices[0].v = uvStartY;
		//右上
		vertices[1].pos = VGet(x + srcW, y, 0.0f);
		vertices[1].u = uvEndX;
		vertices[1].v = uvStartY;
		//左下
		vertices[2].pos = VGet(x, y + srcH, 0.0f);
		vertices[2].u = uvStartX;
		vertices[2].v = uvEndY;
		//右下
		vertices[3].pos = VGet(x + srcW, y + srcH, 0.0f);
		vertices[3].u = uvEndX;
		vertices[3].v = uvEndY;
		if (trans) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
		}
		//今からの描画に使う画像を設定します
		SetUseTextureToShader(0, handle);
		if (secondH != -1) {
			SetUseTextureToShader(1, secondH);
		}
		SetUsePixelShader(psH);
		return DrawPrimitive2DToShader(vertices.data(),//アドレス
			vertices.size(),//頂点数
			DX_PRIMTYPE_TRIANGLESTRIP);//トポロジー
	}

	int DrawGraph(int x, int y, int handle, int psH, int secondH = -1, int thirdH = -1, int forthH = -1, bool trans = false) {
		int w, h;
		GetGraphSize(handle, &w, &h);
		std::array<VERTEX2DSHADER, 4> vertices;
		for (auto& v : vertices) {
			v.dif = GetColorU8(255, 255, 255, 255);
			v.spc = GetColorU8(0, 0, 0, 0);
			v.rhw = 1.0f;
		}
		//矩形になるように並べる
		//Zの字になるように並べる
		//左上
		vertices[0].pos = VGet(x, y, 0.0f);
		vertices[0].u = 0.0f;
		vertices[0].v = 0.0f;
		//右上
		vertices[1].pos = VGet(x + w, y, 0.0f);
		vertices[1].u = 1.0f;
		vertices[1].v = 0.0f;
		//左下
		vertices[2].pos = VGet(x, y + h, 0.0f);
		vertices[2].u = 0.0f;
		vertices[2].v = 1.0f;
		//右下
		vertices[3].pos = VGet(x + w, y + h, 0.0f);
		vertices[3].u = 1.0f;
		vertices[3].v = 1.0f;
		if (trans) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
		}

		//今からの描画に使う画像を設定します
		SetUseTextureToShader(0, handle);

		if (secondH != -1) {
			SetUseTextureToShader(1, secondH);
		}
		if (thirdH != -1) {
			SetUseTextureToShader(2, thirdH);
		}
		if (forthH != -1) {
			SetUseTextureToShader(3, forthH);
		}
		SetUsePixelShader(psH);
		std::array<uint16_t, 6> indices;

		indices = { 0,1,3,0,3,2 };

		
		//return DrawPrimitiveIndexed2DToShader(vertices.data(),//アドレス
		//	vertices.size(),//頂点数
		//	indices.data(),
		//	indices.size(),
		//	DX_PRIMTYPE_TRIANGLELIST);



		return DrawPrimitive2DToShader(vertices.data(),//アドレス
			vertices.size(),//頂点数
			DX_PRIMTYPE_TRIANGLESTRIP);//トポロジー
	}
}

NormalEnemy::NormalEnemy(void)
{
	
}

NormalEnemy::~NormalEnemy(void)
{
}

void NormalEnemy::Init(void)
{
	// モデルの基本設定
	transform_.modelId = (ResourceManager::GetInstance().Load(ResourceManager::SRC::ENEMY_BOSS_TYPE1).handleId_);
	transform_.scl = MODEL_SCALE;
	transform_.quaRot = Quaternion();
	transform_.pos = DEFAULT_POS;
	
	deltaTime = SceneManager::GetInstance().GetDeltaTime();
	
	camera_ = std::make_unique<Camera>();
	renderer_ = std::make_unique<ModelRenderer>(transform_.modelId, *material_);

	buff_ = CreateShaderConstantBuffer(sizeof(Data));
	data_ = static_cast<Data*>(GetBufferShaderConstantBuffer(buff_));

	InitDissolve();
}

void NormalEnemy::InitAnim(void)
{

}

void NormalEnemy::InitDissolve(void)
{
	// レンダーターゲットそれぞれ専用のスクリーンを作成
	renderTarget_[SECOND_RENDERTARGET] = MakeScreen(1024, 640, true);
	renderTarget_[OUTLINE] = MakeScreen(1024, 640, true);
	renderTarget_[FIRST_RENDERTARGET] = MakeScreen(1024, 640, true);
	renderTarget_[NORMAL] = MakeScreen(1024, 640, true);
	renderTarget_[BLUR] = MakeScreen(1024, 640, true);
	renderTarget_[SHRINK] = MakeScreen(512, 320, true);

	// 深度値を記録するバッファの作成
	SetCreateGraphChannelBitDepth(32);
	SetCreateDrawValidGraphChannelNum(1);
	renderTarget_[DEPTH] = MakeScreen(1024, 640, true);

	// テクスチャ設定
	dissolveTex_ = LoadGraph(DATA_DISSOLVE);

	// シェーダ読み込み
	postEffect_ = LoadPixelShader(PATH_POSTEFFECT);
	pixelShader_ = LoadPixelShader(PATH_PIXELSHADER);
	vertexShader_ = LoadVertexShader(PATH_VERTEXSHADER);

	data_->dissolveHeight_ = 1.0f;

	// モデルのメッシュ取得
	auto meshNum = MV1GetMeshNum(transform_.modelId);
	VECTOR maxPos = Utility::VECTOR_ZERO;
	VECTOR minPos = { 0,-100,0 };
	for (int i = 0; i < meshNum; ++i)
	{
		MV1SetMeshBackCulling(transform_.modelId, i, DX_CULLING_NONE);
		VECTOR modelMaxPos = MV1GetMeshMaxPosition(transform_.modelId, i);
		VECTOR modelMinPos = MV1GetMeshMinPosition(transform_.modelId, i);

		minPos.x = (std::min)(minPos.x, modelMinPos.x);
		minPos.y = (std::min)(minPos.y, modelMinPos.y);
		minPos.z = (std::min)(minPos.z, modelMinPos.z);

		maxPos.x = (std::max)(maxPos.x, modelMaxPos.x);
		maxPos.y = (std::max)(maxPos.y, modelMaxPos.y);
		maxPos.z = (std::max)(maxPos.z, modelMaxPos.z);
	}
	data_->disMinY_ = minPos.y;
	data_->disMaxY_ = maxPos.y;
}

void NormalEnemy::Update(void)
{
	MakeDissolve();

	if (CheckHitKey(KEY_INPUT_Q))
	{
		data_->dissolveHeight_ += 0.01f;
	}
	if (CheckHitKey(KEY_INPUT_E))
	{
		data_->dissolveHeight_ -= 0.01f;
	}
}

void NormalEnemy::Draw(void)
{
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();
	MV1DrawModel(transform_.modelId);
}

void NormalEnemy::SetPos(VECTOR pos)
{
	transform_.pos = pos;
	transform_.Update();
}

void NormalEnemy::MakeDissolve(void)
{
	SetDrawScreen(renderTarget_[FIRST_RENDERTARGET]);
	ClearDrawScreen();
	FillGraph(renderTarget_[DEPTH], 0, 0, 0, 0);

	MV1SetUseOrigShader(true);
	SetUseVertexShader(vertexShader_);
	SetUsePixelShader(pixelShader_);

	UpdateShaderConstantBuffer(buff_);
	SetShaderConstantBuffer(buff_, DX_SHADERTYPE_PIXEL, 4);

	SetUseTextureToShader(3, dissolveTex_);
	SetUseTextureToShader(1, renderTarget_[DEPTH]);
	SetUseTextureToShader(2, renderTarget_[NORMAL]);

	SetRenderTargetToShader(0, renderTarget_[FIRST_RENDERTARGET]);

	FillGraph(renderTarget_[DEPTH], 0, 0, 0, 0);
	FillGraph(renderTarget_[NORMAL], 0, 0, 0, 0);

	SetRenderTargetToShader(1, renderTarget_[DEPTH]);//深度情報の出力
	SetRenderTargetToShader(2, renderTarget_[NORMAL]);//法線情報の出力

	// 1番,2番を解除
	SetRenderTargetToShader(1, -1);
	SetRenderTargetToShader(2, -1);
	
	SetDrawScreen(renderTarget_[SECOND_RENDERTARGET]);
	ClearDrawScreen();
	//GraphFilterBlt(renderTarget_[FIRST_RENDERTARGET], renderTarget_[BLUR], DX_GRAPH_FILTER_GAUSS, 8, 1400);
	//GraphFilterBlt(renderTarget_[BLUR], renderTarget_[SHRINK], DX_GRAPH_FILTER_DOWN_SCALE, 2);
	//GraphFilter(renderTarget_[SHRINK], DX_GRAPH_FILTER_GAUSS, 8, 1400);
	SetRenderTargetToShader(1, renderTarget_[OUTLINE]);
	//DrawGraph(0, 0, postEffect_, true);
	MyEngine::DrawGraph(0, 0, renderTarget_[FIRST_RENDERTARGET], postEffect_, renderTarget_[BLUR], renderTarget_[DEPTH], renderTarget_[NORMAL], true);
	ClearDrawScreen();
	
	SetRenderTargetToShader(1, -1);

	SetUseTextureToShader(0, -1);//0番を解除
	SetUseTextureToShader(1, -1);//1番を解除
	SetUseTextureToShader(2, -1);//2番を解除
	SetUseTextureToShader(3, -1);//3番を解除

	SetDrawScreen(renderTarget_[FIRST_RENDERTARGET]);
	ClearDrawScreen();
	/*GraphFilterBlt(renderTarget_[OUTLINE], renderTarget_[BLUR], DX_GRAPH_FILTER_GAUSS, 8, 1400);
	GraphFilterBlt(renderTarget_[BLUR], renderTarget_[SHRINK], DX_GRAPH_FILTER_DOWN_SCALE, 2);
	GraphFilter(renderTarget_[SHRINK], DX_GRAPH_FILTER_GAUSS, 8, 1400);*/
	DrawGraph(0, 0, renderTarget_[SECOND_RENDERTARGET], true); 
	//DrawGraph(0, 0, renderTarget_[FIRST_RENDERTARGET], true);

	/*SetDrawBlendMode(DX_BLENDMODE_MULA, 255);
	DrawGraph(0, 0, renderTarget_[OUTLINE], true);
	DrawGraph(0, 0, renderTarget_[BLUR], true);
	DrawExtendGraph(0, 0, 1024, 640, renderTarget_[SHRINK], true);*/

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}