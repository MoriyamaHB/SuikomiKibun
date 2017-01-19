#include "input_ini_info.h"

//コンストラクタ
InputIniInfo::InputIniInfo(ISceneChanger* changer, SceneParam param) :
		BaseScene(changer), font_("font/jkgm.ttf") {
	// フォントの初期化
	if (font_.Error()) {
		uErrorOut(__FILE__, __func__, __LINE__, "タイトルフォントが開けません");
	} else {
		font_.FaceSize(90);
	}
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
		u3Dto2D();
		if (!font_.Error()) {
			glColor4fv(uColor4fv_orange);
			glRasterPos2f(10, 200);
			font_.Render("端末で情報を入力してください");
		}
		u2Dto3D();
		break;
	case 2:  //2フレーム目
		std::cout << "サーバーなら's',クライアントなら'c'を入力してください:";
		std::cin >> data.s_or_c;
		if (data.s_or_c == 'c') {
			std::cout << "サーバーのPC名:";
			std::cin >> data.server_ip;
			std::cout << "ポート番号(1人目:31601,2人目:31602):";
			std::cin >> data.port;
		} else if (data.s_or_c == 's') {
			data.server_ip = "localhost";
			data.port = 31600;
		}
		std::cout << "あなたの名前:";
		std::cin >> data.client_name;
		SceneParam par;
		par.input_ini_info_data = data;
		scene_changer_->ChangeScene(kSceneGame, par);
		break;
	}
}

//描画
void InputIniInfo::Draw() const {
}

//入力スレッド
void InputIniInfo::ThreadInput() {

}
