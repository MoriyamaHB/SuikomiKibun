#include "uGL.h"

//色(4fv)
const float uColor4fv_brack[] = { 0.0f, 0.0f, 0.0f, 1.0f }; //黒
const float uColor4fv_white[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //白
const float uColor4fv_red[] = { 1.0f, 0.0f, 0.0f, 1.0f }; //赤
const float uColor4fv_green[] = { 0.0f, 1.0f, 0.0f, 1.0f }; //緑
const float uColor4fv_blue[] = { 0.0f, 0.0f, 1.0f, 1.0f }; //青
const float uColor4fv_yellow[] = { 1.0f, 1.0f, 0.0f, 1.0f }; //黄色
const float uColor4fv_gray[] = { 0.5f, 0.5f, 0.5f, 1.0f }; //灰色
const float uColor4fv_sky_brue[] = { 0.61176f, 0.65490f, 0.88627f, 1.0f }; //空色
const float uColor4fv_orange[] = { 1.0f, 0.71764f, 0.29803f, 1.0f }; //オレンジ
const float uColor4fv_maroon[] = { 0.46667f, 0.27450f, 0.20784f, 1.0f }; //栗色
const float uColor4fv_jewel_green[] = { 0.19607f, 0.80000f, 0.71372f, 1.0f }; //青竹色
const float uColor4fv_purple[] = { 0.76862f, 0.0f, 0.80000f, 1.0f }; //紫

//材質(4fv)
const float uMaterial4fv_red[] = { 0.8, 0.2, 0.2, 1.0 }; //赤
const float uMaterial4fv_white[] = { 0.9, 0.9, 0.9, 1.0 }; //白
const float uMaterial4fv_black[] = { 0.1, 0.1, 0.1, 1.0 }; //黒
const float uMaterial4fv_blue[] = { 0.1, 0.1, 0.9, 1.0 }; //青
const float uMaterial4fv_green[] = { 0.1, 0.9, 0.1, 1.0 }; //緑
const float uMaterial4fv_brown[] = { 0.8, 0.45, 0.2, 1.0 }; //茶色

static int u3d_2d_flag = 0;

//3D設定から2Dの設定に変更します
//関数の最後にu2Dto3D関数で3Dに戻してください
void u3Dto2D() {
	if (u3d_2d_flag != 0) {
		uErrorOut(__FILE__, __func__, __LINE__, "正しく呼びだされていない可能性があります");
		return;
	}
	//ライト削除
	glDisable(GL_LIGHTING);
	// 平行投影にする
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, glutGet(GLUT_INIT_WINDOW_WIDTH),
			glutGet(GLUT_INIT_WINDOW_HEIGHT), 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	u3d_2d_flag = 1;
}

//2D設定から3Dの設定に戻す
//u3Dto2D関数で変更した設定を戻すことを前提としています
void u2Dto3D() {
	if (u3d_2d_flag != 1) {
		uErrorOut(__FILE__, __func__, __LINE__, "正しく呼びだされていない可能性があります");
		return;
	}

	// 投射投影に戻す
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	//ライトを戻す
	glEnable(GL_LIGHTING);

	u3d_2d_flag = 0;
}

//エラー出力
void uErrorOut(const char file[], const char func[], int line,
		const char message[]) {
	fprintf(stderr, "<file:%s func:%s line:%d>\n", file, func, line);
	fprintf(stderr, "Error:%s\n\n", message);
}

//2D上にの点を描画する(size=直径)
void uPoint2D(int x, int y, float size) {
	u3Dto2D();
	glPointSize(size);
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
	u2Dto3D();
}

//2D上に線を描画する
void uLine2D(int x1, int y1, int x2, int y2, float width) {
	u3Dto2D();
	glLineWidth(width);
	glBegin(GL_LINES);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glEnd();
	u2Dto3D();
}

//2D上に四角形を描画する
void uSquare2D(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4,
		float width, const float color[]) {
	u3Dto2D();
	glColor4fv(color);
	glLineWidth(width);
	glBegin(GL_LINE_LOOP);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glVertex2i(x3, y3);
	glVertex2i(x4, y4);
	glEnd();
	u2Dto3D();
}

void uSquare2D(int x1, int y1, int x2, int y2, float width,
		const float color[]) {
	u3Dto2D();
	glColor4fv(color);
	glLineWidth(width);
	glBegin(GL_LINE_LOOP);
	glVertex2i(x1, y1);
	glVertex2i(x2, y1);
	glVertex2i(x2, y2);
	glVertex2i(x1, y2);
	glEnd();
	u2Dto3D();
}

//2D上に四角形を描画する(塗りつぶし)
void uSquareFill2D(int x1, int y1, int x2, int y2, int x3, int y3, int x4,
		int y4, const float color[]) {
	u3Dto2D();
	glColor4fv(color);
	glBegin(GL_QUADS);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glVertex2i(x3, y3);
	glVertex2i(x4, y4);
	glEnd();
	u2Dto3D();
}

void uSquareFill2D(int x1, int y1, int x2, int y2, const float color[]) {
	u3Dto2D();
	glColor4fv(color);
	glBegin(GL_QUADS);
	glVertex2i(x1, y1);
	glVertex2i(x2, y1);
	glVertex2i(x2, y2);
	glVertex2i(x1, y2);
	glEnd();
	u2Dto3D();
}

//2D上に円を描画
void uCircle2D(float radius, int x, int y) {
	u3Dto2D();
	for (float th1 = 0.0; th1 <= 360.0; th1 = th1 + 1.0) {
		float th2 = th1 + 10.0;
		float th1_rad = th1 / 180.0 * uPI;
		float th2_rad = th2 / 180.0 * uPI;

		float x1 = radius * cos(th1_rad);
		float y1 = radius * sin(th1_rad);
		float x2 = radius * cos(th2_rad);
		float y2 = radius * sin(th2_rad);

		glBegin(GL_LINES);
		glVertex2f(x1 + x, y1 + y);
		glVertex2f(x2 + x, y2 + y);
		glEnd();
	}
	u2Dto3D();
}

//2D上に円を描画(塗りつぶし)
void uCircle2DFill(float radius, int x, int y) {
	u3Dto2D();
	for (float th1 = 0.0; th1 <= 360.0; th1 = th1 + 1.0) {
		float th2 = th1 + 10.0;
		float th1_rad = th1 / 180.0 * uPI;
		float th2_rad = th2 / 180.0 * uPI;

		float x1 = radius * cos(th1_rad);
		float y1 = radius * sin(th1_rad);
		float x2 = radius * cos(th2_rad);
		float y2 = radius * sin(th2_rad);

		glBegin(GL_TRIANGLES);
		glVertex2f(x, y);
		glVertex2f(x1 + x, y1 + y);
		glVertex2f(x2 + x, y2 + y);
		glEnd();
	}
	u2Dto3D();
}

//2D上に楕円を描画
void uOval2D(float radius, int x, int y, float ovalx, float ovaly) {
	u3Dto2D();
	for (float th1 = 0.0; th1 <= 360.0; th1 = th1 + 1.0) {
		float th2 = th1 + 10.0;
		float th1_rad = th1 / 180.0 * uPI;
		float th2_rad = th2 / 180.0 * uPI;

		float x1 = radius * cos(th1_rad) * (ovalx / 100.0f);
		float y1 = radius * sin(th1_rad) * (ovaly / 100.0f);
		float x2 = radius * cos(th2_rad) * (ovalx / 100.0f);
		float y2 = radius * sin(th2_rad) * (ovaly / 100.0f);

		glBegin(GL_LINES);
		glVertex2f(x1 + x, y1 + y);
		glVertex2f(x2 + x, y2 + y);
		glEnd();
	}
	u2Dto3D();
}

//2D上に楕円を描画(塗りつぶし)
void uOval2DFill(float radius, int x, int y, float ovalx, float ovaly) {
	u3Dto2D();
	for (float th1 = 0.0; th1 <= 360.0; th1 = th1 + 1.0) {
		float th2 = th1 + 10.0;
		float th1_rad = th1 / 180.0 * uPI;
		float th2_rad = th2 / 180.0 * uPI;

		float x1 = radius * cos(th1_rad) * (ovalx / 100.0f);
		float y1 = radius * sin(th1_rad) * (ovaly / 100.0f);
		float x2 = radius * cos(th2_rad) * (ovalx / 100.0f);
		float y2 = radius * sin(th2_rad) * (ovaly / 100.0f);

		glBegin(GL_TRIANGLES);
		glVertex2f(x, y);
		glVertex2f(x1 + x, y1 + y);
		glVertex2f(x2 + x, y2 + y);
		glEnd();
	}
	u2Dto3D();
}

//三角形を描画
void uDrawTriangle(Vector3 v1, float color1[], Vector3 v2, float color2[],
		Vector3 v3, float color3[]) {
	u3Dto2D();
	glBegin(GL_TRIANGLES);
	glColor4fv(color1);
	glVertex3f(v1.x, v1.y, v1.z);
	glColor4fv(color2);
	glVertex3f(v2.x, v2.y, v2.z);
	glColor4fv(color3);
	glVertex3f(v3.x, v3.y, v3.z);
	glEnd();
	u2Dto3D();
}

//四角形を描画
void uDrawQuadrangle(Vector3 v1, float color1[], Vector3 v2, float color2[],
		Vector3 v3, float color3[], Vector3 v4, float color4[]) {
	u3Dto2D();
	glBegin(GL_QUADS);
	glColor4fv(color1);
	glVertex3f(v1.x, v1.y, v1.z);
	glColor4fv(color2);
	glVertex3f(v2.x, v2.y, v2.z);
	glColor4fv(color3);
	glVertex3f(v3.x, v3.y, v3.z);
	glColor4fv(color4);
	glVertex3f(v4.x, v4.y, v4.z);
	glEnd();
	u2Dto3D();
}

//黒白の地面を描画
void uDrawGround(int size) {
	if (size < 0 || 1000 < size) {
		uErrorOut(__FILE__, __func__, __LINE__, "サイズ指定値が不正です");
		return;
	}
	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);
	for (int j = -size / 2; j < size / 2; j++) {
		for (int i = -size / 2; i < size / 2; i++) {
			//RGB設定
			if (((i + j) % 2) == 0)
				glColor4fv(uColor4fv_white);
			else
				glColor4fv(uColor4fv_brack);
			//材質設定
			if (((i + j) % 2) == 0)
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, uMaterial4fv_white);
			else
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, uMaterial4fv_black);

			glVertex3d((GLdouble) i, 0.0, (GLdouble) j);
			glVertex3d((GLdouble) i, 0.0, (GLdouble) (j + 1));
			glVertex3d((GLdouble) (i + 1), 0.0, (GLdouble) (j + 1));
			glVertex3d((GLdouble) (i + 1), 0.0, (GLdouble) j);
		}
	}
	glEnd();
	glPopMatrix();
}

