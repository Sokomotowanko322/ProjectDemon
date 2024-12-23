#include <DxLib.h>
#include "FpsControl.h"



FpsControl* FpsControl::instance_ = nullptr;

void FpsControl::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new FpsControl();
	}
	instance_->Init();
}

FpsControl& FpsControl::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		FpsControl::CreateInstance();
	}
	return *instance_;
}

FpsControl::FpsControl()
{
	mStartTime = 0;
	mCount = 0;
	mFps = 0;
}

void FpsControl::Init(void)
{
	mStartTime = 0;
	mCount = 0;
	mFps = 0;
}

bool FpsControl::Update()
{
	if (mCount == 0) { //1�t���[���ڂȂ玞�����L��
		mStartTime = GetNowCount();
	}
	if (mCount == N) { //60�t���[���ڂȂ畽�ς��v�Z����
		int t = GetNowCount();
		mFps = 1000.f / ((t - mStartTime) / (float)N);
		mCount = 0;
		mStartTime = t;
	}
	mCount++;
	return true;
}

void FpsControl::Draw()
{
	DrawFormatString(0, 0, GetColor(255, 255, 255), "%.1f", mFps);
}

void FpsControl::Wait()
{
	int tookTime = GetNowCount() - mStartTime;	//������������
	int waitTime = mCount * 1000 / FPS - tookTime;	//�҂ׂ�����
	if (waitTime > 0) {
		Sleep(waitTime);	//�ҋ@
	}
}
