#include "server_only.h"

ServerOnlyScene::ServerOnlyScene(ISceneChanger* changer, SceneParam param) :
		BaseScene(changer) {
	//ネットワーク
	net_main_ = new NetMain(true);

}

ServerOnlyScene::~ServerOnlyScene() {
	//ネットワーク
	delete net_main_;
}

void ServerOnlyScene::Update() {
	//ネットワーク
	net_main_->Update();
}

void ServerOnlyScene::Draw() const {
	//ネットワーク
	net_main_->Draw();
}
