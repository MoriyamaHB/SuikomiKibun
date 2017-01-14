#ifndef SUIKOMIKIBUN_INPUT_INPUT_H_
#define SUIKOMIKIBUN_INPUT_INPUT_H_

#include <string.h>
#include <GL/freeglut.h>
#include "../util/uGL.h"

//Checkと先頭につく関数はOpenGLからコールバックさせる必要があります
namespace input {
//コールバックによるイベント取得
void CheckPushKey(unsigned char key, int x, int y);
void CheckUpkey(unsigned char key, int x, int y);
void CheckPushSpecialKey(int key, int x, int y);
void CheckUpSpecialkey(int key, int x, int y);
void CheckMouse(int button, int state, int x, int y);
void CheckMouseMotion(int x, int y);
//更新:毎フレーム呼び出す
void UpdateFrame();
//入力状態を取得する
int get_keyboard_frame(unsigned char key);
int get_special_keyboard_frame(int key);
int get_mouse_left_button_frame();
int get_mouse_right_button_frame();
int get_mouse_x();
int get_mouse_y();
//setter
void set_is_enabled_mouse_motion(bool boo);
//初期化
void Init();

//前回呼び出した時からのマウスの移動量を返す
void TakeMouseMotionAndInit(int *dx, int *dy);
}

#endif
