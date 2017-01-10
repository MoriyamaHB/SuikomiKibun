#include "start.h"

//コンストラクタ
StartScene::StartScene(ISceneChanger* changer, SceneParam param) :
		BaseScene(changer), title_font("font/crayon.ttf"), description_font("font/jkgm.ttf"), kTitleFontSize(150), kDescriptionFontSize(
				40) {
	kLight0Pos[0] = 0.0, kLight0Pos[1] = 15.0, kLight0Pos[2] = 0.0, kLight0Pos[3] = 1.0;

	//input初期化
	input::Init();

	//カメラ位置
	camera_.Update(0, 200);

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

	//地面
	{
		btVector3 ground_pos = btVector3(0, 0, 0);
		btVector3 ground_extents = btVector3(25, 0.00001, 25);
		btScalar ground_mass = 0.0;
		btScalar ground_rest = 1.0;
		btVector3 ground_inertia(0, 0, 0);
		btCollisionShape *ground_shape = new btBoxShape(ground_extents);
		btQuaternion qrot(0, 0, 0, 1);
		btDefaultMotionState* ground_motion_state = new btDefaultMotionState(btTransform(qrot, ground_pos));
		ground_shape->calculateLocalInertia(ground_mass, ground_inertia);
		ground_body_ = new btRigidBody(ground_mass, ground_motion_state, ground_shape, ground_inertia);
		ground_body_->setRestitution(ground_rest);
		dynamics_world_->addRigidBody(ground_body_);
	}

	//描画図形乱数
//	start_rand_solid = cc_util::GetRandom(0, 5);
//	start_rand_mate[0] = cc_util::GetRandom(0, 1000) / 1000.0;
//	start_rand_mate[1] = cc_util::GetRandom(0, 1000) / 1000.0;
//	start_rand_mate[2] = cc_util::GetRandom(0, 1000) / 1000.0;
//	start_rand_mate[3] = cc_util::GetRandom(0, 1000) / 1000.0;

// フォントの初期化
	if (title_font.Error() || description_font.Error()) {
		uErrorOut(__FILE__, __func__, __LINE__, "タイトルフォントが開けません");
	} else {
		title_font.FaceSize(kTitleFontSize);
		description_font.FaceSize(kDescriptionFontSize);
	}
}

//デストラクタ
StartScene::~StartScene() {
	//オブジェクト破壊
	delete ground_body_->getMotionState();
	dynamics_world_->removeRigidBody(ground_body_);
	delete ground_body_;

	//オブジェクト
	for (auto itr = bodys_.begin(); itr != bodys_.end();) {
		delete (*itr);
		itr = bodys_.erase(itr);
	}

	//ワールド破壊
	delete dynamics_world_->getBroadphase();
	delete dynamics_world_;
}

//更新
void StartScene::Update() {
	//bulletをすすめる
	dynamics_world_->stepSimulation(1.0 / kFps);

	//カメラ
	camera_.Update(3, 0); //カメラ更新

	//ライト
	glLightfv(GL_LIGHT0, GL_POSITION, kLight0Pos);

	//オブジェクト追加
	bodys_.push_back(new StartBodys(dynamics_world_));

	//↓Renderがconstでは使えないためここに記述
	//タイトル描画
	u3Dto2D();
	if (!title_font.Error()) {
		glColor4fv (uColor4fv_purple);
		glRasterPos2f(220, 180);
		title_font.Render("吸い込み気分");
	}
	//ゲーム説明描画
	if (!description_font.Error()) {
		glColor4fv (uColor4fv_gray);
		glRasterPos2f(200, 260);
		description_font.Render("このゲームは物体を吸い込んで大きくしていくゲームです。");
	}
	u2Dto3D();

	return;
}

