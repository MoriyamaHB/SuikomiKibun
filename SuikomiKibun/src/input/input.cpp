#include "input.h"

///////////////////////// キー入力 /////////////////////////

//キーの入力状態記録
namespace {
unsigned int keyboard[26];
unsigned int keyboard_frame[26];
unsigned int escape;
unsigned int escape_frame;
unsigned int shift;
unsigned int shift_frame;
unsigned int enter;
unsigned int enter_frame;
unsigned int space;
unsigned int space_frame;

bool is_enabled_mouse_motion; //マウスの移動量取得(マウスをセンターに戻す)を有効にしたか
}

//キーボードの入力フレーム数を返す
namespace input {
int get_keyboard_frame(unsigned char key) {
	if ('a' <= key && key <= 'z') {
		return keyboard_frame[key - 'a'];
	} else if (key == '\033') {
		return escape_frame;
	} else if (key == 13) {
		return enter_frame;
	} else if (key == ' ') {
		return space_frame;
	} else {
		uErrorOut(__FILE__, __func__, __LINE__, "keyの値が不正です");
		return -1;
	}
}
}

//特殊キーの入力フレーム数を返す
namespace input {
int get_special_keyboard_frame(int key) {
	if (key == GLUT_KEY_SHIFT_L) {
		return shift_frame;
	} else {
		uErrorOut(__FILE__, __func__, __LINE__, "keyの値が不正です");
		return -1;
	}
}
}

//OpenGLコールバック関数
namespace input {
void CheckPushKey(unsigned char key, int x, int y) {
	//キーボードの取得
	if ('a' <= key && key <= 'z') {
		keyboard[key - 'a'] = 1;
	} else if ('A' <= key && key <= 'Z') {
		keyboard[key - 'A'] = 1;
	} else if (key == '\033') {
		escape = 1;
	} else if (key == 13) {
		enter = 1;
	} else if (key == ' ') {
		space = 1;
	}
}
}

//OpenGLコールバック関数
namespace input {
void CheckUpkey(unsigned char key, int x, int y) {
	//キーボードの取得
	if ('a' <= key && key <= 'z') {
		keyboard[key - 'a'] = 0;
	} else if ('A' <= key && key <= 'Z') {
		keyboard[key - 'A'] = 0;
	} else if (key == '\033') {
		escape = 0;
	} else if (key == 13) {
		enter = 0;
	} else if (key == ' ') {
		space = 0;
	}
}
}

//OpenGLコールバック関数
namespace input {
void CheckPushSpecialKey(int key, int x, int y) {
	if (key == GLUT_KEY_SHIFT_L)
		shift = 1;
	else
		printf("%d", key);
}
}

//OpenGLコールバック関数
namespace input {
void CheckUpSpecialkey(int key, int x, int y) {
	if (key == GLUT_KEY_SHIFT_L)
		shift = 0;
}
}

///////////////////////// マウス入力 /////////////////////////

//マウスの移動量
namespace {
int mouse_x = 0;
int mouse_y = 0;
int mouse_dx = 0;
int mouse_dy = 0;
bool is_down_mouse_left_button = false;
int mouse_left_button_frame = 0;
bool is_down_mouse_right_button = false;
int mouse_right_button_frame = 0;
}

//前回の呼び出しからのマウス移動量を返します
namespace input {
void TakeMouseMotionAndInit(int *dx, int *dy) {
	if (!is_enabled_mouse_motion) {
		//uErrorOut(__FILE__, __func__, __LINE__, "マウス移動量取得が無効になっています");
		*dx = 0;
		*dy = 0;
		return;
	}
	*dx = mouse_dx;
	*dy = mouse_dy;
	mouse_dx = mouse_dy = 0;
}
}

//左マウスクリックフレーム数を取得
namespace input {
int get_mouse_left_button_frame() {
	return mouse_left_button_frame;
}
int get_mouse_right_button_frame() {
	return mouse_right_button_frame;
}
}

//マウス座標取得
namespace input {
int get_mouse_x() {
	return mouse_x;
}
int get_mouse_y() {
	return mouse_y;
}
}

