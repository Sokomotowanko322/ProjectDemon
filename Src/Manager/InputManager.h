#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "../Common/Vector2.h"
#include "../Common/Vector2f.h"
#include "../Singleton.h"

//コントローラーの番号
enum class JOYPAD_NO
{
	KEY_PAD1,
	PAD1,
	PAD2,
	PAD3,
	PAD4,
	INPUT_KEY = 4096,
};


//コントローラーのボタン
enum class JOYPAD_BTN
{
	LEFT = 0,
	RIGHT,
	TOP,
	DOWN,
	R_TRIGGER,
	ZR_TRIGGER,
	R3_TRIGGER,
	L_TRIGGER,
	ZL_TRIGGER,
	L3_TRIGGER,
	START,
	CENTER,
	TEN_UP,
	TEN_DOWN,
	TEN_LEFT,
	TEN_RIGHT,
	MAX,
};

//コントローラーのアナログスティック
enum class JOYPAD_STICK
{
	L_STICK_UP = 0,
	L_STICK_DOWN,
	L_STICK_LEFT,
	L_STICK_RIGHT,

	R_STICK_UP,
	R_STICK_DOWN,
	R_STICK_LEFT,
	R_STICK_RIGHT,

	MAX,
};

//コントローラーの種別
enum class JOYPAD_TYPE
{
	OTHER = 0,
	XBOX_360,
	XBOX_ONE,			//XboxOne
	DUAL_SHOCK_3,
	DUAL_SHOCK_4,
	DUAL_SENCE,
	SWICH_JOY_CON_L,
	SWICH_JOY_CON_R,
	SWICH_JOY_PRO_CTRL,
	SWICH_HORI_PAD,
	MAX,
};

//コントローラーの入力情報
struct JOYPAD_IN_STATE
{
	unsigned char ButtonsOld[static_cast<int>(JOYPAD_BTN::MAX)];		//1f前のボタン入力
	unsigned char ButtonsNew[static_cast<int>(JOYPAD_BTN::MAX)];		//現フレームのボタン入力

	bool aKeyOld[static_cast<int>(JOYPAD_STICK::MAX)];		//1f前のスティック入力
	bool aKeyNew[static_cast<int>(JOYPAD_STICK::MAX)];		//現フレームのスティック入力

	Vector2 AkeyL = {};
	Vector2 AkeyLOld = {};
	Vector2 AkeyR = {};
	Vector2 AkeyROld = {};

	JOYPAD_IN_STATE()
		:
		ButtonsOld{},
		ButtonsNew{},
		aKeyOld{},
		aKeyNew{}
	{};
};

