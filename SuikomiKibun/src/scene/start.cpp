#include "start.h"

//コンストラクタ
StartScene::StartScene(ISceneChanger* changer, SceneParam param) :
		BaseScene(changer), title_font_("font/crayon.ttf"), description_font_("font/jkgm.ttf"), kTitleFontSize(150), kDescriptionFontSize(
				40) {
	kLight0Pos_[0] = 0.0, kLight0Pos_[1] = 15.0, kLight0Pos_[2] = 0.0, kLight0Pos_[3] = 1.0;

	//opengl設定
	glEnable (GL_LIGHTING);
	glDisable (GL_TEXTURE_2D);
	glDisable (GL_COLOR_MATERIAL);

	//input初期化
	input::Init();

	//カメラ位置
	camera_.set_distance(5.0);
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

	// フォントの初期化
	if (title_font_.Error() || description_font_.Error()) {
		uErrorOut(__FILE__, __func__, __LINE__, "タイトルフォントが開けません");
	} else {
		title_font_.FaceSize(kTitleFontSize);
		description_font_.FaceSize(kDescriptionFontSize);
	}

	//サウンド
	bgm_ = new Bgm();
	bgm_->Play(Bgm::kStartBgm, 3.0);

	//ボタン
	button1_ = new Button(500, 600, 770, 680, "ゲームスタート", "font/jkgm.ttf", 40);
	button1_->set_text_color(uColor4fv_red);
	button1_->set_text_active_color(uColor4fv_red);
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

	//サウンド
	delete bgm_;

	//ボタン
	delete button1_;
}

//更新
void StartScene::Update() {
	//bulletをすすめる
	static int ptime = glutGet(GLUT_ELAPSED_TIME);
	static int ntime;
	ntime = glutGet(GLUT_ELAPSED_TIME);
	dynamics_world_->stepSimulation((ntime - ptime) / 1000.0, 0); //前回フレームから経過した時間分すすめる
	ptime = ntime;

	//カメラ
	camera_.Update(3, 0); //カメラ更新

	//ライト
	glLightfv(GL_LIGHT0, GL_POSITION, kLight0Pos_);

	//オブジェクト追加
	static int cnt = 0;
	cnt++;
	if (cnt % 60 == 0)
		bodys_.push_back(new StartBodys(dynamics_world_));

	//サウンド
	Sound::SetListener (camera_);
	bgm_->Update();

	//ボタンによるシーン遷移
	if (button1_->Update())
		scene_changer_->ChangeScene(kSceneInputIniInfo);

	return;
}

//描画
void StartScene::Draw() {
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

	//タイトル描画
	u3Dto2D();
	if (!title_font_.Error()) {
		glColor4fv (uColor4fv_purple);
		glRasterPos2f(220, 180);
		title_font_.Render("吸い込み気分");
	}
	//ゲーム説明描画
	if (!description_font_.Error()) {
		glColor4fv (uColor4fv_gray);
		glRasterPos2f(200, 260);
		description_font_.Render("このゲームは物体を吸い込んで大きくしていくゲームです。");
	}
	u2Dto3D();

	//ボタン描画
	button1_->Draw();
}

////////////////////////////   start_body   //////////////////////////////////////////////////////

