#include "start.h"

//コンストラクタ
StartScene::StartScene(ISceneChanger* changer, SceneParam param) :
		BaseScene(changer) {
}

//デストラクタ
StartScene::~StartScene() {
}

//更新
void StartScene::Update() {
	if (input::get_keyboard_frame(13) == 1) {
		scene_changer_->ChangeScene(kSceneSelect);
	}
}

//描画
void StartScene::Draw() const {
}
