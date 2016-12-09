/*
 * playerteki.cpp
 *
 *  Created on: 2016/12/02
 *      Author: c501506069
 */

#include "playerteki.h"


PlayerTeki::PlayerTeki(btDynamicsWorld* world, btVector3 pos) :
		world_(world) {
	//中心座標
	btVector3 sphere_pos = pos;
	//大きさ
	player_radius_ = 1.0;
	//質量
	btScalar sphere_mass = 0.03;
	//反発係数
	btScalar sphere_rest = 0.2;
	//慣性モーメント
	btVector3 sphere_inertia(0, 0, 0);
	//形状を設定
	btCollisionShape *sphere_shape = new btSphereShape(player_radius_);
	//球体の初期位置、姿勢
	btQuaternion qrot(0, 0, 0, 1);
	btDefaultMotionState* sphere_motion_state = new btDefaultMotionState(
			btTransform(qrot, sphere_pos));
	//剛体オブジェクト生成
	sphere_body_ = new btRigidBody(sphere_mass, sphere_motion_state,
			sphere_shape, sphere_inertia);
	//反発係数
	sphere_body_->setRestitution(sphere_rest);
	//ワールドに剛体オブジェクトを追加
	world_->addRigidBody(sphere_body_);



	color_[0] = btVector3(1, 0, 0);
	color_[1] = btVector3(0, 1, 0);
	color_[2] = btVector3(0, 0, 1);
	color_judge_ = 0;

	playerteki_num_ = world_->getNumCollisionObjects() - 1;

		//描画
		m_shapeDrawer = new GL_ShapeDrawer();
		m_shapeDrawer->enableTexture(true);

}


PlayerTeki::~PlayerTeki() {

}

void PlayerTeki::RenderScene() {
	btScalar m[16];
	btMatrix3x3 rot;
	rot.setIdentity();
	btVector3 wireColor(1, 0, 0);
		btCollisionObject* colObj = world_->getCollisionObjectArray()[playerteki_num_];
		btRigidBody* body = btRigidBody::upcast(colObj);
		if (body && body->getMotionState()) {
			btDefaultMotionState* myMotionState =
					(btDefaultMotionState*) body->getMotionState();
			myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(m);
			rot = myMotionState->m_graphicsWorldTrans.getBasis();
		} else {
			colObj->getWorldTransform().getOpenGLMatrix(m);
			rot = colObj->getWorldTransform().getBasis();
		}

		if (color_judge_ == 1)
			wireColor = color_[0];
		else if (color_judge_ == 2)
			wireColor = color_[1];
		else
			wireColor = color_[2];

		btVector3 aabbMin, aabbMax;
		world_->getBroadphase()->getBroadphaseAabb(aabbMin, aabbMax);

		aabbMin -= btVector3(BT_LARGE_FLOAT, BT_LARGE_FLOAT, BT_LARGE_FLOAT);
		aabbMax += btVector3(BT_LARGE_FLOAT, BT_LARGE_FLOAT, BT_LARGE_FLOAT);

		m_shapeDrawer->drawOpenGL(m, colObj->getCollisionShape(),
				wireColor * btScalar(0.3), 0, aabbMin, aabbMax);

}

void PlayerTeki::Draw() {
	glDisable(GL_LIGHTING);
	RenderScene();
}


//プレイヤー敵データ更新
void PlayerTeki::Update(btVector3 pos) {
	PlayerTekiMove(pos);
}

void PlayerTeki::PlayerTekiMove(btVector3 pos) {
	btQuaternion qrot(0, 0, 0, 1);
	btDefaultMotionState* sphere_motion_state = new btDefaultMotionState(
			btTransform(qrot, pos));
	sphere_body_->setMotionState(sphere_motion_state);
}


