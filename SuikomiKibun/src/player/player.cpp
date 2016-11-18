#include "player.h"

btRigidBody* Player::delete_body_ = NULL;

//コンストラクタ
Player::Player(btDynamicsWorld* world) :
		world_(world) {
	//中心座標
	btVector3 sphere_pos = btVector3(-1, 5, 0);
	//大きさ
	player_radius_ = 1.0;
	//質量
	btScalar sphere_mass = 0.03;
	//反発係数
	btScalar sphere_rest = 0.8;
	//慣性モーメント
	btVector3 sphere_inertia(0, 0, 0);
	//形状を設定
	btCollisionShape *sphere_shape = new btSphereShape(player_radius_);
	//球体の初期位置、姿勢
	btQuaternion qrot(0, 0, 0, 1);
	btDefaultMotionState* sphere_motion_state = new btDefaultMotionState(
			btTransform(qrot, sphere_pos));

	//慣性モーメントの計算
	sphere_shape->calculateLocalInertia(sphere_mass, sphere_inertia);

	//剛体オブジェクト生成
	sphere_body_ = new btRigidBody(sphere_mass, sphere_motion_state,
			sphere_shape, sphere_inertia);
	//反発係数
	sphere_body_->setRestitution(sphere_rest);
	//sphere_body_->setUserPointer(&m_BodyData1);  // ユーザーデータをセット

	//ワールドに剛体オブジェクトを追加
	world_->addRigidBody(sphere_body_);

	// 衝突のコールバック関数をセット
	gContactProcessedCallback = Player::HandleContactProcess;
}

//デストラクタ
Player::~Player() {
	//オブジェクト破壊
	delete sphere_body_->getMotionState();
	world_->removeRigidBody(sphere_body_);
	delete sphere_body_;
}

//更新
void Player::Update(double angle) {
	//撃力を加える
	btVector3 impulse;
	static btVector3 pos;
	double t = 0.1;
	//btTransform xform;
	//sphere_body_->getMotionState()->getWorldTransform(xform);
	if (input::get_special_keyboard_frame(GLUT_KEY_SHIFT_L) >= 1) {
		if (input::get_keyboard_frame('w') >= 1) {
			pos[0] += (t * cos(angle))/100;
			pos[2] += (t * sin(angle))/100;

//			sphere_body_->activate(true);
//						static float angle = 0.1f;
//						angle += 1.0f;
//						btTransform xform;
//						sphere_body_->getMotionState()->getWorldTransform(xform);
//						xform.setRotation(btQuaternion(btVector3(0.0, 1.0, 0.0), angle));
//
//						btVector3 forwardDir = xform.getBasis()[2];
//						btVector3 walkDirection = btVector3(0.0, 0.0, 0.0);
//
//						walkDirection = forwardDir;
//
//						btVector3 pos = sphere_body_->getCenterOfMassPosition();
//						pos[2] += 0.01;
//						sphere_body_->setCenterOfMassTransform(xform);

//			sphere_body_->activate(true);
//			static float tr = 0.f;
//			tr += 0.01f;
//			sphere_body_->translate(btVector3(0,0,tr));
//			btTransform tr;
//
//			sphere_body_->getMotionState()->getWorldTransform(tr);
//			static float angle = 0.f;
//			angle += 0.01f;
//			tr.setRotation(btQuaternion(btVector3(0, 0, 1), angle));
//			sphere_body_->getMotionState()->setWorldTransform(tr);
			/*sphere_body_->activate(true);
			 impulse.setValue(t * cos(angle), 0, t * sin(angle));
			 sphere_body_->applyCentralForce(impulse);*/
		}
		if (input::get_keyboard_frame('s') >= 1) {
			sphere_body_->activate(true);
			impulse.setValue(t * cos(angle + M_PI), 0, t * sin(angle + M_PI));
			sphere_body_->applyCentralForce(impulse);
		}
		if (input::get_keyboard_frame('a') >= 1) {
			sphere_body_->activate(true);
			impulse.setValue(t * cos(angle - M_PI / 2.0), 0,
					t * sin(angle - M_PI / 2.0));
			sphere_body_->applyCentralForce(impulse);
		}
		if (input::get_keyboard_frame('d') >= 1) {
			sphere_body_->activate(true);
			impulse.setValue(t * cos(angle + M_PI / 2.0), 0,
					t * sin(angle + M_PI / 2.0));
			sphere_body_->applyCentralForce(impulse);
		}
		if (input::get_keyboard_frame(' ') >= 1) {
			sphere_body_->activate(true);
			impulse.setValue(0, 0.3, 0);
			sphere_body_->applyCentralForce(impulse);
		}
	} else {
		if (input::get_keyboard_frame('w') == 1) {
			pos = sphere_body_->getCenterOfMassPosition();
			pos[0] += (t * cos(angle))/100;
			pos[2] += (t * sin(angle))/100;
//			sphere_body_->activate(true);
//			static float angle = 0.1f;
//			angle += 0.01f;
//			btTransform xform;
//			sphere_body_->getMotionState()->getWorldTransform(xform);
//			xform.setRotation(btQuaternion(btVector3(0.0, 1.0, 0.0), angle));
//
//			btVector3 forwardDir = xform.getBasis()[2];
//			btVector3 walkDirection = btVector3(0.0, 0.0, 0.0);
//
//			walkDirection = forwardDir;
//
//			btVector3 pos = sphere_body_->getCenterOfMassPosition();
//			pos[2] += 0.01;
//			sphere_body_->setCenterOfMassTransform(xform);
//			static float tr = 0.f;
//			tr += 0.01f;
//			sphere_body_->translate(btVector3(0,0,tr));

			/*btTransform tr =
			 world_->getCollisionObjectArray()[1]->getWorldTransform();
			 static float angle = 0.f;
			 angle += 0.01f;*/
			//tr.setRotation(btQuaternion(btVector3(0, 0, 1), angle));
			//world_->getCollisionObjectArray()[1]->setWorldTransform(tr);
			/*impulse.setValue(t * cos(angle), 0, t * sin(angle));
			 sphere_body_->applyCentralImpulse(impulse);*/
		}
		if (input::get_keyboard_frame('s') == 1) {
			sphere_body_->activate(true);
			impulse.setValue(t * cos(angle + M_PI), 0, t * sin(angle + M_PI));
			sphere_body_->applyCentralImpulse(impulse);
		}
		if (input::get_keyboard_frame('a') == 1) {
			sphere_body_->activate(true);
			impulse.setValue(t * cos(angle - M_PI / 2.0), 0,
					t * sin(angle - M_PI / 2.0));
			sphere_body_->applyCentralImpulse(impulse);
		}
		if (input::get_keyboard_frame('d') == 1) {
			sphere_body_->activate(true);
			impulse.setValue(t * cos(angle + M_PI / 2.0), 0,
					t * sin(angle + M_PI / 2.0));
			sphere_body_->applyCentralImpulse(impulse);
		}
		if (input::get_keyboard_frame(' ') == 1) {
			sphere_body_->activate(true);
			impulse.setValue(0, t, 0);
			sphere_body_->applyCentralImpulse(impulse);
		}
	}

	//player_radius_ += 0.01;
	//PlayerSize(player_radius_);
	PlayerMove(pos);

	int i;
	btCollisionObject* obj;
	btRigidBody* body;
	if (delete_body_ != NULL) {
		for (i = world_->getNumCollisionObjects() - 2; i >= 5; i--) {
			obj = world_->getCollisionObjectArray()[i];
			body = btRigidBody::upcast(obj);
			if (delete_body_ == body)
				DeleteBody(&delete_body_);	// 削除テスト
		}
	}

}

