#ifndef SUIKOMIKIBUN_SCENE_START_H_
#define SUIKOMIKIBUN_SCENE_START_H_

#include "scene_mgr.h"
#include "../input/input.h"

class StartScene: public BaseScene {
public:
	StartScene(ISceneChanger* changer);
	~StartScene();
	void Update();
	void Draw() const;
};

#endif /* SUIKOMIKIBUN_SCENE_START_H_ */
