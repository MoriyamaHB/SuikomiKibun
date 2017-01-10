#ifndef SUIKOMIKIBUN_SCENE_SERVERONLY_H_
#define SUIKOMIKIBUN_SCENE_SERVERONLY_H_

#include "scene_mgr.h"
#include "../define.h"
#include "../gv.h"
#include "../network/net_main.h"

class ServerOnlyScene: public BaseScene {
private:
	NetMain *net_main_;

public:
	ServerOnlyScene(ISceneChanger* changer, SceneParam param);
	~ServerOnlyScene();
	void Update();
	void Draw() const;
};

#endif /* SUIKOMIKIBUN_SCENE_SERVERONLY_H_ */
