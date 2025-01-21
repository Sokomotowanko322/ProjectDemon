#include <math.h>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/TimeManager.h"
#include "../Manager/InputManager.h"
#include "../Utility/Utility.h"
//#include "../Utility/MyDebugUtility.h"
//#include "../Manager/GravityManager.h"
#include "../Object/Common/Transform.h"
#include "../Object/Common/Collider/ColliderController.h"
#include "Camera.h"

using namespace std;
using InputMng = InputManager;

const float INIT_STEP_CAMERASHAKE = Utility::FLOAT_ZERO;
const float INIT_POW_CAMERASHAKE = Utility::FLOAT_ZERO;
const float INIT_TIMESCALE_CAMERASHAKE = Utility::FLOAT_ONE;

const float ROT_POW_CAMERA = 1.0f;		//カメラスピード(度)
const float CAMERACLIP_NEAR = 10.0f;		//カメラクリップ：NEAR
const float CAMERACLIP_FAR = 30000.0f;		//カメラクリップ：NEAR
const VECTOR INIT_CAMERA_POS = { 0.0f, 100.0f, -200.0f };		//カメラの初期座標

const float SPEED_FREECAMERA = 2.0f;
const float ROT_POW_FREECAMERA = 2.0f;

const VECTOR LOCAL_POS_FOLLOWER_CAMERA = { 0.0f, 60.0f, -600.0f };		//追従位置からカメラ位置までの相対座標
const VECTOR LOCAL_POS_FOLLOWER_TARGET = { 0.0f, 0.0f, 500.0f };		//追従位置から注視点までの相対座標

const VECTOR LOCAL_POS_TARGET_CAMERA_MOUSEFOLLOW = { 0.0f, 100.0f, 500.0f };		//マウス追従時の注視点からカメラまでの相対座標
const VECTOR LOCAL_POS_PLAYER_CAMERA_MOUSEFOLLOW = { 0.0f, 100.0f,200.0f };		//マウス追従時の自機からカメラまでの相対座標
const Vector2 INIT_POS_MOUSE = { Application::SCREEN_SIZE_HALF_X ,Application::SCREEN_SIZE_HALF_Y };		//マウス追従時のマウスカーソル初期座標
const float MIN_CAMERA_DEG_Y_MOUSE = 0.0f;		//マウス追従時のカメラの下限
const float MAX_CAMERA_DEG_Y_MOUSE = 70.0f;		//マウス追従時のカメラの上限

const VECTOR LOCAL_C_POS_SUCCESSFUL = { 0.0f,1600.0f,1200.0f };		//成功時の追従対象からカメラ座標相対座標
const VECTOR LOCAL_POS_CAMERA_SUCCESSFUL = { 0.0f,300.0f,100.0f };		//成功時の追従対象から注視点までの相対座標
const VECTOR LOCAL_C_POS_SUCCESSFUL_FIX = { 800.0f,1600.0f,-1200.0f };		//成功時の定点カメラの相対座標

const float LIMIT_X_UP_RAD = 40.0f * (DX_PI_F / 180.0f);		//カメラのX回転角上限
const float LIMIT_X_DW_RAD = -1.0f * (DX_PI_F / 180.0f);		//カメラのX回転角下限

const Vector2f DEF_CAMERA_ROT_POW = { 10.0f,10.0f };		//デフォルトのカメラの回転速度

const float INIT_TIME_SUCCESSCAMERA = Utility::FLOAT_ZERO;		//成功時のカメラ演出初期化
const float MAX_TIME_SUCCESSCAMERA = 18.0f;		//成功時のカメラ演出時間
const float TIME_SUCCESSCAMERA_CHANGEMOVE_Y = 12.0f;		//Y軸回転に切り替わる時間
const float TIME_SUCCESSCAMERA_CHANGEMOVE_X = 6.0f;		//X軸回転に切り替わる時間
const float INIT_SUCCESSCAMERA_DEG_XY = 0.0f;		//成功時カメラの初期化用
const float DECELERATE_RATE_SUCCESSCAMERA = 0.05f;		//成功時カメラの減速率

const float INIT_CAMERA_DEF_X = 30.0f;
const Vector2f DEF_MOUSECAMERA_DEG = { 0.0f,20.0f };

const float INIT_COLLISION_LINE_STAGE = Utility::FLOAT_ZERO;

const float LOCAL_POS_Y_CAMERACOLLISION = 200.0f;
const float LEN_CAMERACOLLISION_LINE = 500.0f;
const float MIN_LEN_CAMERACOLLISION_LINE = 1.0f;
const int NUM_NOTHIT_POLY = 0;


