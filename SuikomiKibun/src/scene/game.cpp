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
}

//更新
void GameScene::Update() {
	//bulletをすすめる
	dynamics_world_->stepSimulation(1.0 / kFps, 0);

	//カメラ更新
	camera_.Update(player_->get_center_pos());
	camera_.set_distance(player_->get_camera_distance());

	//ネットワーク
	net_main_->Update();
	net_main_->SetMePos(player_->get_center_pos());
	net_main_->SetMeLevel(player_->get_level());
	net_main_->SetMeColor(player_->get_color());

	//マップ更新
	map_->Update();

	//プレイヤー更新
	player_->Update(camera_.get_angle_w() + M_PI, map_,net_main_->GetColor(0),net_main_->GetColor(1),net_main_->GetEnemyLevel(0),net_main_->GetEnemyLevel(1));

	//敵プレイヤー更新
	playerteki1_->Update(net_main_->GetEnemyPos(0), net_main_->GetEnemyLevel(0), net_main_->GetColor(0),map_);
	playerteki2_->Update(net_main_->GetEnemyPos(1), net_main_->GetEnemyLevel(1), net_main_->GetColor(1),map_);

	//ライト
	GLfloat kLight0Pos[4] = { 0.0, 100.0, 0.0, 1.0 }; //ライト位置
	glLightfv(GL_LIGHT0, GL_POSITION, kLight0Pos);
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
}
