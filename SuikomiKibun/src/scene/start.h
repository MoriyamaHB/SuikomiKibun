#ifndef SUIKOMIKIBUN_SCENE_START_H_
#define SUIKOMIKIBUN_SCENE_START_H_

#include "scene_mgr.h"
#include "../input/input.h"
#include "../network/client.h"
#include "../network/server.h"
#include "../util/output_display.h"

class StartScene: public BaseScene {
private:
	ServerTcp *server_;
	ClientTcp *client_;
	ClientTcp *client1_;
	ClientTcp *client2_;
public:
	StartScene(ISceneChanger* changer, SceneParam param);
	~StartScene();
	void Update();
	void Draw() const;
};

#endif /* SUIKOMIKIBUN_SCENE_START_H_ */
