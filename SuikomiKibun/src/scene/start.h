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
#include "../camera/camera.h"

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
	FTPixmapFont title_font; //タイトルフォント
	FTPixmapFont description_font; //ゲーム説明フォント

	//定数
	GLfloat kLight0Pos[4];	//ライト位置
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
		kSphere = 0, kCube, kCylinder, kBodyTypeNum
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
