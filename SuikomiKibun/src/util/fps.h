#ifndef SUIKOMIKIBUN_UTIL_FPS_H_
#define SUIKOMIKIBUN_UTIL_FPS_H_

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string.h>
#include "output_display.h"
#include "uGL.h"

class Fps {
private:
	int GLframe_; //フレーム数
	int GLtimenow_; //経過時間
	int GLtimebase_; //計測開始時間
	int frame_count_; //Updateが呼ばれた回数=開始からのフレーム数
	double fps_; //fps
	float color_[4]; //描画色

public:
	Fps();

	//初期化
	void Init();

	//getter,setter
	int get_frame_count() const;
	void set_color(const float color[]);

	//更新
	void Update();

	//描画
	void Draw(OutputDisplay *output) const;
	void Draw(int x0, int y0) const;
};

#endif