const float CAMERA_SENSITIVITY = 10000.0f;

Camera::Camera(void)
	:
	followTransform_(nullptr),
	mode_(MODE::FIXED_POINT),
	pos_{},
	prePos_{},
	angles_{},
	rotOutX_(Quaternion()),
	rot_(Quaternion()),
	targetPos_{},
	cameraUp_{},
	preMousePos_{},
	nowMousePos_{},
	radXY_{},
	cameraRotPow_(DEF_CAMERA_ROT_POW),
	defSetRot_(Quaternion()),
	isShake_(false),
	shakeStep_(INIT_STEP_CAMERASHAKE),
	shakePow_(INIT_POW_CAMERASHAKE),
	timeScale_(INIT_TIMESCALE_CAMERASHAKE),
	successCameraTimeStep_(INIT_TIME_SUCCESSCAMERA),
	successCameraEnd_{},
	stagemodelId_(Utility::HANDLEID_NULL),
	localPosMouseFollow_(LOCAL_POS_TARGET_CAMERA_MOUSEFOLLOW),
	collitionLineStageCamera_(INIT_COLLISION_LINE_STAGE),
	modeChanges_{},
	modeUpdate_{},
	setBeforeDrawMode_{}
{

	modeChanges_.emplace(MODE::FIXED_POINT, bind(&Camera::ChangeFixedPoint, this));
	modeChanges_.emplace(MODE::FREE, bind(&Camera::ChangeFree, this));
	modeChanges_.emplace(MODE::FOLLOW, bind(&Camera::ChangeFollow, this));
	modeChanges_.emplace(MODE::FOLLOW_FIX, bind(&Camera::ChangeFollowFix, this));
	modeChanges_.emplace(MODE::FOLLOW_MOUSE, bind(&Camera::ChangeFollowMouse, this));
	modeChanges_.emplace(MODE::SUCCESSFUL, bind(&Camera::ChangeSuccessful, this));

}

Camera::~Camera(void)
{

	modeChanges_.clear();

}

void Camera::Init(void)
{

	//colliderController_ = make_unique<ColliderController>();
	//colliderController_->SetCollision(OBJECT_TYPE::STAGE);
	localPosMouseFollow_ = LOCAL_POS_TARGET_CAMERA_MOUSEFOLLOW;

	ChangeMode(MODE::FIXED_POINT);

}

void Camera::Update(void)
{

	modeUpdate_();

	CameraShake();

}

void Camera::SetBeforeDraw(void)
{

	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraNearFar(CAMERACLIP_NEAR, CAMERACLIP_FAR);


	setBeforeDrawMode_();

	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();

}

void Camera::SetFollow(const Transform* follow)
{

	followTransform_ = follow;

}

void Camera::SetCameraRotPow(const Vector2f& pow)
{

	cameraRotPow_ = pow;

}

const VECTOR Camera::GetMousePos(void)
{

	VECTOR ret = {};
	Vector2f retPos = ToVector2f(nowMousePos_);
	ret = { retPos.x,retPos.y,Utility::FLOAT_ZERO };

	return ret;

}

void Camera::SetSuccessCameraMoveEndFunc(std::function<void(void)> func)
{
	successCameraEnd_ = func;
}

void Camera::SetCameraShake(float time, float shakePow, float timeScale)
{

	if (isShake_) { return; }

	isShake_ = true;
	shakeStep_ = time;
	shakePow_ = shakePow;
	timeScale_ = timeScale;

}

void Camera::CameraShake(void)
{

	const int mod = 3;
	const int subVal = 1;

	if (!isShake_) { return; }

	Vector2f shakePos = { pos_.x,pos_.y };

	Vector2 pow = Vector2(
		(int)(shakeStep_ * timeScale_) % mod - subVal,
		(int)(shakeStep_ * timeScale_) % mod - subVal
	);

	Vector2f shakeMove = ToVector2f(pow) * shakePow_;
	shakePos += shakeMove;

	pos_.x = shakePos.x;
	pos_.y = shakePos.y;

	shakeStep_ -= TimeManager::GetInstance().GetDeltaTime();
	if (shakeStep_ < INIT_STEP_CAMERASHAKE)
	{
		isShake_ = false;
	}

}

