#ifndef SUIKOMIKIBUN_SCENE_SELECT_H_
#define SUIKOMIKIBUN_SCENE_SELECT_H_

#include <boost/thread.hpp>

#include "scene_mgr.h"
#include "../input/input.h"
#include "../util/output_display.h"
#include "../gv.h"

class InputIniInfo: public BaseScene {
private:
	InputIniInfoData data;
	FTPixmapFont font_;
	boost::thread *thread_;
	bool is_finish_input;

	void ThreadInput();
public:
	InputIniInfo(ISceneChanger* changer, SceneParam param);
	~InputIniInfo();
	void Update();
	void Draw();
};

#endif /* SUIKOMIKIBUN_SCENE_SELECT_H_ */