//描画
void Player::Draw() const {

	//球
	/*
	 btVector3 pos = sphere_body_->getCenterOfMassPosition();
	 glPushMatrix();
	 glTranslatef(pos[0], pos[1], pos[2]);
	 btScalar player_radius;
	 player_radius = static_cast<const btSphereShape*>(sphere_body_->getCollisionShape())->getRadius();
	 glScalef(player_radius, player_radius, player_radius);
	 glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, uMaterial4fv_brown);
	 glutSolidSphere(1.0, 20, 20);
	 glPopMatrix();
	 */
}

void Player::PlayerSize(double size) {
	//形状を設定
	btCollisionShape *new_sphere_shape = new btSphereShape(size);
	delete sphere_body_->getCollisionShape();
	sphere_body_->setCollisionShape(new_sphere_shape);
}

void Player::PlayerMove(btVector3 pos) {
	btVector3 t_pos = sphere_body_->getCenterOfMassPosition();
	btVector3 i_pos = sphere_body_->getCenterOfMassPosition();
	t_pos[0] = pos[0] - i_pos[0];
	t_pos[1] = i_pos[1] = 0;
	t_pos[2] = pos[2] - i_pos[2];
	sphere_body_->translate(pos);

}

Vector3 Player::get_center_pos() {
	btVector3 pos = sphere_body_->getCenterOfMassPosition();
	Vector3 rpos(pos[0], pos[1], pos[2]);
	return rpos;
}

double Player::get_camera_distance() {
	return player_radius_ * 3;
}

void Player::DeleteBody(btRigidBody** ppBody) {
	btRigidBody* pBody = *ppBody;
	world_->removeRigidBody(pBody);
	if (pBody) {
		delete pBody->getMotionState();
	}
	delete pBody;
	*ppBody = NULL;
}

bool Player::HandleContactProcess(btManifoldPoint& p, void* a, void* b) {
	delete_body_ = static_cast<btRigidBody*>(a);
	//btRigidBody* pBody1 = (btRigidBody*) b;

	//	btRigid
//	btRigidBody* obj = world_->getCollisionObjectArray()[0];
	//	if(pBody){
	//		delete pBody->getMotionState();
//		}
	//	delete pBody;
	//	pBody0 = NULL;

//	DeleteBody(pBody0);

	//TestData* pUserData0 = (TestData*) pBody0->getUserPointer();
//	TestData* pUserData1 = (TestData*) pBody1->getUserPointer();

	// カウント
	//if (pUserData0)
	//pUserData0->count++;
//	if (pUserData1)
//		pUserData1->count++;

	return true;
}

