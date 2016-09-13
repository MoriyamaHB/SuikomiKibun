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
}

//デストラクタ
GameScene::~GameScene() {
	//マップ破壊
	delete map_;
	//ワールド破壊
	delete dynamics_world_->getBroadphase();
	delete dynamics_world_;
}

//更新
void GameScene::Update() {
	//bulletをすすめる
	dynamics_world_->stepSimulation(1.0 / kFps);

	//マップ更新
	map_->Update();

	//カメラ更新
	camera_.Update(0, 0, 0);

	//ライト
	GLfloat kLight0Pos[4] = { 0.0, 15.0, 0.0, 1.0 }; //ライト位置
	glLightfv(GL_LIGHT0, GL_POSITION, kLight0Pos);
}

//描画
void GameScene::Draw() const {
	map_->Draw();
}
