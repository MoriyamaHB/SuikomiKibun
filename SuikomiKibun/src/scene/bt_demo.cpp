#include "bt_demo.h"

//コンストラクタ
BtDemoScene::BtDemoScene(ISceneChanger* changer, SceneParam param) :
		BaseScene(changer) {

	btVector3 pos = btVector3(-1, 5, 0); //中心座標
	btVector3 pos2 = btVector3(0, 0, 0); //中心座標
	btScalar mass = 0.03; //質量
	btScalar radius = 1.0;
	btVector3 extents = btVector3(-25, 0.1, 25);

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
	btCollisionShape *sphere_shape = new btSphereShape(radius);
	btCollisionShape *ground_shape = new btBoxShape(extents);

	//球体の初期位置、姿勢
	btQuaternion qrot(0, 0, 0, 1);
	btDefaultMotionState* motion_state = new btDefaultMotionState(btTransform(qrot, pos));
	btDefaultMotionState* motion_state2 = new btDefaultMotionState(btTransform(qrot, pos2));

	//慣性モーメントの計算
	btVector3 inertia(0, 0, 0);
	sphere_shape->calculateLocalInertia(mass, inertia);

	//剛体オブジェクト生成
	sphere_body_ = new btRigidBody(mass, motion_state, sphere_shape, inertia);
	ground_body_ = new btRigidBody(0.00, motion_state2, ground_shape, inertia);

	//ワールドに剛体オブジェクトを追加
	dynamics_world_->addRigidBody(sphere_body_);
	dynamics_world_->addRigidBody(ground_body_);

	//撃力を加える
	btVector3 impulse(0.1, 0.01, 0);
	sphere_body_->applyCentralImpulse(impulse);
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
	btVector3 pos = ground_body_->getCenterOfMassPosition();
	glPushMatrix();
	//glTranslated(pos[0], pos[1], pos[2]);
	uDrawGround(50);
	glPopMatrix();

	pos = sphere_body_->getCenterOfMassPosition();
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);
	printf("%5.3f,%5.3f,%5.3f\n", pos[0], pos[1], pos[2]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, uMaterial4fv_brown);
	glutSolidSphere(1.0, 20, 20);
	glPopMatrix();
}
