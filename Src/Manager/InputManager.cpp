#include <Dxlib.h>
#include "InputManager.h"

using namespace std;

const float DEF_CONTROLLER_SENSITIVITY = 0.5f;

InputManager::InputManager(void)
    :
    inputActionMaps_{},
    currentInput_{},
    currentInputType_(InputType::KEYBOARD),
    lastInput_{},
    joyPadType_(JOYPAD_TYPE::XBOX_ONE),
    joyPadState_(JOYPAD_IN_STATE()),
    dstate_(DINPUT_JOYSTATE()),
    xstate_(XINPUT_STATE()),
    controlType_(CONTROL_TYPE::KEY),
    stick_(JOYPAD_STICK()),
    controllerSensitivity_(DEF_CONTROLLER_SENSITIVITY)
{

    //キー情報の入力
    inputActionMaps_[ACTION::ENTER] =
    {
        {InputType::KEYBOARD,KEY_INPUT_RETURN},
        {InputType::GAMEPAD,(int)JOYPAD_BTN::RIGHT},
        {InputType::MOUSE,MOUSE_INPUT_LEFT},
    };

#pragma region 選択画面用

    inputActionMaps_[ACTION::SELECT_UP] =
    {
        {InputType::KEYBOARD,KEY_INPUT_UP},
        {InputType::KEYBOARD,KEY_INPUT_W},
        {InputType::GAMEPAD_STICK,(int)JOYPAD_STICK::L_STICK_UP},
        {InputType::GAMEPAD,(int)JOYPAD_BTN::TEN_UP},
    };

    inputActionMaps_[ACTION::SELECT_LEFT] =
    {
        {InputType::KEYBOARD,KEY_INPUT_LEFT},
        {InputType::KEYBOARD,KEY_INPUT_A},
        {InputType::GAMEPAD_STICK,(int)JOYPAD_STICK::L_STICK_LEFT},
        {InputType::GAMEPAD,(int)JOYPAD_BTN::TEN_LEFT},
    };

    inputActionMaps_[ACTION::SELECT_RIGHT] =
    {
        {InputType::KEYBOARD,KEY_INPUT_RIGHT},
        {InputType::KEYBOARD,KEY_INPUT_D},
        {InputType::GAMEPAD_STICK,(int)JOYPAD_STICK::L_STICK_RIGHT},
        {InputType::GAMEPAD,(int)JOYPAD_BTN::TEN_RIGHT},
    };

    inputActionMaps_[ACTION::SELECT_DOWN] =
    {
        {InputType::KEYBOARD,KEY_INPUT_DOWN},
        {InputType::KEYBOARD,KEY_INPUT_S},
        {InputType::GAMEPAD_STICK,(int)JOYPAD_STICK::L_STICK_DOWN},
        {InputType::GAMEPAD,(int)JOYPAD_BTN::TEN_DOWN},
    };

    inputActionMaps_[ACTION::SELECT_ENTER] =
    {
        {InputType::KEYBOARD,KEY_INPUT_RETURN},
        {InputType::GAMEPAD,(int)JOYPAD_BTN::RIGHT},
        {InputType::MOUSE,MOUSE_INPUT_LEFT},
    };

    inputActionMaps_[ACTION::SELECT_CANCEL] =
    {
        {InputType::KEYBOARD,KEY_INPUT_Q},
        {InputType::GAMEPAD,(int)JOYPAD_BTN::DOWN },
        {InputType::MOUSE,MOUSE_INPUT_RIGHT},

    };

#pragma endregion

    inputActionMaps_[ACTION::ATTACK] =
    {
        //{InputType::KEYBOARD,KEY_INPUT_K},
        {InputType::GAMEPAD,(int)JOYPAD_BTN::TOP},
        {InputType::MOUSE,MOUSE_INPUT_LEFT},
    };

    inputActionMaps_[ACTION::PAUSE] =
    {
        {InputType::KEYBOARD,KEY_INPUT_P},
        {InputType::GAMEPAD,(int)JOYPAD_BTN::START},
    };

    inputActionMaps_[ACTION::SCENE_CHANGE] =
    {
        {InputType::KEYBOARD ,KEY_INPUT_SPACE },
        //{InputType::GAMEPAD,(int)JOYPAD_BTN::START}

    };

    inputActionMaps_[ACTION::DODGE] =
    {
        {InputType::KEYBOARD,KEY_INPUT_C},
        {InputType::GAMEPAD,(int)JOYPAD_BTN::DOWN},
        //{InputType::MOUSE,MOUSE_INPUT_MIDDLE},
    };

    inputActionMaps_[ACTION::RUN] =
    {
        {InputType::KEYBOARD,KEY_INPUT_LSHIFT},
        {InputType::GAMEPAD,(int)JOYPAD_BTN::R_TRIGGER},
        //{InputType::GAMEPAD,(int)JOYPAD_BTN::LEFT},
        //{InputType::MOUSE,MOUSE_INPUT_MIDDLE},
    };

    inputActionMaps_[ACTION::PULL] =
    {
        {InputType::KEYBOARD,KEY_INPUT_G},
        {InputType::GAMEPAD,(int)JOYPAD_BTN::ZR_TRIGGER},
    };

    inputActionMaps_[ACTION::DRAW_UNDRAW] =
    {
        {InputType::KEYBOARD,KEY_INPUT_Q},
        {InputType::GAMEPAD,(int)JOYPAD_BTN::LEFT},
        //{InputType::MOUSE,MOUSE_INPUT_MIDDLE},
    };

    inputActionMaps_[ACTION::BLOCK] =
    {
        //{InputType::KEYBOARD,KEY_INPUT_P},
        {InputType::GAMEPAD,(int)JOYPAD_BTN::ZL_TRIGGER},
        {InputType::MOUSE,MOUSE_INPUT_RIGHT},
    };

    //移動系
    inputActionMaps_[ACTION::MOVE_FORWARD] =
    {
        {InputType::KEYBOARD ,KEY_INPUT_W },
    };

    inputActionMaps_[ACTION::MOVE_BACK] =
    {
        {InputType::KEYBOARD ,KEY_INPUT_S },
    };

    inputActionMaps_[ACTION::MOVE_LEFT] =
    {
        {InputType::KEYBOARD ,KEY_INPUT_A },
    };

    inputActionMaps_[ACTION::MOVE_RIGHT] =
    {
        {InputType::KEYBOARD ,KEY_INPUT_D },
    };

    inputActionMaps_[ACTION::MOVE_PAD] =
    {
        {InputType::GAMEPAD ,(int)JOYPAD_BTN::ZL_TRIGGER },
    };

    inputActionMaps_[ACTION::DEBUG] =
    {
        {InputType::KEYBOARD ,KEY_INPUT_BACKSLASH },
    };
    inputActionMaps_[ACTION::DEBUGCON] =
    {
        {InputType::KEYBOARD ,KEY_INPUT_LCONTROL },
    };

    inputActionMaps_[ACTION::ITEMCALL] =
    {
        {InputType::KEYBOARD ,KEY_INPUT_R ,"R"},
        {InputType::GAMEPAD,(int)JOYPAD_BTN::LEFT,"X/□"},
    };

    inputActionMaps_[ACTION::NEXTITEM] =
    {
        //{InputType::GAMEPAD,(int)JOYPAD_BTN::R_TRIGGER},
        {InputType::GAMEPAD,(int)JOYPAD_BTN::TEN_RIGHT},
    };

    inputActionMaps_[ACTION::PREITEM] =
    {
        //{InputType::GAMEPAD,(int)JOYPAD_BTN::L_TRIGGER},
        {InputType::GAMEPAD,(int)JOYPAD_BTN::TEN_LEFT},
    };

    inputActionMaps_[ACTION::USEITEM] =
    {
        {InputType::KEYBOARD ,KEY_INPUT_E ,"E"},
        {InputType::MOUSE,MOUSE_INPUT_MIDDLE},
        {InputType::GAMEPAD,(int)JOYPAD_BTN::TEN_DOWN},
    };

    inputActionMaps_[ACTION::SELECT] =
    {
        {InputType::KEYBOARD ,KEY_INPUT_F ,"F"},
        {InputType::GAMEPAD,(int)JOYPAD_BTN::RIGHT},

    };
    inputActionMaps_[ACTION::INHALE] =
    {
        {InputType::KEYBOARD ,KEY_INPUT_V, "V"},
        {InputType::GAMEPAD,(int)JOYPAD_BTN::DOWN},

    };

    inputActionMaps_[ACTION::DEBUG_EFFECT] =
    {
        {InputType::KEYBOARD ,KEY_INPUT_L,"L"},
    };


    //カメラ
    inputActionMaps_[ACTION::CAM_UP] =
    {
        {InputType::KEYBOARD ,KEY_INPUT_UP,"↑"},
    };

    inputActionMaps_[ACTION::CAM_DOWN] =
    {
        {InputType::KEYBOARD ,KEY_INPUT_DOWN,"↓"},
    };

    inputActionMaps_[ACTION::CAM_LEFT] =
    {
        {InputType::KEYBOARD ,KEY_INPUT_LEFT,"←"},
    };

    inputActionMaps_[ACTION::CAM_RIGHT] =
    {
        {InputType::KEYBOARD ,KEY_INPUT_RIGHT,"→"},
    };

}

