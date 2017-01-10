#ifndef SUIKOMIKIBUN_SCENE_START_H_
#define SUIKOMIKIBUN_SCENE_START_H_

#include <FTGL/ftgl.h>

#include "scene_mgr.h"
#include "../input/input.h"
#include "../network/client.h"
#include "../network/server.h"
#include "../util/output_display.h"
#include "../util/cc_util.h"
#include "../util/uGL.h"
#include "../camera/camera.h"

class StartScene: public BaseScene {
private:
	Camera3D3P camera_;

	int start_rand_solid;
	float start_rand_mate[4];

	FTPixmapFont title_font; //タイトルフォント
	FTPixmapFont description_font; //ゲーム説明フォント

	//定数
	GLfloat kLight0Pos[4];	//ライト位置
	const unsigned long kTitleFontSize;  //タイトルフォントサイズ
	const unsigned long kDescriptionFontSize;  //説明フォントサイズ
public:
	StartScene(ISceneChanger* changer, SceneParam param);
	~StartScene();
	void Update();
	void Draw() const;
};

#endif /* SUIKOMIKIBUN_SCENE_START_H_ */
