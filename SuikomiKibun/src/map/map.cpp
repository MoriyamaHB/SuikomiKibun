/*
 * map.cpp
 *
 *  Created on: 2016/09/11
 *      Author: mi
 */
#include "map.h"

//コンストラクタ
StageMap::StageMap(btDynamicsWorld* world):world_(world)
{
	//中心座標
	btVector3 ground_pos = btVector3(0, 0, 0);
	btVector3 cube_pos = btVector3(0, 0, 2);
	btVector3 cube_pos2 = btVector3(2,0, 0);
	btVector3 cube_pos3 = btVector3(-2, 0, 0);
	btVector3 cube_pos4 = btVector3(0,0, -2);
	//大きさ
	btVector3 ground_extents = btVector3(100, 0.00001, 100);
	double a = 1.0, b = 1.0, c = 1.0;
	btVector3 cube_extents = btVector3(a, b, c);
	//質量
	btScalar ground_mass = 0.0;
	btScalar cube_mass = 0.001;
	//反発係数
	btScalar ground_rest = 1.0;
	btScalar cube_rest = 0.5;
	//慣性モーメント
	btVector3 ground_inertia(0, 0, 0);
	btVector3 cube_inertia(cube_mass * (b * b + c * c) / 3.0, cube_mass * (a * a + c * c) / 3.0,
			cube_mass * (b * b + a * a) / 3.0);
	//形状を設定
	btCollisionShape *ground_shape = new btBoxShape(ground_extents);
	btCollisionShape *cube_shape = new btBoxShape(cube_extents);
	//初期位置姿勢
	btQuaternion qrot2(0, 0, 0, 1);
	btDefaultMotionState* ground_motion_state = new btDefaultMotionState(btTransform(qrot2, ground_pos));
	btDefaultMotionState* cube_motion_state = new btDefaultMotionState(btTransform(qrot2, cube_pos));
	btDefaultMotionState* cube_motion_state2 = new btDefaultMotionState(btTransform(qrot2, cube_pos2));
	btDefaultMotionState* cube_motion_state3 = new btDefaultMotionState(btTransform(qrot2, cube_pos3));
	btDefaultMotionState* cube_motion_state4 = new btDefaultMotionState(btTransform(qrot2, cube_pos4));
	//慣性モーメントの計算
	ground_shape->calculateLocalInertia(ground_mass, ground_inertia);
	cube_shape->calculateLocalInertia(cube_mass, cube_inertia);
	//剛体オブジェクト生成
	ground_body_ = new btRigidBody(ground_mass, ground_motion_state, ground_shape, ground_inertia);
	cube_body_ = new btRigidBody(cube_mass, cube_motion_state, cube_shape, cube_inertia);
	cube_body2_ = new btRigidBody(cube_mass, cube_motion_state2, cube_shape, cube_inertia);
	cube_body3_ = new btRigidBody(cube_mass, cube_motion_state3, cube_shape, cube_inertia);
	cube_body4_ = new btRigidBody(cube_mass, cube_motion_state4, cube_shape, cube_inertia);
	//反発係数
	ground_body_->setRestitution(ground_rest);
	cube_body_->setRestitution(cube_rest);
	cube_body2_->setRestitution(cube_rest);
	cube_body3_->setRestitution(cube_rest);
	cube_body4_->setRestitution(cube_rest);
	//ワールドに剛体オブジェクトを追加

	world_->addRigidBody(ground_body_);
	world_->addRigidBody(cube_body_);
	world_->addRigidBody(cube_body2_);
	world_->addRigidBody(cube_body3_);
	world_->addRigidBody(cube_body4_);

}

//デストラクタ
StageMap::~StageMap(){

	delete ground_body_->getMotionState();
	delete cube_body_->getMotionState();
	delete cube_body2_->getMotionState();
	delete cube_body3_->getMotionState();
	delete cube_body4_->getMotionState();
	world_->removeRigidBody(ground_body_);
	world_->removeRigidBody(cube_body_);
	world_->removeRigidBody(cube_body2_);
	world_->removeRigidBody(cube_body3_);
	world_->removeRigidBody(cube_body4_);
	delete ground_body_;
	delete cube_body_;
	delete cube_body2_;
	delete cube_body3_;
	delete cube_body4_;

}

//更新
void StageMap::Update(){

}

//描画
void StageMap::Draw() const{

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
	glScaled(2*cube_half_extent[0], 2*cube_half_extent[1], 2*cube_half_extent[2]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, uMaterial4fv_blue);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	GLfloat cube_m2[16];
	btDefaultMotionState *cube_motion2 = static_cast<btDefaultMotionState*>(cube_body2_->getMotionState());
	cube_motion2->m_graphicsWorldTrans.getOpenGLMatrix(cube_m2);
	glMultMatrixf(cube_m2);
	const btBoxShape* cube_shape2 = static_cast<const btBoxShape*>(cube_body2_->getCollisionShape());
	btVector3 cube_half_extent2 = cube_shape2->getHalfExtentsWithMargin();
	glScaled(2*cube_half_extent2[0], 2*cube_half_extent2[1], 2*cube_half_extent2[2]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, uMaterial4fv_blue);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	GLfloat cube_m3[16];
	btDefaultMotionState *cube_motion3 = static_cast<btDefaultMotionState*>(cube_body3_->getMotionState());
	cube_motion3->m_graphicsWorldTrans.getOpenGLMatrix(cube_m3);
	glMultMatrixf(cube_m3);
	const btBoxShape* cube_shape3 = static_cast<const btBoxShape*>(cube_body3_->getCollisionShape());
	btVector3 cube_half_extent3 = cube_shape3->getHalfExtentsWithMargin();
	glScaled(2*cube_half_extent3[0], 2*cube_half_extent3[1], 2*cube_half_extent3[2]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, uMaterial4fv_blue);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	GLfloat cube_m4[16];
	btDefaultMotionState *cube_motion4 = static_cast<btDefaultMotionState*>(cube_body4_->getMotionState());
	cube_motion4->m_graphicsWorldTrans.getOpenGLMatrix(cube_m4);
	glMultMatrixf(cube_m4);
	const btBoxShape* cube_shape4 = static_cast<const btBoxShape*>(cube_body4_->getCollisionShape());
	btVector3 cube_half_extent4 = cube_shape4->getHalfExtentsWithMargin();
	glScaled(2*cube_half_extent4[0], 2*cube_half_extent4[1], 2*cube_half_extent4[2]);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, uMaterial4fv_blue);
	glutSolidCube(1.0);
	glPopMatrix();

}


