#include "button.h"

Sound* Button::se_button_ = NULL;

Button::Button(int x1, int y1, int x2, int y2, const std::string& text, const std::string& font_path,
		unsigned int font_size) :
		font_(font_path.c_str()) {
	//ボタン位置
	x1_ = x1;
	y1_ = y1;
	x2_ = x2;
	y2_ = y2;
	//ボタン色
	memcpy(button_color_, uColor4fv_brack, sizeof(button_color_));
	memcpy(button_active_color_, uColor4fv_red, sizeof(button_color_));
	//テキスト
	memcpy(text_color_, uColor4fv_white, sizeof(text_color_));
	memcpy(text_active_color_, uColor4fv_white, sizeof(text_color_));
	text_ = text;
	font_size_ = font_size;
	if (font_.Error()) {
		uErrorOut(__FILE__, __func__, __LINE__, "フォントが開けません");
	} else {
		font_.FaceSize(font_size_);
	}
	//効果音
	if (se_button_ == NULL)
		se_button_ = new Sound("sound/button.wav");
}

Button::~Button() {

}

bool Button::Update() {
	//効果音位置更新
	se_button_->SetSourceToListener();
	//クリックされていたらtrueを返す
	if (input::get_mouse_left_button_frame() == 1 || input::get_mouse_right_button_frame() == 1) {
		if (uIsCollisionSquareAndPoint(x1_, y1_, x2_, y2_, input::get_mouse_x(), input::get_mouse_y())) {
			se_button_->Play(); //効果音を鳴らす
			return true;
		}
	}
	return false;
}

void Button::Draw() {
	if (uIsCollisionSquareAndPoint(x1_, y1_, x2_, y2_, input::get_mouse_x(), input::get_mouse_y()))
		uSquare2D(x1_, y1_, x2_, y2_, 4.0, button_active_color_); //アクティブ時
	else
		uSquare2D(x1_, y1_, x2_, y2_, 4.0, button_color_); //アクティブ時
	u3Dto2D();
	if (uIsCollisionSquareAndPoint(x1_, y1_, x2_, y2_, input::get_mouse_x(), input::get_mouse_y()))
		glColor4fv(text_active_color_); //アクティブ時
	else
		glColor4fv(text_color_); //アクティブ時
	glRasterPos2f(x1_, y1_ + 50);
	if (!font_.Error())
		font_.Render(text_.c_str());
	u2Dto3D();
}

//setter
void Button::set_button_color(const float col[]) {
	memcpy(button_color_, col, sizeof(button_color_));
}

void Button::set_button_active_color(const float col[]) {
	memcpy(button_active_color_, col, sizeof(button_active_color_));
}

void Button::set_text_color(const float col[]) {
	memcpy(text_color_, col, sizeof(text_color_));
}
void Button::set_text_active_color(const float col[]) {
	memcpy(text_active_color_, col, sizeof(text_active_color_));
}
