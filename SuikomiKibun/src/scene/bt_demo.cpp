#include "bt_demo.h"

//コンストラクタ
BtDemoScene::BtDemoScene(ISceneChanger* changer, SceneParam param) :
		BaseScene(changer) {

	//中心座標
	btVector3 sphere_pos = btVector3(-1, 5, 0);
	btVector3 ground_pos = btVector3(0, 0, 0);
	//大きさ
	btScalar sphere_radius = 1.0;
	btVector3 ground_extents = btVector3(25, 0.1, 25);
	//質量
	btScalar sphere_mass = 0.03;
	btScalar ground_mass = 0.0;

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

	//形状を設定
	btCollisionShape *sphere_shape = new btSphereShape(sphere_radius);
	btCollisionShape *ground_shape = new btBoxShape(ground_extents);

	//球体の初期位置、姿勢
	btQuaternion qrot(0, 0, 0, 1);
	btDefaultMotionState* motion_state = new btDefaultMotionState(btTransform(qrot, sphere_pos));
	btDefaultMotionState* motion_state2 = new btDefaultMotionState(btTransform(qrot, ground_pos));

	//慣性モーメントの計算
	btVector3 inertia(0, 0, 0);
	sphere_shape->calculateLocalInertia(sphere_mass, inertia);

	//剛体オブジェクト生成
	sphere_body_ = new btRigidBody(sphere_mass, motion_state, sphere_shape, inertia);
	ground_body_ = new btRigidBody(ground_mass, motion_state2, ground_shape, inertia);

	//ワールドに剛体オブジェクトを追加
	dynamics_world_->addRigidBody(sphere_body_);
	dynamics_world_->addRigidBody(ground_body_);

	//撃力を加える
//	btVector3 impulse(0.1, 0.01, 0);
//	sphere_body_->applyCentralImpulse(impulse);

//反発係数
	sphere_body_->setRestitution(0.8);
	ground_body_->setRestitution(1.0);
}

//デストラクタ
BtDemoScene::~BtDemoScene() {
	//オブジェクト破壊
	delete sphere_body_->getMotionState();
	delete ground_body_->getMotionState();
	dynamics_world_->removeRigidBody(sphere_body_);
	dynamics_world_->removeRigidBody(ground_body_);
	delete sphere_body_;
	delete ground_body_;

	//ワールド破壊
	delete dynamics_world_->getBroadphase();
	delete dynamics_world_;
}

//更新
void BtDemoScene::Update() {
	camera.Update();

	//ライト
	GLfloat kLight0Pos[4] = { 0.0, 15.0, 0.0, 1.0 }; //ライト位置
	glLightfv(GL_LIGHT0, GL_POSITION, kLight0Pos);

	dynamics_world_->stepSimulation(1.0 / kFps);

}

//描画
void BtDemoScene::Draw() const {
	btVector3 pos;

	glPushMatrix();
	pos = ground_body_->getCenterOfMassPosition();
	glTranslatef(pos[0], pos[1], pos[2]);
	const btBoxShape* box_shape = static_cast<const btBoxShape*>(ground_body_->getCollisionShape());
	btVector3 half_extent = box_shape->getHalfExtentsWithMargin();
//	glScaled(2 * half_extent[0], 2 * half_extent[1], 2 * half_extent[2]);
//	glutSolidCube(1.0);
	uDrawGround(half_extent[0] * 2);
	glPopMatrix();

	pos = sphere_body_->getCenterOfMassPosition();
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, uMaterial4fv_brown);
	glutSolidSphere(1.0, 20, 20);
	glPopMatrix();
}
