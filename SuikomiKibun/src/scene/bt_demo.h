#ifndef SUIKOMIKIBUN_SCENE_BTDEMO_H_
#define SUIKOMIKIBUN_SCENE_BTDEMO_H_

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <GL/freeglut.h>
#include <math.h>

#include "scene_mgr.h"
#include "../input/input.h"
#include "../util/uGL.h"
#include "../camera/camera.h"
#include "../define.h"
#include "../gv.h"
#include "../network/server.h"
#include "../network/client.h"

class BtDemoScene: public BaseScene {
private:
	btDynamicsWorld* dynamics_world_;
	btRigidBody* sphere_body_;
	btRigidBody* sphere_body1_;
	btRigidBody* sphere_body2_;
	btRigidBody* ground_body_;
	btRigidBody* cube_body_;
	Camera3D3P camera;

	//ネットワーク
	Server *server_;
	ClientUdp *client_;
	bool is_server_;
	Vector3 pos1_;
	Vector3 pos2_;
public:
	BtDemoScene(ISceneChanger* changer, SceneParam param);
	~BtDemoScene();
	void Update();
	void Draw() const;
};

#endif /* SUIKOMIKIBUN_SCENE_BTDEMO_H_ */
