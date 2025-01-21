#include "../Utility/Utility.h"
#include "TimeManager.h"

const float INIT_TIME = Utility::FLOAT_ZERO;
const float INIT_DELTATIME = Utility::FLOAT_ZERO;
const bool INIT_IS_STOP = false;

void TimeManager::Destroy(void)
{
    time_ = 0.0f;
    deltaTime_ = 0.0f;


    //delete instance_;
}

void TimeManager::Update(void)
{
    // デルタタイム
    const auto& nowTime = std::chrono::system_clock::now();
    
    deltaTime_ = static_cast<float>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
    
    preTime_ = nowTime;


    if (isStop_) { return; }

    time_ += deltaTime_;

}

void TimeManager::ResetDeltaTime(void)
{
    deltaTime_ = 0.016f;
    preTime_ = std::chrono::system_clock::now();
    time_ = 0.0f;
}

void TimeManager::SetStop(bool isStop)
{
    isStop_ = isStop;
}

const bool TimeManager::GetIsStop(void)
{
    return isStop_;
}

const float TimeManager::GetTime(void)
{
    return time_;
}

const float TimeManager::GetDeltaTime(void)
{
    return deltaTime_;
}

TimeManager::TimeManager(void)
    :
    time_(INIT_TIME),
    deltaTime_(INIT_DELTATIME),
    isStop_(INIT_IS_STOP)
{
}