void Camera::SetDefQuaRot(const Quaternion& qua)
{

	defSetRot_ = qua;

	auto& insI = InputMng::GetInstance();
	if (insI.IsControlType_Key())
	{
		radXY_ = Vector2f();
		radXY_.y = Utility::Deg2RadF(DEF_MOUSECAMERA_DEG.y);
	}
	if (insI.IsControlType_Controller())
	{
		angles_ = qua.ToEuler();
	}

}

void Camera::SetAngle(const VECTOR& angle)
{

	angles_ = angle;

}

const VECTOR Camera::GetForward(void) const
{

	return VNorm(VSub(targetPos_, pos_));

}

void Camera::ChangeMode(const MODE mode)
{

	// カメラの初期設定
	SetDefault();

	mode_ = mode;
	modeChanges_[mode_]();

}

void Camera::SetDefault(void)
{

	// カメラの初期設定
	pos_ = INIT_CAMERA_POS;

	// 注視点
	targetPos_ = Utility::VECTOR_ZERO;

	// カメラの上方向
	cameraUp_ = Utility::DIR_U;

	angles_.x = Utility::Deg2RadF(INIT_CAMERA_DEF_X);
	angles_.y = Utility::FLOAT_ZERO;
	angles_.z = Utility::FLOAT_ZERO;

	rot_ = Quaternion();

}

void Camera::SyncFollow(void)
{

	// 同期先の位置
	VECTOR pos = followTransform_->pos;

	// 正面から設定されたY軸分、回転させる
	rotOutX_ = Quaternion::AngleAxis(angles_.y, Utility::AXIS_Y);

	// 正面から設定されたX軸分、回転させる
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, Utility::AXIS_X));

	VECTOR localPos;

	// 注視点(通常重力でいうところのY値を追従対象と同じにする)
	localPos = rotOutX_.PosAxis(LOCAL_POS_FOLLOWER_TARGET);
	targetPos_ = VAdd(pos, localPos);

	// カメラ位置
	localPos = rot_.PosAxis(LOCAL_POS_FOLLOWER_CAMERA);
	pos_ = VAdd(pos, localPos);

	// カメラの上方向
	cameraUp_ = Utility::DIR_U;

}

void Camera::SyncFollowFix(void)
{

	//auto& gIns = GravityManager::GetInstance();

	// 同期先の位置
	VECTOR pos = followTransform_->pos;

	//角度変化
	Quaternion gRot = Quaternion::Euler(Utility::VECTOR_ZERO);

	// 正面から設定されたY軸分、回転させる
	rotOutX_ = gRot.Mult(Quaternion::AngleAxis(angles_.y, Utility::AXIS_Y));

	// 正面から設定されたX軸分、回転させる
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, Utility::AXIS_X));

	VECTOR localPos;

	// 注視点(通常重力でいうところのY値を追従対象と同じにする)
	localPos = rotOutX_.PosAxis(LOCAL_POS_FOLLOWER_TARGET);
	targetPos_ = VAdd(pos, localPos);

	// カメラ位置
	if (mode_ == MODE::SUCCESSFUL)
	{
		localPos = rot_.PosAxis(LOCAL_C_POS_SUCCESSFUL_FIX);
	}
	else
	{
		localPos = rot_.PosAxis(LOCAL_POS_FOLLOWER_CAMERA);
	}

	pos_ = VAdd(pos, localPos);

	// カメラの上方向
	cameraUp_ = gRot.GetUp();

}

