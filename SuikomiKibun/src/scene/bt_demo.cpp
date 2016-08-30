#include "bt_demo.h"

//コンストラクタ
BtDemoScene::BtDemoScene(ISceneChanger* changer, SceneParam param) :
		BaseScene(changer) {

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

	//中心座標
	btVector3 sphere_pos = btVector3(-1, 5, 0);
	btVector3 ground_pos = btVector3(0, 0, 0);
	btVector3 cube_pos = btVector3(1, 25, 0);
	//大きさ
	btScalar sphere_radius = 1.0;
	btVector3 ground_extents = btVector3(25, 0.1, 25);
	double a = 1.0, b = 20.0, c = 1.0;
	btVector3 cube_extents = btVector3(a, b, c);
	//質量
	btScalar sphere_mass = 0.03;
	btScalar ground_mass = 0.0;
	btScalar cube_mass = 0.08;
	//反発係数
	btScalar sphere_rest = 0.8;
	btScalar ground_rest = 1.0;
	btScalar cube_rest = 0.5;
	//慣性モーメント
	btVector3 sphere_inertia(0, 0, 0);
	btVector3 ground_inertia(0, 0, 0);
	btVector3 cube_inertia(cube_mass * (b * b + c * c) / 3.0, cube_mass * (a * a + c * c) / 3.0,
			cube_mass * (b * b + a * a) / 3.0);

	//形状を設定
	btCollisionShape *sphere_shape = new btSphereShape(sphere_radius);
	btCollisionShape *ground_shape = new btBoxShape(ground_extents);
	btCollisionShape *cube_shape = new btBoxShape(cube_extents);
	//球体の初期位置、姿勢
	btQuaternion qrot(0, 0, 0, 1);
	btDefaultMotionState* sphere_motion_state = new btDefaultMotionState(btTransform(qrot, sphere_pos));
	btDefaultMotionState* ground_motion_state = new btDefaultMotionState(btTransform(qrot, ground_pos));
	btDefaultMotionState* cube_motion_state = new btDefaultMotionState(btTransform(qrot, cube_pos));
	//慣性モーメントの計算
	sphere_shape->calculateLocalInertia(sphere_mass, sphere_inertia);
	ground_shape->calculateLocalInertia(ground_mass, ground_inertia);
	cube_shape->calculateLocalInertia(cube_mass, cube_inertia);
	//剛体オブジェクト生成
	sphere_body_ = new btRigidBody(sphere_mass, sphere_motion_state, sphere_shape, sphere_inertia);
	ground_body_ = new btRigidBody(ground_mass, ground_motion_state, ground_shape, ground_inertia);
	cube_body_ = new btRigidBody(cube_mass, cube_motion_state, cube_shape, cube_inertia);
	//反発係数
	sphere_body_->setRestitution(sphere_rest);
	ground_body_->setRestitution(ground_rest);
	cube_body_->setRestitution(cube_rest);
	//摩擦
//	sphere_body_->setFriction(100000);
//	ground_body_->setFriction(0.5);
//	cube_body_->setFriction(100000);
	//ワールドに剛体オブジェクトを追加
	dynamics_world_->addRigidBody(sphere_body_);
	dynamics_world_->addRigidBody(ground_body_);
	dynamics_world_->addRigidBody(cube_body_);
}

//デストラクタ
BtDemoScene::~BtDemoScene() {
	//オブジェクト破壊
	delete sphere_body_->getMotionState();
	delete ground_body_->getMotionState();
	delete cube_body_->getMotionState();
	dynamics_world_->removeRigidBody(sphere_body_);
	dynamics_world_->removeRigidBody(ground_body_);
	dynamics_world_->removeRigidBody(cube_body_);
	delete sphere_body_;
	delete ground_body_;
	delete cube_body_;

	//ワールド破壊
	delete dynamics_world_->getBroadphase();
	delete dynamics_world_;
}

//更新
void BtDemoScene::Update() {
	//bulletをすすめる
	dynamics_world_->stepSimulation(1.0 / kFps);

	//カメラ更新
	btVector3 pos = sphere_body_->getCenterOfMassPosition();
	camera.Update(pos[0], pos[1], pos[2]);

	//ライト
	GLfloat kLight0Pos[4] = { 0.0, 15.0, 0.0, 1.0 }; //ライト位置
	glLightfv(GL_LIGHT0, GL_POSITION, kLight0Pos);

	//撃力を加える
	btVector3 impulse;
	const double ang = camera.get_angle_w() + M_PI * 3.0 / 2.0;
	printf("%f,%f,%f\n", ang, sin(ang), cos(ang));
	if (input::get_keyboard_frame('w') == 1) {
		impulse.setValue(0.01, 0, 0);
		sphere_body_->applyCentralImpulse(impulse);
	}
	if (input::get_keyboard_frame('s') == 1) {
		impulse.setValue(sin(-ang), 0, cos(-ang));
		sphere_body_->applyCentralImpulse(impulse);
	}
	if (input::get_keyboard_frame('a') == 1) {
		impulse.setValue(sin(ang + M_PI / 2.0), 0, cos(ang + M_PI / 2.0));
		sphere_body_->applyCentralImpulse(impulse);
	}
	if (input::get_keyboard_frame('d') == 1) {
		impulse.setValue(sin(ang - M_PI / 2.0), 0, cos(ang - M_PI / 2.0));
		sphere_body_->applyCentralImpulse(impulse);
	}


}

//描画
void BtDemoScene::Draw() const {
	btVector3 pos;

	//地面
	glPushMatrix();
	pos = ground_body_->getCenterOfMassPosition();
	glTranslatef(pos[0], pos[1], pos[2]);
	const btBoxShape* ground_shape = static_cast<const btBoxShape*>(ground_body_->getCollisionShape());
	btVector3 ground_half_extent = ground_shape->getHalfExtentsWithMargin();
	uDrawGround(ground_half_extent[0] * 2);
	glPopMatrix();

	//直方体
	glPushMatrix();
	GLfloat cube_m[16];
	btDefaultMotionState *cube_motion = static_cast<btDefaultMotionState*>(cube_body_->getMotionState());
	cube_motion->m_graphicsWorldTrans.getOpenGLMatrix(cube_m);
	glMultMatrixf(cube_m);
	const btBoxShape* cube_shape = static_cast<const btBoxShape*>(cube_body_->getCollisionShape());
	btVector3 cube_half_extent = cube_shape->getHalfExtentsWithMargin();
	glScaled(2 * cube_half_extent[0], 2 * cube_half_extent[1], 2 * cube_half_extent[2]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, uMaterial4fv_green);
	glutSolidCube(1.0);
	glPopMatrix();

	//球
	pos = sphere_body_->getCenterOfMassPosition();
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, uMaterial4fv_brown);
	glutSolidSphere(1.0, 20, 20);
	glPopMatrix();
}
