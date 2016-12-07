#include "start.h"

//コンストラクタ
StartScene::StartScene(ISceneChanger* changer, SceneParam param) :
		BaseScene(changer) {
	server_ = new Server(31600, 3);
	client_ = new ClientTcp("127.0.0.1", 31600);
	client1_ = new ClientTcp("127.0.0.1", 31601);
	client2_ = new ClientTcp("127.0.0.1", 31602);
}

//デストラクタ
StartScene::~StartScene() {
	delete client_;
	delete client1_;
	delete client2_;
	delete server_;
}

//更新
void StartScene::Update() {
	server_->Update();
	client_->Update();
	client1_->Update();
	client2_->Update();
	if (input::get_keyboard_frame(13) == 1) {
		scene_changer_->ChangeScene(kSceneSelect);
	}
}

//描画
void StartScene::Draw() const {
	server_->Draw();
	client_->Draw();
	client1_->Draw();
	client2_->Draw();
}
