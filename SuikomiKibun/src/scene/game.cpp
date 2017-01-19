#include "game.h"

//コンストラクタ
GameScene::GameScene(ISceneChanger* changer, SceneParam param) :
		BaseScene(changer) {
	//input初期化
	input::Init();
	input::set_is_enabled_mouse_motion(true); //マウス移動料取得を有効にする

	//ネットワーク
	net_main_ = new NetMain();

	//ワールド作成
	{
		//衝突検出方法の選択(デフォルトを選択)
		btDefaultCollisionConfiguration *config = new btDefaultCollisionConfiguration();
		btCollisionDispatcher *dispatcher = new btCollisionDispatcher(config);
		//ブロードフェーズ法の設定(Dynamic AABB tree method)
		btDbvtBroadphase *broadphase = new btDbvtBroadphase();
		//拘束(剛体間リンク)ソルバの設定
		btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
		//Bulletのワールド作成
		dynamics_world_ = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, config);
		dynamics_world_->setGravity(btVector3(0, -9.8, 0));
	}
	//マップ作成
	map_ = new StageMap(dynamics_world_);

	//敵プレイヤー作成
	playerteki1_ = new PlayerTeki(dynamics_world_, btVector3(1, 1, 0));
	playerteki2_ = new PlayerTeki(dynamics_world_, btVector3(10, 10, 2));

	//プレイヤー作成
	player_ = new Player(dynamics_world_);

	//BGM
	bgm_ = new Bgm();
	bgm_->Play(Bgm::kGameBgm);
}

//デストラクタ
GameScene::~GameScene() {
	//ネットワーク
	delete net_main_;
	//プレイヤー破壊
	delete player_;
	delete playerteki1_;
	delete playerteki2_;
	//マップ破壊
	delete map_;
	//ワールド破壊
	delete dynamics_world_->getBroadphase();
	delete dynamics_world_;
	//bgm
	delete bgm_;
}

//更新
void GameScene::Update() {
	//bulletをすすめる
	static int ptime = glutGet(GLUT_ELAPSED_TIME) - 25; //初期フレームは25ミリ秒すすめる(すり抜けにより調節)
	static int ntime;
	ntime = glutGet(GLUT_ELAPSED_TIME);
	dynamics_world_->stepSimulation((ntime - ptime) / 1000.0, 0); //前回フレームから経過した時間分すすめる
	ptime = ntime;

	//カメラ更新
	camera_.Update(player_->get_center_pos());
	camera_.set_distance(player_->get_camera_distance());

	//ネットワーク
	net_main_->Update();
	net_main_->SetMyPos(player_->get_center_pos());
	net_main_->SetMyLevel(player_->get_level());
	net_main_->SetMyColor(player_->get_color());

	//マップ更新
	map_->Update();

	//プレイヤー更新
	if (net_main_->GetGameState() == kPlay)
		player_->Update(camera_.get_angle_w() + M_PI, map_);

	//敵プレイヤー更新
	playerteki1_->Update(net_main_->GetEnemyPos(0), net_main_->GetEnemyLevel(0), net_main_->GetColor(0));
	playerteki2_->Update(net_main_->GetEnemyPos(1), net_main_->GetEnemyLevel(1), net_main_->GetColor(1));

	//ライト
	GLfloat kLight0Pos[4] = { 0.0, 100.0, 0.0, 1.0 }; //ライト位置
	glLightfv(GL_LIGHT0, GL_POSITION, kLight0Pos);

	//BGM
	Sound::SetListener(camera_);
	bgm_->Update();
}

//描画
void GameScene::Draw() const {
	//ネットワーク
	net_main_->Draw();
	//マップ描画
	map_->Draw();
	//プレイヤー描画
	player_->Draw();
	playerteki1_->Draw();
	playerteki2_->Draw();
	//制限時間描画
	output_display0.Regist("残り時間:" + uToStr(net_main_->GetLimitedTime()), uColor4fv_orange, 1);
	//名前(テスト描画)
	output_display0.Regist("自分の名前:" + net_main_->GetMyName(), uColor4fv_green, 1);
	output_display0.Regist("クライアント1:" + net_main_->GetEnemyName(0), uColor4fv_green, 1);
	output_display0.Regist("クライアント2:" + net_main_->GetEnemyName(1), uColor4fv_green, 1);
}