InputManager::~InputManager(void)
{
    for (auto& iam : inputActionMaps_)
    {
        iam.second.clear();
    }
    inputActionMaps_.clear();

    currentInput_.clear();
    lastInput_.clear();
}

void InputManager::Destroy(void)
{
}

void InputManager::Update(void)
{

    lastInput_ = currentInput_;

    SetJPadInState(JOYPAD_NO::PAD1);

    //全ての入力情報を取得
    char keyState[256] = {};
    GetHitKeyStateAll(keyState);

    int padState = {};
    padState = GetJoypadInputState(DX_INPUT_PAD1);

    int mouseState = {};
    mouseState = GetMouseInput();

    //それぞれのアクション名に割り当てている全ての入力チェック
    for (const auto& mapInfo : inputActionMaps_)
    {
        bool isPressed = false;

        for (const auto& inputInfo : mapInfo.second)
        {
            isPressed = GetHitKey(inputInfo, keyState, padState, mouseState);
            if (isPressed)
            {
                currentInputType_ = inputInfo.type;
                //入力していたらループ終了
                break;
            }
        }

        //入力情報を更新
        currentInput_[mapInfo.first] = isPressed;
    }
}

bool InputManager::IsPressed(const ACTION& action) const
{
    auto ret = currentInput_.find(action);
    if (ret == currentInput_.end())
    {
        //入力情報に存在していない
        return false;
    }
    else
    {
        return ret->second;
    }

}

