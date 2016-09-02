#include "output_display.h"

OutputDisplay::OutputDisplay(std::string font_path, int x0, int y0, unsigned int font_size) :
		font_(font_path.c_str()) {
	if (font_.Error()) {
		uErrorOut(__FILE__, __func__, __LINE__, "フォントが開けません");
	} else {
		font_.FaceSize(font_size);
	}
	x0_ = x0;
	y0_ = y0;
	font_size_ = font_size;
	line_space_ = 5;
	Init();
}

OutputDisplay::~OutputDisplay() {

}

//空のitemの添字を返します
//なければ-1を返します
int OutputDisplay::SerchEmptyItem() {
	for (int i = 0; i < kMaxItem; i++)
		if (item_[i].flag == 0)
			return i;
	return -1;
}

void OutputDisplay::Init() {
	memset(item_, 0, sizeof(item_));
}

//画面出力文字の登録
void OutputDisplay::Regist(const char str[], const float color[], int life) {
	int n = SerchEmptyItem();
	if (n == -1) { //満杯なら
		uErrorOut(__FILE__, __func__, __LINE__, "出力文字は満杯です");
		return;
	}

	//項目に登録する
	strcpy(item_[n].str, str);
	for (int i = 0; i < 4; i++)
		item_[n].color[i] = color[i];
	item_[n].life = life;
	item_[n].flag = 1;
}

//登録された文字列を描画
void OutputDisplay::Draw() {
	u3Dto2D();
	int dn = 0; //描画回数
	for (int i = 0; i < kMaxItem; i++)
		if (item_[i].flag == 1) {
			if (item_[i].life >= 1) { //ライフが1以上で描画
				//項目描画
				glColor4fv(item_[i].color);
				glRasterPos2f(x0_, y0_ + (font_size_ + line_space_) * dn);
				font_.Render(item_[i].str);
				dn++;
			}
			//削除処理
			if (--item_[i].life <= 0)
				item_[i].flag = 0;
		}
	u2Dto3D();
}

void OutputDisplay::set_line_space(unsigned int line_space) {
	line_space_ = line_space;
}