void Camera::SyncMouse(void)
{
	auto dT = SceneManager::GetInstance().GetDeltaTime();

	//角度変化
	Quaternion gRot = defSetRot_;

	// 同期先の位置
	VECTOR pos = followTransform_->pos;

	Vector2f vec;
	Vector2f vecN;
	//float len;

	preMousePos_ = nowMousePos_;
	GetMousePoint(&nowMousePos_.x, &nowMousePos_.y);
	radXY_.x += Utility::Deg2RadF(
		float(nowMousePos_.x - INIT_POS_MOUSE.x) * cameraRotPow_.x * dT
	);

	if (Utility::Rad2DegF(radXY_.y) <= MAX_CAMERA_DEG_Y_MOUSE && Utility::Rad2DegF(radXY_.y) >= MIN_CAMERA_DEG_Y_MOUSE)
	{
		radXY_.y -= Utility::Deg2RadF(
			float(nowMousePos_.y - INIT_POS_MOUSE.y) * cameraRotPow_.y * dT
		);

		if (Utility::Rad2DegF(radXY_.y) < MIN_CAMERA_DEG_Y_MOUSE)
		{
			radXY_.y = Utility::Deg2RadF(MIN_CAMERA_DEG_Y_MOUSE);
		}
		if (Utility::Rad2DegF(radXY_.y) > MAX_CAMERA_DEG_Y_MOUSE)
		{
			radXY_.y = Utility::Deg2RadF(MAX_CAMERA_DEG_Y_MOUSE);
		}
	}

	// 正面から設定されたY軸分、回転させる
	rotOutX_ = gRot.Mult(Quaternion::AngleAxis(radXY_.x, Utility::AXIS_Y));

	// 正面から設定されたX軸分、回転させる
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(radXY_.y, Utility::AXIS_X));

	angles_ = Quaternion::ToEuler(rot_);

	VECTOR localPos;
	localPos = rotOutX_.PosAxis(LOCAL_POS_PLAYER_CAMERA_MOUSEFOLLOW);
	targetPos_ = VAdd(pos, localPos);

	// カメラ位置
	//localPos = rot_.PosAxis(LOCAL_POS_TARGET_CAMERA_MOUSEFOLLOW);
	localPos = rot_.PosAxis(localPosMouseFollow_);
	prePos_ = pos_;
	pos_ = VAdd(pos, localPos);

	// カメラの上方向
	cameraUp_ = gRot.GetUp();

	SetMousePoint(INIT_POS_MOUSE.x, INIT_POS_MOUSE.y);

	ProcessRot();

}

void Camera::SyncSuccessCamera(void)
{
  
	// 同期先の位置
	VECTOR pos = followTransform_->pos;

	// 正面から設定されたY軸分、回転させる
	rotOutX_ = Quaternion::AngleAxis(angles_.y, Utility::AXIS_Y);

	// 正面から設定されたX軸分、回転させる

	rot_ = followTransform_->quaRot;
	rot_ = rot_.Mult(rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, Utility::AXIS_X)));



	VECTOR localPos;

	// 注視点(通常重力でいうところのY値を追従対象と同じにする)
	localPos = rotOutX_.PosAxis(LOCAL_POS_CAMERA_SUCCESSFUL);
	targetPos_ = VAdd(pos, localPos);

	localPos = rot_.PosAxis(LOCAL_C_POS_SUCCESSFUL);
	pos_ = VAdd(pos, localPos);

	// カメラの上方向
	cameraUp_ = Utility::DIR_U;

}

void Camera::ProcessRot(void)
{
	auto& ins = InputMng::GetInstance();

	auto padRstick = ins.GetJPadState(JOYPAD_NO::PAD1).AkeyR;

	float rotPow = ROT_POW_CAMERA;

	//右回転
	if (ins.IsPressed(InputMng::ACTION::CAM_RIGHT))
	{
		angles_.y += Utility::Deg2RadF(rotPow);
	}
	if (ins.IsPressed(InputMng::ACTION::CAM_LEFT))
	{
		angles_.y += Utility::Deg2RadF(-rotPow);
	}


	if (angles_.x <= LIMIT_X_UP_RAD)
	{
		if (ins.IsPressed(InputMng::ACTION::CAM_UP))
		{
			angles_.x += Utility::Deg2RadF(rotPow);
		}
	}

	if (angles_.x >= -LIMIT_X_DW_RAD)
	{
		if (ins.IsPressed(InputMng::ACTION::CAM_DOWN))
		{
			angles_.x += Utility::Deg2RadF(-rotPow);
		}
	}

	if (padRstick.x != InputMng::STICK_NEUTRAL_XBOX_ONE || padRstick.y != InputMng::STICK_NEUTRAL_XBOX_ONE)
	{
		if (angles_.x >= -LIMIT_X_DW_RAD && padRstick.y < InputMng::STICK_NEUTRAL_XBOX_ONE)
		{
			angles_.x += padRstick.y / CAMERA_SENSITIVITY;
		}

		if (angles_.x <= LIMIT_X_UP_RAD && padRstick.y > InputMng::STICK_NEUTRAL_XBOX_ONE)
		{
			angles_.x += padRstick.y / CAMERA_SENSITIVITY;
		}

		angles_.y += padRstick.x / CAMERA_SENSITIVITY;
	}

}

void Camera::ProcessRotMouse(void)
{

}

