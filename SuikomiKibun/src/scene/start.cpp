#include "start.h"

//コンストラクタ
StartScene::StartScene(ISceneChanger* changer, SceneParam param) :
		BaseScene(changer) {
	server = new Server();
	client = new Client("127.0.0.1", 31470);
	client1 = new Client("127.0.0.1", 31471);
	client2 = new Client("127.0.0.1", 31472);
}

//デストラクタ
StartScene::~StartScene() {
	delete client;
	delete client1;
	delete client2;
	delete server;
}

//更新
void StartScene::Update() {
	server->Update();
	client->Update();
	client1->Update();
	client2->Update();
	if (input::get_keyboard_frame(13) == 1) {
		scene_changer_->ChangeScene(kSceneSelect);
	}
}

//描画
void StartScene::Draw() const {
	server->Draw();
	client->Draw();
	client1->Draw();
	client2->Draw();
}
