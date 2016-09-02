#include "fps.h"

Fps::Fps() {
	memcpy(color_, uColor4fv_blue, sizeof(float) * 4);
	Init();
}

void Fps::Init() {
	GLframe_ = 0;
	GLtimenow_ = 0;
	GLtimebase_ = 0;
	frame_count_ = 0;
	fps_ = 0;
}

int Fps::get_frame_count() const {
	return frame_count_;
}

void Fps::set_color(const float color[]) {
	memcpy(color_, color, sizeof(float) * 4);
}

//計測したいスレッドにある関数から毎フレーム呼び出してください
void Fps::Update() {
	GLframe_++; //フレーム数を＋１
	frame_count_++; //フレームカウント
	GLtimenow_ = glutGet(GLUT_ELAPSED_TIME); //経過時間を取得
	if (GLtimenow_ - GLtimebase_ > 1000) { //１秒以上たったらfpsを出力
		fps_ = GLframe_ * 1000.0 / (GLtimenow_ - GLtimebase_);
		GLtimebase_ = GLtimenow_; //基準時間を設定
		GLframe_ = 0; //フレーム数をリセット
	}
}

//OutputDisplayにfps情報を登録します
void Fps::Draw(OutputDisplay *output) const {
	char string[256];
	sprintf(string, "fps:%.2f", fps_);
	output->Regist(string, color_);
}

//x0,y0の位置に計測したfpsの描画を登録します
void Fps::Draw(int x0, int y0) const {
	char string[256];
	sprintf(string, "fps:%.2f", fps_);
	uDrawString2(string, x0, y0, color_);
}