void Camera::SuccessCameraMove(void)
{

	float rotPow = ROT_POW_CAMERA * DECELERATE_RATE_SUCCESSCAMERA;

	if (successCameraTimeStep_ < TIME_SUCCESSCAMERA_CHANGEMOVE_X)
	{
		//処理なし
	}
	else if (
		successCameraTimeStep_ >= TIME_SUCCESSCAMERA_CHANGEMOVE_X
		&& successCameraTimeStep_ < TIME_SUCCESSCAMERA_CHANGEMOVE_Y
		)
	{
		angles_.x += Utility::Deg2RadF(rotPow);
	}
	else if (successCameraTimeStep_ >= TIME_SUCCESSCAMERA_CHANGEMOVE_Y)
	{
		angles_.x = Utility::FLOAT_ZERO;
		angles_.y += Utility::Deg2RadF(-rotPow);
	}

}

void Camera::ChangeFixedPoint(void)
{

	setBeforeDrawMode_ = bind(&Camera::SetBeforeDrawFixedPoint, this);
	modeUpdate_ = bind(&Camera::UpdateFixedPoint, this);

}

void Camera::ChangeFree(void)
{

	setBeforeDrawMode_ = bind(&Camera::SetBeforeDrawFree, this);
	modeUpdate_ = bind(&Camera::UpdateFree, this);

}

void Camera::ChangeFollow(void)
{

	setBeforeDrawMode_ = bind(&Camera::SetBeforeDrawFollow, this);
	modeUpdate_ = bind(&Camera::UpdateFollow, this);

}

void Camera::ChangeFollowFix(void)
{

	modeUpdate_ = bind(&Camera::UpdateFollowFix, this);
	setBeforeDrawMode_ = bind(&Camera::SetBeforeDrawFollowFix, this);

}

void Camera::ChangeFollowMouse(void)
{

	modeUpdate_ = bind(&Camera::UpdateFollowMouse, this);
	setBeforeDrawMode_ = bind(&Camera::SetBeforeDrawFollowMouse, this);
	radXY_.y = Utility::Deg2RadF(DEF_MOUSECAMERA_DEG.y);

	SetMousePoint(INIT_POS_MOUSE.x, INIT_POS_MOUSE.y);

}

void Camera::ChangeSuccessful(void)
{

	successCameraTimeStep_ = INIT_TIME_SUCCESSCAMERA;
	angles_.x = INIT_SUCCESSCAMERA_DEG_XY;
	angles_.y = INIT_SUCCESSCAMERA_DEG_XY;

	modeUpdate_ = bind(&Camera::UpdateSuccessful, this);
	setBeforeDrawMode_ = bind(&Camera::SetBeforeDrawSuccessful, this);

}

void Camera::UpdateFixedPoint(void)
{

}

void Camera::UpdateFree(void)
{

	auto& insI = InputMng::GetInstance();

	VECTOR dir = Utility::VECTOR_ZERO;

	//前後左右上下
	if (insI.IsPressed(InputMng::ACTION::MOVE_FORWARD))
	{
		dir = Utility::DIR_F;
	}
	if (insI.IsPressed(InputMng::ACTION::MOVE_LEFT))
	{
		dir = Utility::DIR_L;
	}
	if (insI.IsPressed(InputMng::ACTION::MOVE_BACK))
	{
		dir = Utility::DIR_B;
	}
	if (insI.IsPressed(InputMng::ACTION::MOVE_RIGHT))
	{
		dir = Utility::DIR_R;
	}

	if (insI.IsPressed(InputMng::ACTION::DEBUGCON))
	{
		dir = Utility::DIR_D;
	}

	if (insI.IsPressed(InputMng::ACTION::RUN))
	{
		dir = Utility::DIR_U;
	}


	//角度
	float rotPow = Utility::Deg2RadF(ROT_POW_FREECAMERA);
	if (insI.IsPressed(InputMng::ACTION::CAM_UP))
	{
		angles_.x -= rotPow;
	}
	if (insI.IsPressed(InputMng::ACTION::CAM_DOWN))
	{
		angles_.x += rotPow;
	}
	if (insI.IsPressed(InputMng::ACTION::CAM_LEFT))
	{
		angles_.y -= rotPow;
	}
	if (insI.IsPressed(InputMng::ACTION::CAM_RIGHT))
	{
		angles_.y += rotPow;
	}

	Quaternion qua = Quaternion::Euler(angles_);

	VECTOR moveDir = qua.PosAxis(dir);

	pos_ = VAdd(pos_, VScale(moveDir, SPEED_FREECAMERA));

}

void Camera::UpdateFollow(void)
{

	// カメラ操作
	ProcessRot();

	// 追従対象との相対位置を同期
	SyncFollow();

	CameraCollision();

}

