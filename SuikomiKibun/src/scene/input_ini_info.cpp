#include "input_ini_info.h"

//コンストラクタ
InputIniInfo::InputIniInfo(ISceneChanger* changer, SceneParam param) :
		BaseScene(changer) {
}

//デストラクタ
InputIniInfo::~InputIniInfo() {
}

//更新
void InputIniInfo::Update() {
	//フレームカウント
	static int frame_num = 0;
	frame_num++;

	switch (frame_num) {
	case 1: //1フレーム目
		output_display0.Regist("端末で情報を入力してください", uColor4fv_orange, 1);
		break;
	case 2:  //2フレーム目
		std::cout << "サーバーなら's',クライアントなら'c'を入力してください:";
		std::cin >> data.s_or_c;
		if (data.s_or_c == 'c') {
			std::cout << "サーバーのPC名:";
			std::cin >> data.server_ip;
			std::cout << "ポート番号(1人目:31601,2人目:31602):";
			std::cin >> data.port;
			std::cout << "あなたの名前:";
			std::cin >> data.client_name;
		}
		SceneParam par;
		par.input_ini_info_data = data;
		scene_changer_->ChangeScene(kSceneGame, par);
		break;
	}
}

//描画
void InputIniInfo::Draw() const {
}
