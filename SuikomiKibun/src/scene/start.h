#ifndef SUIKOMIKIBUN_SCENE_START_H_
#define SUIKOMIKIBUN_SCENE_START_H_

#include "scene_mgr.h"
#include "../input/input.h"
#include "../util/output_display.h"
#include "../network/network.h"
#include "../net/server.h"

class StartScene: public BaseScene {
private:
	Server *server;
	Client *client;
	Client *client1;
	Client *client2;
public:
	StartScene(ISceneChanger* changer, SceneParam param);
	~StartScene();
	void Update();
	void Draw() const;
};

#endif /* SUIKOMIKIBUN_SCENE_START_H_ */
