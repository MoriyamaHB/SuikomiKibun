#ifndef SUIKOMIKIBUN_SCENE_GAME_H_
#define SUIKOMIKIBUN_SCENE_GAME_H_

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
#include "../map/map.h"
#include "../player/player.h"
#include "../player/playerteki.h"
#include "../network/net_main.h"

class GameScene: public BaseScene {
private:
	btDynamicsWorld* dynamics_world_;
	Camera3D3P camera_;
	StageMap* map_;
	Player *player_;
	PlayerTeki *playerteki1_;
	PlayerTeki *playerteki2_;
	NetMain *net_main_;

public:
	GameScene(ISceneChanger* changer, SceneParam param);
	~GameScene();
	void Update();
	void Draw() const;
};

#endif /* SUIKOMIKIBUN_SCENE_GAME_H_ */
