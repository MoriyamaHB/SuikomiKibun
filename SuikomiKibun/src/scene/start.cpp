#include "start.h"

//コンストラクタ
StartScene::StartScene(ISceneChanger* changer, SceneParam param) :
		BaseScene(changer), title_font("font/crayon.ttf"), description_font("font/jkgm.ttf"), kTitleFontSize(150), kDescriptionFontSize(
				40) {
	kLight0Pos[0] = 0.0, kLight0Pos[1] = 15.0, kLight0Pos[2] = 0.0, kLight0Pos[3] = 1.0;

	//input初期化
	input::Init();

	//カメラ位置
	camera_.Update(0, 200);
	//描画図形乱数
	start_rand_solid = cc_util::GetRandom(0, 5);
	start_rand_mate[0] = cc_util::GetRandom(0, 1000) / 1000.0;
	start_rand_mate[1] = cc_util::GetRandom(0, 1000) / 1000.0;
	start_rand_mate[2] = cc_util::GetRandom(0, 1000) / 1000.0;
	start_rand_mate[3] = cc_util::GetRandom(0, 1000) / 1000.0;
	// フォントの初期化
	if (title_font.Error() || description_font.Error()) {
		uErrorOut(__FILE__, __func__, __LINE__, "タイトルフォントが開けません");
	} else {
		title_font.FaceSize(kTitleFontSize);
		description_font.FaceSize(kDescriptionFontSize);
	}
}

//デストラクタ
StartScene::~StartScene() {

}

//更新
void StartScene::Update() {
	//カメラ
	camera_.Update(3, 0); //カメラ更新

	//ライト
	glLightfv(GL_LIGHT0, GL_POSITION, kLight0Pos);

	//↓Renderがconstでは使えないためここに記述
	//タイトル描画
	u3Dto2D();
	if (!title_font.Error()) {
		glColor4fv (uColor4fv_purple);
		glRasterPos2f(220, 180);
		title_font.Render("吸い込み気分");
	}
	//ゲーム説明描画
	if (!description_font.Error()) {
		glColor4fv (uColor4fv_gray);
		glRasterPos2f(200, 260);
		description_font.Render("このゲームは物体を吸い込んで大きくしていくゲームです。");
	}
	u2Dto3D();

	return;
}

//描画
void StartScene::Draw() const {
	//地面描画
	uDrawGround(20);

	//ランダムで図形描画
	glPushMatrix();
	glScalef(0.5, 0.5, 0.5);
	glTranslated(0, 0.5, 0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, start_rand_mate);
	switch (start_rand_solid) {
	case 0:
		glutSolidDodecahedron();
		break;
	case 1:
		glutSolidTeapot(1);
		break;
	case 2:
		glutSolidCone(1, 1, 24, 24);
		break;
	case 3:
		glutSolidIcosahedron();
		break;
	case 4:
		glutSolidRhombicDodecahedron();
		break;
	case 5:
		glutSolidTetrahedron();
		break;
	default:
		uErrorOut(__FILE__, __func__, __LINE__, "表示する図形の指定ミス");
		break;
	}
	glPopMatrix();

	//文字描画
	uDrawString2("Aキーを押すとゲーム開始です", 940, 760, uColor4fv_red);
}