void Camera::UpdateFollowFix(void)
{

	// カメラ操作
	ProcessRot();

	// 追従対象との相対位置を同期
	SyncFollowFix();

}

void Camera::UpdateFollowMouse(void)
{

	VECTOR mousePos = {};

	//カメラ操作
	ProcessRotMouse();

	// 追従対象との相対位置を同期
	SyncMouse();

	// カメラ操作
	ProcessRot();

	// 追従対象との相対位置を同期
	SyncFollowFix();

	CameraCollision();

}

void Camera::UpdateSuccessful(void)
{

	// カメラ操作
	SuccessCameraMove();

	if (successCameraTimeStep_ >= TIME_SUCCESSCAMERA_CHANGEMOVE_X)
	{
		// 追従対象との相対位置を同期
		SyncSuccessCamera();
	}
	else
	{
		SyncFollowFix();
	}

	if (successCameraTimeStep_ > MAX_TIME_SUCCESSCAMERA)
	{
		successCameraEnd_();
	}

	successCameraTimeStep_ += TimeManager::GetInstance().GetDeltaTime();

}

void Camera::SetBeforeDrawFixedPoint(void)
{

	// 何もしない
		// カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUp_
	);

}

void Camera::SetBeforeDrawFree(void)
{

	SetCameraPositionAndAngle(pos_, angles_.x, angles_.y, angles_.z);

}

void Camera::SetBeforeDrawFollow(void)
{

	// カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUp_
	);

}

void Camera::SetBeforeDrawFollowFix(void)
{

	// カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUp_
	);

}

void Camera::SetBeforeDrawFollowMouse(void)
{

	// カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUp_
	);

	SetMouseDispFlag(false);

}

void Camera::SetBeforeDrawSuccessful(void)
{
	// カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUp_
	);
}

void Camera::DrawDebug(void)
{

}

void Camera::CameraCollision(void)
{

	VECTOR Ppos = followTransform_->pos;
	Ppos.y += LOCAL_POS_Y_CAMERACOLLISION;

	//float Radius = 1.0f;

	auto hit = MV1CollCheck_Line(
		stagemodelId_, -1,
		pos_, Ppos);

	if (hit.HitFlag)
	{
		// コリジョン用ポリゴンが存在しない座標までカメラ座標を注視点に近づける

		// 注視点座標からカメラ座標へのベクトルを算出
		VECTOR Direction = VNorm(VSub(pos_, Ppos));

		// ポリゴンに当たらない距離をセット
		float NotHitDistance = Utility::FLOAT_ZERO;

		// ポリゴンに当たる距離をセット

		VECTOR TestPosition = pos_;

		float HitDistance = LEN_CAMERACOLLISION_LINE;
		while (HitDistance - NotHitDistance > MIN_LEN_CAMERACOLLISION_LINE)
		{
			// 当たるかどうかテストする距離を算出( 当たらない距離と当たる距離の中間 )
			collitionLineStageCamera_ = NotHitDistance + (HitDistance - NotHitDistance) / 2.0f;

			// テスト用のカメラ座標を算出
			TestPosition =
				VAdd(Ppos, VScale(Direction, collitionLineStageCamera_));

			hit = MV1CollCheck_Line(
				stagemodelId_, -1,
				TestPosition, Ppos);

			// 新しい座標でコリジョン用ポリゴンに当たるかテスト
			if (hit.HitFlag)
			{
				// 当たる距離を collitionLineStageCamera_ に変更する
				HitDistance = collitionLineStageCamera_;
			}
			else
			{
				// 当たらなかったら当たらない距離を collitionLineStageCamera_ に変更する
				NotHitDistance = collitionLineStageCamera_;
			}

			// HitDistance と NoHitDistance が十分に近づいていなかったらループ
		}

		// カメラの座標をセット
		pos_ = TestPosition;
	}

}

bool Camera::HitCheck(VECTOR campos, VECTOR Ppos, float size)
{
	// ステージ用コリジョン３Ｄモデルに当たるかをチェック
	auto HitDim = MV1CollCheck_Capsule(stagemodelId_, -1, campos, Ppos, size);

	auto HitNum = HitDim.HitNum;
	MV1CollResultPolyDimTerminate(HitDim);

	// 当たっていたらここで終了
	if (HitNum > NUM_NOTHIT_POLY)
	{
		return true;
	}

	return false;

}

void Camera::SetHitModel(const int modelId)
{
	stagemodelId_ = modelId;
}

