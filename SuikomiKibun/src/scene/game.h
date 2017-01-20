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
#include "../sound/bgm.h"
#include "../ranking/ranking.h"
#include "../util/button.h"

class GameScene: public BaseScene {
private:
	btDynamicsWorld* dynamics_world_;
	Camera3D3P camera_;
	StageMap* map_;
	Player *player_;
	PlayerTeki *playerteki1_;
	PlayerTeki *playerteki2_;
	NetMain *net_main_;
	Bgm *bgm_;
	Ranking ranking_;
	Button *button_;
	FTPixmapFont nav_font_, nav_font_1;
public:
	GameScene(ISceneChanger* changer, SceneParam param);
	~GameScene();
	void Update();
	void Draw();
};

#endif /* SUIKOMIKIBUN_SCENE_GAME_H_ */
