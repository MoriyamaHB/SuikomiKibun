#ifndef SUIKOMIKIBUN_SCENE_SELECT_H_
#define SUIKOMIKIBUN_SCENE_SELECT_H_

#include "scene_mgr.h"
#include "../input/input.h"

class SelectScene: public BaseScene {
public:
	SelectScene(ISceneChanger* changer);
	~SelectScene();
	void Update();
	void Draw() const;
};

#endif /* SUIKOMIKIBUN_SCENE_SELECT_H_ */
