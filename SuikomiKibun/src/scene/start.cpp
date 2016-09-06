#include "start.h"

//コンストラクタ
StartScene::StartScene(ISceneChanger* changer, SceneParam param) :
		BaseScene(changer) {
	server = new Server();
//	ClientData posdata;
//	asio::io_service io_service;
//	Client client(io_service, "127.0.0.1", &posdata);
//	client.Start();
}

//デストラクタ
StartScene::~StartScene() {
	delete server;
}

//更新
void StartScene::Update() {
	server->Update();
	if (input::get_keyboard_frame(13) == 1) {
		scene_changer_->ChangeScene(kSceneSelect);
	}
}

//描画
void StartScene::Draw() const {
	server->Draw();
}
