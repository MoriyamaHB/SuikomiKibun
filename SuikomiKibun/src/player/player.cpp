#include "player.h"

//コンストラクタ
Player::Player(btDynamicsWorld* world) :world_(world) {
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
	//ワールドに剛体オブジェクトを追加
	world_->addRigidBody(sphere_body_);

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

	double t = 0.1;
	if (input::get_special_keyboard_frame(GLUT_KEY_SHIFT_L) >= 1) {
		if (input::get_keyboard_frame('w') >= 1) {
			sphere_body_->activate(true);
			impulse.setValue(t * cos(angle), 0, t * sin(angle));
			sphere_body_->applyCentralForce(impulse);
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
			sphere_body_->activate(true);
			impulse.setValue(t * cos(angle), 0, t * sin(angle));
			sphere_body_->applyCentralImpulse(impulse);
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

	player_radius_+= 0.01;
	PlayerSize(player_radius_);
}

//描画
void Player::Draw() const {
	//球
	btVector3 pos = sphere_body_->getCenterOfMassPosition();
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);
	btScalar player_radius;
	player_radius = static_cast<const btSphereShape*>(sphere_body_->getCollisionShape())->getRadius();
	glScalef(player_radius,player_radius,player_radius);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, uMaterial4fv_brown);
	glutSolidSphere(1.0, 20, 20);
	glPopMatrix();
}

void Player::PlayerSize(double size){
	//形状を設定
		btCollisionShape *new_sphere_shape = new btSphereShape(size);
		delete sphere_body_->getCollisionShape();
		sphere_body_->setCollisionShape(new_sphere_shape);
}

Vector3 Player::get_center_pos() {
	btVector3 pos = sphere_body_->getCenterOfMassPosition();
	Vector3 rpos(pos[0], pos[1], pos[2]);
	return rpos;
}

double Player::get_camera_distance(){
	return player_radius_ * 3;

}

