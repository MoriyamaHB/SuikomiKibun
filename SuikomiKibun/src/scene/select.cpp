#include "select.h"

//コンストラクタ
SelectScene::SelectScene(ISceneChanger* changer) :
		BaseScene(changer) {
}

//デストラクタ
SelectScene::~SelectScene() {
}

//更新
void SelectScene::Update() {
	if (input::get_keyboard_frame(13) == 1)
		scene_changer_->ChangeScene(kSceneStart);
}

//描画
void SelectScene::Draw() const {
}
