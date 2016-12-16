#ifndef SUIKOMIKIBUN_TIME_TIME_H_
#define SUIKOMIKIBUN_TIME_TIME_H_

#include <time.h>
#include "../gv.h"

class LimitedTime {
private:
	time_t start_; //スタート時間
	time_t limited_time_sec_; //制限時間
	bool is_measuring; //測定中か
	time_t time_remaining_;

public:
	LimitedTime();
	void Init(int limited_time_sec);
	int Update();
	void Draw() const;

	time_t get_time_remaining() const;
};

#endif
