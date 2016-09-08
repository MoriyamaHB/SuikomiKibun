#ifndef SUIKOMIKIBUN_UTIL_UGL_H_
#define SUIKOMIKIBUN_UTIL_UGL_H_

#include <GL/freeglut.h>
#include <FTGL/ftgl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <utility>
#include <stdarg.h>
#include <iostream>
#include <sstream>
#include <string>

#include "../util/vector3.h"

#define uPI 3.1415926


//色(4fv)
extern const float uColor4fv_brack[];
extern const float uColor4fv_white[];
extern const float uColor4fv_red[];
extern const float uColor4fv_green[];
extern const float uColor4fv_blue[];
extern const float uColor4fv_yellow[];
extern const float uColor4fv_gray[];
extern const float uColor4fv_orange[];
extern const float uColor4fv_sky_brue[];
extern const float uColor4fv_maroon[];
extern const float uColor4fv_jewel_green[];
extern const float uColor4fv_purple[];

//材質(4fv)
extern const float uMaterial4fv_red[];
extern const float uMaterial4fv_white[];
extern const float uMaterial4fv_black[];
extern const float uMaterial4fv_blue[];
extern const float uMaterial4fv_green[];
extern const float uMaterial4fv_brown[];

//エラー出力
extern void uErrorOut(const std::string file, const std::string func, int line, const std::string message);

//数値をstd::stringに変換
template<typename T> std::string uToStr(const T& t) {
	std::ostringstream os;
	os << t;
	return os.str();
}

//OpenGL用の強制終了関数(エラー時用)
extern void uExit();

//3D設定から2Dの設定に変更します
extern void u3Dto2D();

//2D設定から3Dの設定に戻す
extern void u2Dto3D();

//2D上にの点を描画する(size=直径)
extern void uPoint2D(int x, int y, float size);

//2D上に線を描画する
extern void uLine2D(int x1, int y1, int x2, int y2, float width);

//2D上に四角形を描画する
extern void uSquare2D(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, float width, const float color[]);
extern void uSquare2D(int x1, int y1, int x2, int y2, float width, const float color[]);

//2D上に四角形を描画する(塗りつぶし)
extern void uSquareFill2D(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, const float color[]);
extern void uSquareFill2D(int x1, int y1, int x2, int y2, const float color[]);

//2D上に円を描画
extern void uCircle2D(float radius, int x, int y);

//2D上に円を描画(塗りつぶし)
extern void uCircle2DFill(float radius, int x, int y);

//2D上に楕円を描画
extern void uOval2D(float radius, int x, int y, float ovalx, float ovaly);

//2D上に楕円を描画(塗りつぶし)
extern void uOval2DFill(float radius, int x, int y, float ovalx, float ovaly);

//三角形を描画
extern void uDrawTriangle(Vector3 v1, float color1[], Vector3 v2, float color2[], Vector3 v3, float color3[]);

//四角形を描画
extern void uDrawQuadrangle(Vector3 v1, float color1[], Vector3 v2, float color2[], Vector3 v3, float color3[],
		Vector3 v4, float color4[]);

//黒白の地面を描画
extern void uDrawGround(int size);

//可変長引数で文字列を返す
//linux環境では使えないみたいなのでコメントアウト
//const TCHAR* uPrintfString(const TCHAR* format, ...);

//文字列描画
extern void uDrawString(const char str[], int x0, int y0, const float color[]);

//文字列描画(ftgl版)
void uDrawString2(const char str[], int x0, int y0, const float color[]);

//範囲外の時trueを返す
extern bool uOutOfRange(Vector3 place, Vector3 v1, Vector3 v2);

//球と球の当たり判定
extern bool uIsCollisionBallAndBall(Vector3 b1, double r1, Vector3 b2, double r2, double *distance);

#endif /* SUIKOMIKIBUN_UTIL_UGL_H_ */
