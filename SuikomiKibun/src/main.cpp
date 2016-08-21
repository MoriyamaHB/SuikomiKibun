#include <GL/freeglut.h>
#include "define.h"

//ゲーム起動時に行う初期化
void FirstInit(int argc, char *argv[]) {
	//openGLの初期化,ウィンドウ生成
	glutInitWindowSize(kWindowInitWidth, kWindowInitHeight);
	glutInitWindowPosition(kWindowInitPosX, kWindowInitPosY);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow(argv[0]);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS); //メインループ終了後も処理を続ける
}

//メイン
int main(int argc, char *argv[]) {
	FirstInit(argc, argv);
	glutMainLoop();
	return 0;
}