//描画
void StartScene::Draw() const {
	//地面
	btVector3 pos;
	glPushMatrix();
	pos = ground_body_->getCenterOfMassPosition();
	glTranslatef(pos[0], pos[1], pos[2]);
	const btBoxShape* ground_shape = static_cast<const btBoxShape*>(ground_body_->getCollisionShape());
	btVector3 ground_half_extent = ground_shape->getHalfExtentsWithMargin();
	uDrawGround(ground_half_extent[0] * 2);
	glPopMatrix();

	//オブジェクト
	for (auto itr = bodys_.begin(); itr != bodys_.end(); ++itr) {
		(*itr)->Draw();
	}

	//文字描画
	uDrawString2("Aキーを押すとゲーム開始です", 940, 760, uColor4fv_red);
}

////////////////////////////   start_body   //////////////////////////////////////////////////////

StartBodys::StartBodys(btDynamicsWorld *world) :
		world_(world) {
	//乱数で材質,大きさ,タイプ設定
	material_[0] = cc_util::GetRandom(0, 1000) / 1000.0;
	material_[1] = cc_util::GetRandom(0, 1000) / 1000.0;
	material_[2] = cc_util::GetRandom(0, 1000) / 1000.0;
	material_[3] = cc_util::GetRandom(0, 1000) / 1000.0;
	btScalar radius = cc_util::GetRandom(0, 1000) / 1000.0;
	btVector3 extents = btVector3(cc_util::GetRandom(0, 1000) / 1000.0, cc_util::GetRandom(0, 1000) / 1000.0,
			cc_util::GetRandom(0, 1000) / 1000.0);
	type_ = static_cast<StartBodys::BodyType>(cc_util::GetRandom(0, kBodyTypeNum - 1));

	//他ステータスを設定
	btVector3 pos = btVector3(0, 5, 0);	//中心座標
	btScalar mass = 0.03;	//質量
	btScalar rest = 0.8;	//反発係数
	btVector3 inertia(0, 0, 0);	//慣性モーメント
	btQuaternion qrot(0, 0, 0, 1);	//姿勢

	btDefaultMotionState* motion_state = new btDefaultMotionState(btTransform(qrot, pos));	//姿勢,位置設定

	//オブジェクト生成
	btCollisionShape *shape;
	switch (type_) {
	case kSphere:
		shape = new btSphereShape(radius);
		break;
	case kCube:
		shape = new btBoxShape(extents);
		break;
	default:
		uErrorOut(__FILE__, __func__, __LINE__, "不明なタイプです.球を作成します.");
		shape = new btSphereShape(radius);
		type_ = kSphere;
		break;
	}

	shape->calculateLocalInertia(mass, inertia);	//慣性モーメントの計算
	body_ = new btRigidBody(mass, motion_state, shape, inertia);	//剛体オブジェクト生成
	body_->setRestitution(rest);	//反発係数設定
	world_->addRigidBody(body_);	//ワールドに剛体オブジェクトを追加
}

StartBodys::~StartBodys() {
	//オブジェクト破壊
	delete body_->getMotionState();
	world_->removeRigidBody(body_);
	delete body_;
}

//描画
void StartBodys::Draw() {
	glPushMatrix();
	switch (type_) {
	case kSphere: {
		btVector3 pos = body_->getCenterOfMassPosition();
		glTranslatef(pos[0], pos[1], pos[2]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_);
		glutSolidSphere(static_cast<btSphereShape*>(body_->getCollisionShape())->getRadius(), 20, 20);
		break;
	}
	case kCube: {
		GLfloat cube_m[16];
		btDefaultMotionState *cube_motion = static_cast<btDefaultMotionState*>(body_->getMotionState());
		cube_motion->m_graphicsWorldTrans.getOpenGLMatrix(cube_m);
		glMultMatrixf(cube_m);
		const btBoxShape* cube_shape = static_cast<const btBoxShape*>(body_->getCollisionShape());
		btVector3 cube_half_extent = cube_shape->getHalfExtentsWithMargin();
		glScaled(2 * cube_half_extent[0], 2 * cube_half_extent[1], 2 * cube_half_extent[2]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_);
		glutSolidCube(1.0);
		break;
	}
	default:
		uErrorOut(__FILE__, __func__, __LINE__, "不明なタイプです.");
		break;
	}
	glPopMatrix();
}
