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
	//入力終了フラグセット
	is_finish_input = false;
	//入力開始
	thread_ = new boost::thread(&InputIniInfo::ThreadInput, this);
}

//デストラクタ
InputIniInfo::~InputIniInfo() {
	//スレッド終了まで待機←これやると終わらないので強制終了
//	if (thread_->joinable())
//		thread_->join();
	//スレッド削除
	delete thread_;
}

//更新
void InputIniInfo::Update() {
	//入力終了で次のシーンに進む
	if (is_finish_input) {
		SceneParam par;
		par.input_ini_info_data = data;
		scene_changer_->ChangeScene(kSceneGame, par);
	}

	//案内表示
	u3Dto2D();
	if (!font_.Error()) {
		glColor4fv (uColor4fv_orange);
		glRasterPos2f(10, 200);
		font_.Render("端末で情報を入力してください");
	}
	u2Dto3D();
}

//描画
void InputIniInfo::Draw() {
}

//入力スレッド
void InputIniInfo::ThreadInput() {
	std::cout << "サーバーなら's',クライアントなら'c'を入力してください:";
	std::cin >> data.s_or_c;
	if (data.s_or_c == 'c') {
		std::cout << "サーバーのPC名:";
		std::cin >> data.server_ip;
		std::cout << "ポート番号(1人目:31601,2人目:31602):";
		std::cin >> data.port;
	} else if (data.s_or_c == 's') {
		strcpy(data.server_ip, "localhost");
		data.port = 31600;
	}
	std::cout << "あなたの名前:";
	std::cin >> data.client_name;
	is_finish_input = true;	//入力終了
}
