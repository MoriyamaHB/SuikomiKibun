#include "time.h"

LimitedTime::LimitedTime() {
	limited_time_sec_ = 0;
	start_ = 0;
	is_measuring = false;
	time_remaining_ = 0;
}

//計測開始
void LimitedTime::Init(int limited_time_sec) {
	limited_time_sec_ = (time_t) limited_time_sec;
	is_measuring = true;
	start_ = time(NULL);
}

//制限時間更新
int LimitedTime::Update() {
	if (is_measuring) { //測定中なら
		//残り時間計算
		time_remaining_ = limited_time_sec_ - (time(NULL) - start_);
		//時間切れなら-1を返す
		if (time_remaining_ <= 0)
			return -1;
		else
			return 0;
	} else {
		time_remaining_ = 0;		//残り時間を0に設定
		return -1;		//未測定なら-1を返す
	}
}

//残り時間描画
void LimitedTime::Draw() const {
	char string[256];
	sprintf(string, "残り時間=%ld秒", time_remaining_);
	output_display0.Regist(string, uColor4fv_red, 1);
}

time_t LimitedTime::get_time_remaining() const {
	return time_remaining_;
}
