#include <GL/freeglut.h>
#include <AL/alut.h>
#include <gtest/gtest.h>

#include "define.h"
#include "util/uGL.h"
#include "util/fps.h"
#include "util/output_display.h"
#include "input/input.h"
#include "scene/scene_mgr.h"

void DisplayFunc(void);
void Resize(int w, int h);
void Timer(int value);

//無名名前空間
namespace {

//画面遷移管理
SceneMgr *scene_mgr;
//fps管理
Fps fps;

//ゲーム起動時に行う初期化
void FirstInit(int argc, char *argv[]) {
	//openGLの初期化,ウィンドウ生成
	glutInitWindowSize(kWindowInitWidth, kWindowInitHeight);
	glutInitWindowPosition(kWindowInitPosX, kWindowInitPosY);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow(argv[0]);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS); //メインループ終了後も処理を続ける

	//openglコールバック関数
	glutReshapeFunc(Resize);
	glutTimerFunc(100, Timer, 0);
	glutDisplayFunc(DisplayFunc);
	glutKeyboardFunc(input::CheckPushKey);
	glutKeyboardUpFunc(input::CheckUpkey);
	glutSpecialFunc(input::CheckPushSpecialKey);
	glutSpecialUpFunc(input::CheckUpSpecialkey);
	glutPassiveMotionFunc(input::CheckMouseMotion);
	glutMouseFunc(input::CheckMouse);
	glutMotionFunc(input::CheckMouseMotion);

	//その他openGLの設定
	glutIgnoreKeyRepeat(GL_TRUE); //繰り返しのキー入力を無視する
	glClearColor(uColor4fv_sky_brue[0], uColor4fv_sky_brue[1], uColor4fv_sky_brue[2], uColor4fv_sky_brue[3]); //塗りつぶし色を空色に設定
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//alutの初期化
	alutInit(&argc, argv);

	//ゲームの初期化
	scene_mgr = new SceneMgr(); //シーン遷移管理実体化
	fps.Init(); 				//fps初期化
	output_display::Init(); 	//ディスプレイ文字列初期化
}

//ゲーム終了時に行う処理
void EndFinalize() {
	//ゲームの終了処理
	delete scene_mgr;

	//ライブラリ終了処理
	alutExit();
}

}

//OpenGLコールバック関数
//リサイズ
void Resize(int w, int h) {
	//ビューポート設定
	glViewport(0, 0, w, h); //ウィンドウ全体をビューポートにする

	//透視変換行列設定
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); //透視変換行列の初期化
	gluPerspective(85.0, (double) w / (double) h, 0.1, 200.0);

	//モデルビュー変換行列の指定
	glMatrixMode(GL_MODELVIEW);
}

//OpenGLコールバック関数
//タイマー
void Timer(int value) {
	glutTimerFunc(1000 / kFps, Timer, 0);
	glutPostRedisplay(); //再描画
}

//OpenGLコールバック関数
//ディスプレイ
void DisplayFunc(void) {
	//ディスプレイ初期化
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //画面の初期化
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); //モデルビュー変換行列の初期化

	//入力更新
	input::UpdateFrame();

	//fps
	fps.Update();
	fps.Draw();

	//シーン
	scene_mgr->Update();
	scene_mgr->Draw();

	//ディスプレイ文字列描画
	output_display::Draw();

	//ディスプレイ終了処理
	glEnd();
	glEndList();
	glutSwapBuffers();

	if (input::get_keyboard_frame('\033') == 1) //Escを押すと
		glutLeaveMainLoop(); //メインループを抜ける
}

//メイン
int main(int argc, char *argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	FirstInit(argc, argv);
	glutMainLoop();
	EndFinalize();
	return RUN_ALL_TESTS();
}
