#include <math.h>
#include "DxLib.h"

class FpsControl {
	int mStartTime;         //測定開始時刻
	int mCount;             //カウンタ
	float mFps;             //fps
	static const int N = 60;//平均を取るサンプル数
	static const int FPS = 60;	//設定したFPS

public:
	// インスタンスを明示的に生成
	static void CreateInstance(void);

	// インスタンスの取得
	static FpsControl& GetInstance(void);
	FpsControl();
	void Init(void);
	bool Update();
	void Draw();
	void Wait();

private:
	// シングルトン用インスタンス
	static FpsControl* instance_;
	
};