bool InputManager::IsTriggered(const ACTION& action) const
{
    if (IsPressed(action)) {
        auto ret = lastInput_.find(action);
        if (ret == lastInput_.end())
        {            //直前に押下していない
            return true;
        }
        else
        {
            //直前に押下している
            return !ret->second;
        }
    }
    else
    {
        return false;
    }
}


bool InputManager::IsReleased(const ACTION& action) const
{
    if (!IsPressed(action)) {

        auto ret = lastInput_.find(action);
        if (ret == lastInput_.end())
        {
            //直前に押下していない
            return false;
        }
        else
        {
            //直前に押下している
            return ret->second;
        }
    }
    else
    {
        return false;
    }
}

const JOYPAD_TYPE InputManager::GetJPadType(const JOYPAD_NO no)
{
    return (JOYPAD_TYPE)(GetJoypadType((int)no));
}

const JOYPAD_IN_STATE InputManager::GetJPadState(const JOYPAD_NO no)
{

    JOYPAD_IN_STATE ret = JOYPAD_IN_STATE();

    auto type = GetJPadType(no);

    if (type == JOYPAD_TYPE::XBOX_ONE)
    {
        //XboxOneの入力状態

        //DirectInputで取得出来る入力状態
        auto d = GetJPadDInputState(1);

        //XInputで取得出来る入力状態
        auto x = GetJPadXInputState(1);

        int idx;

        //Yボタン
        idx = static_cast<int>(JOYPAD_BTN::TOP);
        ret.ButtonsNew[idx] = d.Buttons[3];

        //Xボタン
        idx = static_cast<int>(JOYPAD_BTN::LEFT);
        ret.ButtonsNew[idx] = d.Buttons[2];

        //Bボタン
        idx = static_cast<int>(JOYPAD_BTN::RIGHT);
        ret.ButtonsNew[idx] = d.Buttons[1];

        //Aボタン
        idx = static_cast<int>(JOYPAD_BTN::DOWN);
        ret.ButtonsNew[idx] = d.Buttons[0];

        //Lボタン
        idx = static_cast<int>(JOYPAD_BTN::L_TRIGGER);
        ret.ButtonsNew[idx] = d.Buttons[4];

        //Rボタン
        idx = static_cast<int>(JOYPAD_BTN::R_TRIGGER);
        ret.ButtonsNew[idx] = d.Buttons[5];

        //XBOXボタン
        idx = static_cast<int>(JOYPAD_BTN::CENTER);
        ret.ButtonsNew[idx] = d.Buttons[6];

        //Startボタン
        idx = static_cast<int>(JOYPAD_BTN::START);
        ret.ButtonsNew[idx] = d.Buttons[7];

        //ZR
        idx = static_cast<int>(JOYPAD_BTN::ZR_TRIGGER);
        ret.ButtonsNew[idx] = x.RightTrigger;

        //ZL
        idx = static_cast<int>(JOYPAD_BTN::ZL_TRIGGER);
        ret.ButtonsNew[idx] = x.LeftTrigger;

        //デジタルスティック上
        idx = static_cast<int>(JOYPAD_BTN::TEN_UP);
        ret.ButtonsNew[idx] = x.Buttons[XINPUT_BUTTON_DPAD_UP];

        //デジタルスティック下
        idx = static_cast<int>(JOYPAD_BTN::TEN_DOWN);
        ret.ButtonsNew[idx] = x.Buttons[XINPUT_BUTTON_DPAD_DOWN];

        //デジタルスティック左
        idx = static_cast<int>(JOYPAD_BTN::TEN_LEFT);
        ret.ButtonsNew[idx] = x.Buttons[XINPUT_BUTTON_DPAD_LEFT];

        //デジタルスティック右
        idx = static_cast<int>(JOYPAD_BTN::TEN_RIGHT);
        ret.ButtonsNew[idx] = x.Buttons[XINPUT_BUTTON_DPAD_RIGHT];

        // 左アナログスティック
        ret.AkeyL.x = d.X;
        ret.AkeyL.y = d.Y;

        // 右アナログスティック
        ret.AkeyR.x = d.Rx;
        ret.AkeyR.y = d.Ry;


        //左スティック関連
        //左入力
        idx = static_cast<int>(JOYPAD_STICK::L_STICK_LEFT);
        bool isHit = IsInputStick(ret.AkeyL.x, STICK_X_LEFT_XBOX_ONE);
        ret.aKeyNew[idx] = isHit;

        //右入力
        idx = static_cast<int>(JOYPAD_STICK::L_STICK_RIGHT);
        isHit = IsInputStick(ret.AkeyL.x, STICK_X_RIGHT_XBOX_ONE);
        ret.aKeyNew[idx] = isHit;

        //上入力
        idx = static_cast<int>(JOYPAD_STICK::L_STICK_UP);
        isHit = IsInputStick(ret.AkeyL.y, STICK_Y_UP_XBOX_ONE);
        ret.aKeyNew[idx] = isHit;

        //下入力
        idx = static_cast<int>(JOYPAD_STICK::L_STICK_DOWN);
        isHit = IsInputStick(ret.AkeyL.y, STICK_Y_DOWN_XBOX_ONE);
        ret.aKeyNew[idx] = isHit;

        //スティック関連
        //左入力
        idx = static_cast<int>(JOYPAD_STICK::R_STICK_LEFT);
        isHit = IsInputStick(ret.AkeyR.x, STICK_X_LEFT_XBOX_ONE);
        ret.aKeyNew[idx] = isHit;

        //右入力
        idx = static_cast<int>(JOYPAD_STICK::R_STICK_RIGHT);
        isHit = IsInputStick(ret.AkeyR.x, STICK_X_RIGHT_XBOX_ONE);
        ret.aKeyNew[idx] = isHit;

        //上入力
        idx = static_cast<int>(JOYPAD_STICK::R_STICK_UP);
        isHit = IsInputStick(ret.AkeyR.y, STICK_Y_UP_XBOX_ONE);
        ret.aKeyNew[idx] = isHit;

        //下入力
        idx = static_cast<int>(JOYPAD_STICK::R_STICK_DOWN);
        isHit = IsInputStick(ret.AkeyR.y, STICK_Y_DOWN_XBOX_ONE);
        ret.aKeyNew[idx] = isHit;
    }

    return ret;

}