StartBodys::StartBodys(btDynamicsWorld *world) :
		world_(world) {
	//乱数で大きさ,タイプ設定
	btScalar radius = cc_util::GetRandom(1, 1000) / 1000.0;
	btVector3 extents = btVector3(cc_util::GetRandom(1, 1000) / 1000.0, cc_util::GetRandom(1, 1000) / 1000.0,
			cc_util::GetRandom(1, 1000) / 1000.0);
	type_ = static_cast<StartBodys::BodyType>(cc_util::GetRandom(0, kBodyTypeNum - 1));

	//他ステータスを設定
	btVector3 pos = btVector3(0, 8, 0);	//中心座標
	btScalar mass = 0.03;	//質量
	btScalar rest = 0.2;	//反発係数
	btVector3 inertia(0, 0, 0);	//慣性モーメント
	btQuaternion qrot(0, 0, 0, 1);	//姿勢
	btScalar ccd_radius;	//すり抜け時に衝突判定する球の半径

	btDefaultMotionState* motion_state = new btDefaultMotionState(btTransform(qrot, pos));	//姿勢,位置設定

	//オブジェクト生成,材質設定
	btCollisionShape *shape;
	switch (type_) {
	case kSphere:
		memcpy(material_, uMaterial4fv_brown, sizeof(material_));
		shape = new btSphereShape(radius);
		ccd_radius = radius;
		break;
	case kCube:
		memcpy(material_, uMaterial4fv_blue, sizeof(material_));
		shape = new btBoxShape(extents);
		ccd_radius = sqrt(pow(uGetMaxOfBtVector3(extents), 2.0) * 2.0);
		break;
	case kCylinder:
		memcpy(material_, uMaterial4fv_red, sizeof(material_));
		shape = new btCylinderShapeZ(extents);
		ccd_radius = sqrt(pow(uGetMaxOfBtVector3(extents), 2.0) * 2.0);
		break;
	case kCapsule:
		memcpy(material_, uMaterial4fv_green, sizeof(material_));
		shape = new btCapsuleShapeZ(radius, extents[1]);
		ccd_radius = radius > extents[1] ? radius : extents[1];
		break;
	case kCone:
		memcpy(material_, uMaterial4fv_white, sizeof(material_));
		shape = new btConeShapeZ(radius, extents[1] * 2.0);
		ccd_radius = radius > extents[1] * 2.0 ? radius : extents[1] * 2.0;
		break;
	default:
		uErrorOut(__FILE__, __func__, __LINE__, "不明なタイプです.球を作成します.");
		shape = new btSphereShape(radius);
		memcpy(material_, uMaterial4fv_black, sizeof(material_));
		type_ = kSphere;
		break;
	}

	shape->calculateLocalInertia(mass, inertia);	//慣性モーメントの計算
	body_ = new btRigidBody(mass, motion_state, shape, inertia);	//剛体オブジェクト生成
	body_->setRestitution(rest);	//反発係数設定
	body_->setCcdSweptSphereRadius(ccd_radius);
	body_->setCcdMotionThreshold(ccd_radius);
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

	//モーションステータス
	GLfloat m[16];
	btDefaultMotionState *motion = static_cast<btDefaultMotionState*>(body_->getMotionState());
	motion->m_graphicsWorldTrans.getOpenGLMatrix(m);
	glMultMatrixf(m);

	//各オブジェクト描画
	switch (type_) {
	case kSphere: {
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_);
		glutSolidSphere(static_cast<btSphereShape*>(body_->getCollisionShape())->getRadius(), 20, 20);
		break;
	}
	case kCube: {
		const btBoxShape* shape = static_cast<const btBoxShape*>(body_->getCollisionShape());
		btVector3 half_extent = shape->getHalfExtentsWithMargin();
		glScaled(2 * half_extent[0], 2 * half_extent[1], 2 * half_extent[2]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_);
		glutSolidCube(1.0);
		break;
	}
	case kCylinder: {
		const btCylinderShapeZ* shape = static_cast<const btCylinderShapeZ*>(body_->getCollisionShape());
		btScalar rad = shape->getRadius();
		btScalar len = shape->getHalfExtentsWithMargin()[shape->getUpAxis()];
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_);
		glutSolidCylinder(rad, len, 20, 20);
		break;
	}
	case kCapsule: {
		const btCapsuleShapeZ* shape = static_cast<const btCapsuleShapeZ*>(body_->getCollisionShape());
		btScalar rad = shape->getRadius();
		btScalar len = shape->getHalfHeight() * 2;
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_);
		DrawCapsule(rad, len, 20, 20);
		break;
	}
	case kCone: {
		const btConeShapeZ* shape = static_cast<const btConeShapeZ*>(body_->getCollisionShape());
		btScalar rad = shape->getRadius();
		btScalar len = shape->getHeight();
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_);
		glTranslated(0.0, 0.0, -len / 2.0);	//opengl標準のままだとずれるので調節
		glutSolidCone(rad, len, 20, 20);
		break;
	}
	default:
		uErrorOut(__FILE__, __func__, __LINE__, "不明なタイプです.");
		break;
	}

	glPopMatrix();
}
