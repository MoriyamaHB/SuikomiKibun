#ifndef SUIKOMIKIBUN_OUTPUTDISPLAY_H_
#define SUIKOMIKIBUN_OUTPUTDISPLAY_H_

#include <iostream>
#include <string>
#include <FTGL/ftgl.h>
#include "../util/uGL.h"

class OutputDisplay {
private:
	const static int kMaxItem = 24; //最大登録数
	//登録項目
	struct {
		char string[128];
		float color[4];
		int life; //描画回数
		int flag;
	} item_[kMaxItem];
	int x0_, y0_; //描画開始座標
	unsigned int font_size_; //フォントの大きさ
	unsigned int line_space_; //行間幅
	FTPixmapFont font_; //フォント

	int SerchEmptyItem();

public:
	OutputDisplay(std::string font_path, int x0, int y0, unsigned int font_size);
	~OutputDisplay();
	void Init(); //登録している文字をすべて削除)
	void Regist(const std::string& string, const float color[], int life = 1); //項目を登録
	void Draw(); //描画(毎フレーム呼び出し)

	void set_line_space(unsigned int line_space);
};

#endif