/// <summary>
/// 入力制御を行うクラス
/// </summary>
class InputManager
	: public Singleton<InputManager>
{

public:

	static constexpr int STICK_NEUTRAL_XBOX_ONE = 0;		//XBOXスティック : ニュートラル
	static constexpr int STICK_X_LEFT_XBOX_ONE = -1000;		//XBOXスティック : 左
	static constexpr int STICK_X_RIGHT_XBOX_ONE = 1000;		//XBOXスティック : 右
	static constexpr int STICK_Y_UP_XBOX_ONE = -1000;		//XBOXスティック : 上
	static constexpr int STICK_Y_DOWN_XBOX_ONE = 1000;		//XBOXスティック : 下

	/// <summary>
	/// 入力タイプ
	/// </summary>
	enum class InputType
	{
		KEYBOARD,		//キーボード
		GAMEPAD,		//ゲームパッド
		MOUSE,		//マウス
		GAMEPAD_STICK,		//スティック
	};


	/// <summary>
	/// 入力情報
	/// </summary>
	struct InputMapInfo
	{
		InputType type;		//入力種別
		int buttonID_;		//キー識別
		std::string KBname = "null";		//text表示
	};

	/// <summary>
	/// アクションテーブル
	/// </summary>
	enum class ACTION
	{
		ENTER,		//決定
		PAUSE,		//ポーズ画面
		SCENE_CHANGE,		//画面遷移用

		//選択画面用
		SELECT_UP,		//カーソル上
		SELECT_DOWN,		//カーソル下
		SELECT_LEFT,		//カーソル左
		SELECT_RIGHT,		//カーソル右
		SELECT_ENTER,		//決定
		SELECT_CANCEL,		//キャンセル


		//プレイヤー操作
		ATTACK,		//攻撃
		BLOCK,		//ガード
		DODGE,		//回避
		RUN,		//走る
		PULL,		//
		DRAW_UNDRAW,		//
		CAST,		//
		MOVE_PAD,		//
		MOVE_FORWARD,		//前方移動
		MOVE_BACK,		//後方移動
		MOVE_LEFT,		//左移動
		MOVE_RIGHT,		//右移動

		DEBUG,		//
		DEBUGCON,		//

		//アイテム
		ITEMCALL,		//アイテム欄呼出
		DEBUG_EFFECT,		//
		NEXTITEM,		//アイテム欄前切替
		PREITEM,		//アイテム欄後切替
		USEITEM,		//使用
		SELECT,		//選択
		INHALE,		//選択キャンセル

		//カメラ
		CAM_UP,		//上回転
		CAM_DOWN,		//下回転
		CAM_LEFT,		//左回転
		CAM_RIGHT,		//右回転

		MAX,		//最大数カウント用
	};

	/// <summary>
	/// コントローラータイプ
	/// </summary>
	enum class CONTROL_TYPE
	{
		KEY,
		CONTROLLER,
	};

	/// <summary>
	/// Dxlib_End()前のリソース解放
	/// </summary>
	void Destroy(void) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void);

	/// <summary>
	/// 入力チェック
	/// </summary>
	/// <param name="action">アクションの名前</param>
	/// <returns>
	/// true : 押されている
	/// false : 押されてない
	/// </returns>
	bool IsPressed(const ACTION& action) const;

	/// <summary>
	/// 入力チェック
	/// </summary>
	/// <param name="action">調べたいアクション</param>
	/// <returns>
	/// true : 今押された 
	/// false : 押されてないか押しっぱなし
	/// </returns>
	bool IsTriggered(const ACTION& action) const;

	/// <summary>
	/// 入力チェック
	/// </summary>
	/// <param name="action">調べたいアクション</param>
	/// <returns>
	/// true : 今離された
	/// false : 押されている
	/// </returns>
	bool IsReleased(const ACTION& action) const;

	/// <summary>
	/// 対象のコントローラーのタイプを取得
	/// </summary>
	/// <param name="no">コントローラー番号</param>
	/// <returns>JOYPAD_TYPE</returns>
	const JOYPAD_TYPE GetJPadType(const JOYPAD_NO no);

	/// <summary>
	/// //現在のパッドの入力情報を取得
	/// </summary>
	/// <param name="no">ジョイパッドの番号</param>
	/// <returns>JOYPAD_IN_STATE</returns>
	const JOYPAD_IN_STATE GetJPadState(const JOYPAD_NO no);

	/// <summary>
	/// マウス座標の取得
	/// </summary>
	/// <param name="pos">マウス座標を書き込むVector2</param>
	void GetMousePos(Vector2& pos);

	/// <summary>
	/// UI表示
	/// </summary>
	/// <param name="action">アクションの名前</param>
	/// <returns>ACTIONの文字列を返す</returns>
	const std::string& GetName(const ACTION& action) const;

	/// <summary>
	/// 現在の入力タイプを取得
	/// </summary>
	/// <returns>InputType</returns>
	inline const InputType GetInputType(void) const
	{
		return currentInputType_;
	}

	/// <summary>
	/// キーかコントローラーかの種別をセット
	/// </summary>
	/// <param name="type">セットしたいCONTROL_TYPE</param>
	void SetControlType(const CONTROL_TYPE type);

	/// <summary>
	///キーかコントローラーかの種別をセット
	/// </summary>
	/// <param name="type">CONTROL_TYPEに対応したint型</param>
	void SetControlType(const int type);

	/// <summary>
	/// キーかコントローラーかの種別を取得
	/// </summary>
	/// <returns>CONTROL_TYPE</returns>
	inline const CONTROL_TYPE GetControlType(void) const
	{
		return controlType_;
	}

	/// <summary>
	///　キー入力状態か
	/// </summary>
	/// <returns>
	/// true : キー入力
	/// false : キー入力ではない
	/// </returns>
	const bool IsControlType_Key(void) const;

	/// <summary>
	/// コントローラー入力状態か
	/// </summary>
	/// <returns>
	/// true : コントローラー入力か
	/// false : コントローラー入力ではない
	/// </returns>
	const bool IsControlType_Controller(void) const;

	/// <summary>
	/// コントローラーの感度をセット
	/// </summary>
	/// <param name="sens">コントローラーの感度(割合)</param>
	void SetControllerSensitivity(const float sens);

	/// <summary>
	/// コントローラーの感度を取得
	/// </summary>
	/// <returns>controllerSensitivity_のconst参照</returns>
	inline const float& GetControllerSensitivity(void) const
	{
		return controllerSensitivity_;
	}

private:

	//シングルトンテンプレートにインスタンスの生成を許可
	friend class Singleton<InputManager>;

	//入力とアクションの対応テーブル
	std::unordered_map<ACTION, std::vector<InputMapInfo>> inputActionMaps_;

	//現在の入力状態
	std::unordered_map<ACTION, bool> currentInput_;

	//現在の入力タイプ
	InputType currentInputType_;

	//直前の入力状態
	std::unordered_map<ACTION, bool> lastInput_;

	//コントローラーの種別
	JOYPAD_TYPE joyPadType_;

	//コントローラーの入力状態
	JOYPAD_IN_STATE joyPadState_;

	//DirectInputで取得出来るコントローラーの入力状態
	DINPUT_JOYSTATE dstate_;

	//XInputで取得出来るコントローラーの入力状態
	XINPUT_STATE xstate_;

	//キー入力かコントローラーの入力か
	CONTROL_TYPE controlType_;

	//スティックの入力状態
	JOYPAD_STICK stick_;

	//コントローラーの感度(割合)
	float controllerSensitivity_;

	InputManager(void);
	~InputManager(void);

	/// <summary>
	/// DirectInput()から取得出来る入力情報の取得
	/// </summary>
	/// <param name="no">コントローラーの番号</param>
	/// <returns>DINPUT_JOYSTATE</returns>
	DINPUT_JOYSTATE GetJPadDInputState(const int no);

	/// <summary>
	/// XInput()から取得出来る入力情報の取得
	/// </summary>
	/// <param name="no">コントローラーの番号</param>
	/// <returns>XINPUT_STATE</returns>
	XINPUT_STATE GetJPadXInputState(const int no);

	//指定したコントローラーの入力情報の取得
	void SetJPadInState(const JOYPAD_NO joypadNo);

	//接続したコントローラーの数を取得
	int GetJPadNum(void);

	//スティックの入力販売
	const bool GetHitJPadStick(const InputMapInfo& info, const int& pad);

	//キーボードの入力判定
	const bool GetHitKey(const InputMapInfo& info, const char* key, const int& pad, const int& mouse);

	//コントローラーボタンの入力判定
	const bool GetHitJPad(const InputMapInfo& info, const int& pad);

	//スティックの入力判定
	const bool IsInputStick(const int stick, const int max);

};

