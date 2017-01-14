#ifndef SUIKOMIKIBUN_SCENE_START_H_
#define SUIKOMIKIBUN_SCENE_START_H_

#include <FTGL/ftgl.h>
#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <GL/freeglut.h>
#include <math.h>
#include <vector>
#include <memory>

#include "scene_mgr.h"
#include "../input/input.h"
#include "../network/client.h"
#include "../network/server.h"
#include "../util/output_display.h"
#include "../util/cc_util.h"
#include "../util/uGL.h"
#include "../util/button.h"
#include "../camera/camera.h"
#include "../sound/bgm.h"

class StartBodys;

class StartScene: public BaseScene {
private:
	//カメラ
	Camera3D3P camera_;

	//オブジェクト、地面
	btRigidBody* ground_body_;
	std::vector<StartBodys*> bodys_;

	//bullet
	btDynamicsWorld* dynamics_world_;

	//フォント
	FTPixmapFont title_font_; //タイトルフォント
	FTPixmapFont description_font_; //ゲーム説明フォント

	//サウンド
	Bgm *bgm_;

	//ボタン
	Button *button1;

	//定数
	GLfloat kLight0Pos_[4];	//ライト位置
	const unsigned long kTitleFontSize;  //タイトルフォントサイズ
	const unsigned long kDescriptionFontSize;  //説明フォントサイズ
public:
	StartScene(ISceneChanger* changer, SceneParam param);
	~StartScene();
	void Update();
	void Draw() const;
};

class StartBodys {
public:
	enum BodyType {
		kSphere = 0, kCube, kCylinder, kCapsule, kCone, kBodyTypeNum
	};
private:
	btRigidBody* body_;
	btDynamicsWorld* world_;
	BodyType type_;
	float material_[4];
public:
	StartBodys(btDynamicsWorld *world);
	~StartBodys();
	void Draw();
};

#endif /* SUIKOMIKIBUN_SCENE_START_H_ */