void InputManager::GetMousePos(Vector2& pos)
{
    GetMousePoint(&pos.x, &pos.y);
}

const std::string& InputManager::GetName(const ACTION& action) const
{

    int num;

    if (currentInputType_ == InputType::KEYBOARD)
    {
        num = 0;
    }
    else
    {
        num = 1;
    }

    auto it = inputActionMaps_.find(action);
    if (it != inputActionMaps_.end() && !it->second.empty())
    {
        return it->second[num].KBname;
    }

    return "Unknown";

}

void InputManager::SetControlType(const CONTROL_TYPE type)
{
    controlType_ = type;
}

void InputManager::SetControlType(const int type)
{
    controlType_ = (CONTROL_TYPE)type;
}

const bool InputManager::IsControlType_Key(void) const
{
    return controlType_ == CONTROL_TYPE::KEY;
}

const bool InputManager::IsControlType_Controller(void) const
{
    return controlType_ == CONTROL_TYPE::CONTROLLER;
}


DINPUT_JOYSTATE InputManager::GetJPadDInputState(const int no)
{
    GetJoypadDirectInputState(no, &dstate_);
    return dstate_;
}

XINPUT_STATE InputManager::GetJPadXInputState(const int no)
{
    GetJoypadXInputState(no, &xstate_);
    return xstate_;
}

