#ifndef SUIKOMIKIBUN_UTIL_BUTTON_H_
#define SUIKOMIKIBUN_UTIL_BUTTON_H_

#include <FTGL/ftgl.h>
#include <GL/freeglut.h>
#include <memory>
#include <string>

#include "../input/input.h"
#include "uGL.h"
#include "../sound/sound.h"

class Button {
private:
	//ボタン位置
	int x1_, y1_, x2_, y2_;
	//ボタン色
	float button_color_[4];
	float button_active_color_[4];
	//テキスト
	float text_color_[4];
	float text_active_color_[4];
	std::string text_;
	unsigned int font_size_; //フォントの大きさ
	FTPixmapFont font_; //フォント
	//効果音
	static Sound *se_button_;

public:
	Button(int x1, int y1, int x2, int y2, const std::string& text, const std::string& font_path,
			unsigned int font_size);
	~Button();
	bool Update(); //押された時trueを返す
	void Draw();

	//setter
	void set_button_color(const float col[]);
	void set_button_active_color(const float col[]);
	void set_text_color(const float col[]);
	void set_text_active_color(const float col[]);
};

#endif /* SUIKOMIKIBUN_UTIL_BUTTON_H_ */
