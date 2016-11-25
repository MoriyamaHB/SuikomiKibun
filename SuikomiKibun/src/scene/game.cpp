#include "game.h"

//コンストラクタ
GameScene::GameScene(ISceneChanger* changer, SceneParam param) :
		BaseScene(changer) {
	//input初期化
	input::Init();
	input::set_is_enabled_mouse_motion(true); //マウス移動料取得を有効にする

	//ワールド作成
	{
		//衝突検出方法の選択(デフォルトを選択)
		btDefaultCollisionConfiguration *config =
				new btDefaultCollisionConfiguration();
		btCollisionDispatcher *dispatcher = new btCollisionDispatcher(config);
		//ブロードフェーズ法の設定(Dynamic AABB tree method)
		btDbvtBroadphase *broadphase = new btDbvtBroadphase();
		//拘束(剛体間リンク)ソルバの設定
		btSequentialImpulseConstraintSolver* solver =
				new btSequentialImpulseConstraintSolver();
		//Bulletのワールド作成
		dynamics_world_ = new btDiscreteDynamicsWorld(dispatcher, broadphase,
				solver, config);
		dynamics_world_->setGravity(btVector3(0, -9.8, 0));
	}
	//マップ作成
	map_ = new StageMap(dynamics_world_);
	//プレイヤー作成
	player_ = new Player(dynamics_world_);
	//敵プレイヤー作成
	playerteki1_ = new PlayerTeki(dynamics_world_,btVector3(1,1,0));
	playerteki2_ = new PlayerTeki(dynamics_world_,btVector3(10,10,2));
}

//デストラクタ
GameScene::~GameScene() {
	//マップ破壊
	delete map_;
	//プレイヤー破壊
	delete player_;
	//ワールド破壊
	delete dynamics_world_->getBroadphase();
	delete dynamics_world_;
}

//更新
void GameScene::Update() {
	//bulletをすすめる
	dynamics_world_->stepSimulation(1.0 / kFps,0);

	//カメラ更新
	Vector3 player_pos = player_->get_center_pos();
	camera_.Update(player_pos.x, player_pos.y, player_pos.z);
	camera_.set_distance(player_->get_camera_distance());

	//マップ更新
	map_->Update();

	//プレイヤー更新
	player_->Update(camera_.get_angle_w() + M_PI, map_);
	//敵プレイヤー更新
	playerteki1_->Update(btVector3(1,5,0));
	playerteki2_->Update(btVector3(20,5,0));

	//ライト

	GLfloat kLight0Pos[4] = { 0.0, 100.0, 0.0, 1.0 }; //ライト位置

	glLightfv(GL_LIGHT0, GL_POSITION, kLight0Pos);
}

//描画
void GameScene::Draw() const {
	//マップ描画
	map_->Draw();
	//プレイヤー描画
	player_->Draw();
}