void InputManager::SetJPadInState(const JOYPAD_NO joypadNo)
{
    int no = (int)joypadNo;
    auto stateNew = GetJPadState(joypadNo);

    auto& stateNow = joyPadState_;

    int max = static_cast<int>(JOYPAD_BTN::MAX);
    for (int n = 0; n < max; n++)
    {
        stateNow.ButtonsOld[n] = stateNow.ButtonsNew[n];
        stateNow.ButtonsNew[n] = stateNew.ButtonsNew[n];

        stateNow.AkeyL.x = stateNew.AkeyL.x;
        stateNow.AkeyL.y = stateNew.AkeyL.y;
        stateNow.AkeyR.x = stateNew.AkeyR.x;
        stateNow.AkeyR.y = stateNew.AkeyR.y;
    }

    max = static_cast<int>(JOYPAD_STICK::MAX);
    for (int n = 0; n < max; n++)
    {
        stateNow.aKeyOld[n] = stateNew.aKeyNew[n];
        stateNow.aKeyNew[n] = stateNew.aKeyNew[n];
    }
}

int InputManager::GetJPadNum(void)
{
    return GetJoypadNum();
}

const bool InputManager::GetHitKey(const InputMapInfo& info, const char* key, const int& pad, const int& mouse)
{
    return
        (info.type == InputType::KEYBOARD && key[info.buttonID_]) ||
        (GetHitJPad(info, pad)) ||
        (GetHitJPadStick(info, pad)) ||
        (info.type == InputType::MOUSE && mouse & info.buttonID_);
}

const bool InputManager::GetHitJPadStick(const InputMapInfo& info, const int& pad)
{
    bool ret = false;

    if (info.type != InputType::GAMEPAD_STICK) return ret;

    return joyPadState_.aKeyNew[info.buttonID_];
}

const bool InputManager::GetHitJPad(const InputMapInfo& info, const int& pad)
{
    bool ret = false;

    if (joyPadState_.ButtonsNew[info.buttonID_])
    {
        ret = true;
    }

    return info.type == InputType::GAMEPAD && ret;
}

void InputManager::SetControllerSensitivity(const float sens)
{
    controllerSensitivity_ = sens;
}

const bool InputManager::IsInputStick(const int stick, const int max)
{
    auto stickMax = (int)((float)max * controllerSensitivity_);

    //符号の違いを確認
    return
        stickMax < STICK_NEUTRAL_XBOX_ONE
        ? stick < stickMax
        : stick > stickMax;
}