//可変長引数で文字列を返す
//printfのように引数を渡すとそれを文字列にして返す
//const TCHAR* uPrintfString(const TCHAR* format, ...) {
//	static TCHAR strBuffer_g[1024];
//	va_list args;
//	va_start(args, format);
//
//#if _DEBUG
//	int len = _vsctprintf( format, args );
//	if ( len >= 1024 )
//	_ASSERT(0);
//#endif
//
//	_vstprintf(strBuffer_g, format, args);
//	return strBuffer_g;
//}

//文字列描画
void uDrawString(const char str[], int x0, int y0, const float color[]) {
	u3Dto2D();
	// 画面上にテキスト描画
	glColor4fv(color);
	glRasterPos2f(x0, y0);
	for (; *str; str++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *str);
	u2Dto3D();
}

//文字列描画(ftgl版)
void uDrawString2(const char str[], int x0, int y0, const float color[]) {
	u3Dto2D();
	// フォントのデータ
	static FTPixmapFont* pFont; //フォント
	unsigned long ulFontSize = 40;  //フォントサイズ
	const static char kFilePath[256] = "font/hui.ttf"; //フォントのパス
	// フォントの初期化
	if (!pFont) {
		pFont = new FTPixmapFont(kFilePath);
		if (pFont->Error()) {
			char es[256];
			sprintf(es, "%sが開けません", kFilePath);
			uErrorOut(__FILE__, __func__, __LINE__, es);
			delete pFont;
			pFont = 0;
		} else {
			pFont->FaceSize(ulFontSize);
		}
	}
	// FTGLで文字列を描画
	if (pFont) {
		glColor4fv(color);
		glRasterPos2f(x0, y0);
		pFont->Render(str);
	}
	u2Dto3D();
}

//範囲外の時trueを返す
bool uOutOfRange(Vector3 place, Vector3 v1, Vector3 v2) {
	//v1が小さくなるように入れ替え
	if (v1.x > v2.x)
		std::swap(v1, v2);

	if (!(place.x >= v1.x && place.x <= v2.x))
		return true;
	if (!(place.y >= v1.y && place.y <= v2.y))
		return true;
	if (!(place.z >= v1.z && place.z <= v2.z))
		return true;
	return false;
}

//球と球の当たり判定
bool uIsCollisionBallAndBall(Vector3 b1, double r1, Vector3 b2, double r2,
		double *distance/*距離を格納するポインタ(NULLでも可*/) {
	double d1 = pow(b2.x - b1.x, 2) + pow(b2.y - b1.y, 2) + pow(b2.z - b1.z, 2);
	double d2 = pow(r1 + r2, 2);
	if (distance != NULL) {
		*distance = sqrt(d1) - (r1 + r2);
	}
	if (d1 <= d2)
		return true;
	return false;
}