//OpenGLコールバック関数
//マウスクリックしたor放した時に呼び出されます
namespace input {
void CheckMouse(int button, int state, int x, int y) {
	//左クリック
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		is_down_mouse_left_button = true;
	} else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		is_down_mouse_left_button = false;
	}
	//右クリック
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		is_down_mouse_right_button = true;
	} else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
		is_down_mouse_right_button = false;
	}
}
}

//マウス移動量取得の設定
namespace input {
void set_is_enabled_mouse_motion(bool boo) {
	if (is_enabled_mouse_motion != boo) { //設定を変更するとき
		//boolを書き換え
		is_enabled_mouse_motion = boo;
		if (is_enabled_mouse_motion == true) {		//有効にするとき
			//カーソル変更
			glutSetCursor (GLUT_CURSOR_CROSSHAIR);
			//変数を初期化,マウスをセンターに移動
			mouse_dx = 0;
			mouse_dy = 0;
			int ww = glutGet(GLUT_WINDOW_WIDTH);
			int wh = glutGet(GLUT_WINDOW_HEIGHT);
			glutWarpPointer(ww / 2, wh / 2);
		} else {
			//カーソル変更
			glutSetCursor (GLUT_CURSOR_INHERIT);
		}
	}
}
}

//OpenGLコールバック関数
//ボタンを押している時 & 押していない時の両方で呼び出されます
namespace input {
void CheckMouseMotion(int x, int y) {
	//マウス座標記録
	mouse_x = x;
	mouse_y = y;

	//マウスラップ(マウスを中心に戻しそこからの移動量を記録)
	if (!is_enabled_mouse_motion)
		return;
	static int wrap_flag = 0;

	if (!wrap_flag) {
		int ww = glutGet(GLUT_WINDOW_WIDTH);
		int wh = glutGet(GLUT_WINDOW_HEIGHT);
		mouse_dx += x - (ww / 2);
		mouse_dy += y - (wh / 2);
		wrap_flag = 1;
		glutWarpPointer(ww / 2, wh / 2);
	} else {
		wrap_flag = 0;
	}
}
}

//デフォルトのカーソルを返す
namespace input {
int GetDefaultCursor() {
	if (is_enabled_mouse_motion)
		return GLUT_CURSOR_CROSSHAIR;
	else
		return GLUT_CURSOR_INHERIT;
}
}

///////////////////////// 更新 /////////////////////////

//押しているフレームをカウントする
namespace input {
void UpdateFrame() {
	//キーボードのフレームカウント
	for (int i = 0; i < 26; i++) {
		if (keyboard[i] == 1)
			keyboard_frame[i]++;
		else
			keyboard_frame[i] = 0;
	}
	if (escape)
		escape_frame++;
	else
		escape_frame = 0;
	if (shift)
		shift_frame++;
	else
		shift_frame = 0;
	if (enter)
		enter_frame++;
	else
		enter_frame = 0;
	if (space)
		space_frame++;
	else
		space_frame = 0;
	//左マウスクリック
	if (is_down_mouse_left_button)
		mouse_left_button_frame++;
	else
		mouse_left_button_frame = 0;
	//右マウスクリック
	if (is_down_mouse_right_button)
		mouse_right_button_frame++;
	else
		mouse_right_button_frame = 0;
}
}

///////////////////////// 初期化 /////////////////////////
namespace input {
void Init() {
	memset(keyboard, 0, sizeof(keyboard));
	memset(keyboard_frame, 0, sizeof(keyboard_frame));
	escape = 0;
	escape_frame = 0;
	shift = 0;
	shift_frame = 0;
	enter = 0;
	enter_frame = 0;
	space = 0;
	space_frame = 0;
	mouse_x = 0;
	mouse_y = 0;
	mouse_dx = 0;
	mouse_dy = 0;
	is_down_mouse_left_button = false;
	mouse_left_button_frame = 0;
	is_down_mouse_right_button = false;
	mouse_right_button_frame = 0;
	is_enabled_mouse_motion = false;
	//カーソル初期化
	glutSetCursor (GLUT_CURSOR_INHERIT);
}
}
