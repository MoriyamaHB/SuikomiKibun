#ifndef SUIKOMIKIBUN_SCENE_BTDEMO_H_
#define SUIKOMIKIBUN_SCENE_BTDEMO_H_

#include "scene_mgr.h"
#include "../input/input.h"

class BtDemoScene: public BaseScene {
public:
	BtDemoScene(ISceneChanger* changer, SceneParam param);
	~BtDemoScene();
	void Update();
	void Draw() const;
};

#endif /* SUIKOMIKIBUN_SCENE_BTDEMO_H_ */
