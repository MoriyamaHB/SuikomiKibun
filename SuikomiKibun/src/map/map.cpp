/*
 * map.cpp
 *
 *  Created on: 2016/09/11
 *      Author: mi
 */
#include "map.h"
#include "GlutStuff.h"
#include "btBulletDynamicsCommon.h"
#include "BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h"//picking
#include "BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h"//picking

#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "BulletCollision/CollisionShapes/btSphereShape.h"
#include "BulletCollision/CollisionShapes/btCompoundShape.h"
#include "BulletCollision/CollisionShapes/btUniformScalingShape.h"
#include "BulletDynamics/ConstraintSolver/btConstraintSolver.h"
#include "GL_ShapeDrawer.h"
#include "LinearMath/btQuickprof.h"
#include "LinearMath/btDefaultMotionState.h"
#include "LinearMath/btSerializer.h"


//コンストラクタ
StageMap::StageMap(btDynamicsWorld* world)
	:m_enableshadows(true),
	 m_sundirection(btVector3(1,-2,1)*1000),
	 m_defaultContactProcessingThreshold(BT_LARGE_FLOAT),
	 world_(world)
{
	num_ = 0;
	object_num_ = 0;
	memset(object_, 0, 200);

	//オブジェクト
	btRigidBody* ground_body;
	btRigidBody* ground_body2;
	btRigidBody* wall_body[10];
	btRigidBody* pole_body;
	btRigidBody* kan_body[5];
	//中心座標
	btVector3 ground_pos = btVector3(0, 0, 0);
	btVector3 ground_pos2 = btVector3(6, 155, 6);
	btVector3 ground_pos3 = btVector3(6, 155, -6);
	btVector3 ground_pos4 = btVector3(-6, 155, 6);
	btVector3 ground_pos5 = btVector3(-6, 155, -6);
	btVector3 stairs_pos = btVector3(-7, 148, 3);
	btVector3 stairs_pos2 = btVector3(0, -0.4 , 5);
	btVector3 stairs_pos3 = btVector3(-10, 154, 10);
	btVector3 stairs_pos4 = btVector3(10, 154, 10);
	btVector3 stairs_pos5 = btVector3(10, 154, -10);
	btVector3 stairs_pos6 = btVector3(-10, 154, -10);
	btVector3 wall_pos = btVector3(0, 100, 200);
	btVector3 wall_pos2 = btVector3(0, 100, -200);
	btVector3 wall_pos3 = btVector3(200, 100, 0);
	btVector3 wall_pos4 = btVector3(-200, 100, 0);
	btVector3 wall_pos5 = btVector3(100, 50, 0);
	btVector3 wall_pos6 = btVector3(-100, 50, 0);
	btVector3 wall_pos7 = btVector3(0, 50, 150);
	btVector3 wall_pos8 = btVector3(0, 50, -150);
	btVector3 wall_pos9 = btVector3(50, 50, 100);
	btVector3 wall_pos10 = btVector3(-50, 50, -100);
	btVector3 pole_pos = btVector3(199, 100, 199);
	btVector3 pole_pos2 = btVector3(-199, 100, -199);
	btVector3 pole_pos3 = btVector3(-199, 100, 199);
	btVector3 pole_pos4 = btVector3(199, 100, -199);
	btVector3 pole_pos5 = btVector3(0, 50, 100);
	btVector3 pole_pos6 = btVector3(0, 50, -100);
	btVector3 pole_pos7 = btVector3(100, 50, 100);
	btVector3 pole_pos8 = btVector3(-100, 50, -100);
	btVector3 pole_pos9 = btVector3(100, 50, -100);
	btVector3 pole_pos10 = btVector3(-100, 50, 100);
	btVector3 pole_pos11 = btVector3(0, 50, 199);
	btVector3 pole_pos12 = btVector3(0, 50, -199);
	btVector3 kan_pos = btVector3(10, 10, 10);
	btVector3 kan_pos2 = btVector3(10, 10, 10);
	btTransform offset; offset.setIdentity();
	btTransform offset2; offset2.setIdentity();
	//形状を設定
	btCollisionShape *ground_shape = new btBoxShape(btVector3(400, 0.01, 200));
	btCollisionShape *ground_shape2 = new btBoxShape(btVector3(3, 0.01, 6));
	btCollisionShape *pole_shape = new btBoxShape(btVector3(1, 100, 1));
	btCollisionShape *pole_shape2 = new btBoxShape(btVector3(1, 50, 1));
	btCollisionShape *stairs = new btBoxShape(btVector3(4 , 0.01, 2));
	btCollisionShape *slide = new btBoxShape(btVector3(4 , 0.01, 150));
	btCollisionShape *wall_shape = new btBoxShape(btVector3(200, 100, 0.1));
	btCollisionShape *wall_shape2 = new btBoxShape(btVector3(100, 50, 0.1));
	btCollisionShape *wall_shape3 = new btBoxShape(btVector3(50, 50, 0.1));
	btCollisionShape *kan_shape = new btBoxShape(btVector3(10, 1, 8));

	//bulletに登録（地面＆壁）
	offset.setOrigin(ground_pos);
	ground_body = LocalCreateRigidBody(btScalar(0.), offset, ground_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(1.0, 1.0, 1.0);
	offset.setOrigin(ground_pos2);
	offset.setRotation(btQuaternion(-PI_ / 4, 0, 0));
	ground_body2 = LocalCreateRigidBody(btScalar(0), offset, ground_shape2);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(1.0, 1.0, 1.0);
	offset.setOrigin(ground_pos3);
	offset.setRotation(btQuaternion(PI_ / 4, 0, 0));
	ground_body2 = LocalCreateRigidBody(btScalar(0), offset, ground_shape2);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(1.0, 1.0, 1.0);
	offset.setOrigin(ground_pos4);
	offset.setRotation(btQuaternion(PI_ / 4, 0, 0));
	ground_body2 = LocalCreateRigidBody(btScalar(0), offset, ground_shape2);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(1.0, 1.0, 1.0);
	offset.setOrigin(ground_pos5);
	offset.setRotation(btQuaternion(-PI_ / 4, 0, 0));
	ground_body2 = LocalCreateRigidBody(btScalar(0), offset, ground_shape2);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(1.0, 1.0, 1.0);

	//螺旋階段
	double h, r, g, b;
	int h2;
	h = 0;
	offset.setOrigin(stairs_pos);
	offset2.setOrigin(stairs_pos2);
	for(int ten = 0; ten < 300; ten++){
		offset.setRotation(btQuaternion(PI_ / 5 * ten + PI_ / 4, 0, 0));
		ground_body2 = LocalCreateRigidBody(btScalar(0), offset, stairs);
		h += 1.1;
		h2 = h / 60;
		if(h2 == 5){
			r = 255;
			g = 0;
			b = ((360-h) / 60) * 255;
		}else if(h2 == 4){
			r = ((h - 240) / 60) * 255;
			g = 0;
			b = 255;
		}else if(h2 == 3){
			r = 0;
			g = ((240 - h) / 60) * 255;
			b = 255;
		}else if(h2 == 2){
			r = 0;
			g = 255;
			b = ((h-120) / 60) * 255;
		}else if(h2){
			r = ((120-h) / 60) * 255;
			g = 255;
			b = 0;
		}else{
			r = 255;
			g = (h / 60) * 255;
			b = 0;
		}
		object_[num_] = object_num_;
		color_[num_++] = btVector3(r/255, g/255, b/255);
		offset = offset*offset2;
	}

    //	赤階段
	offset.setOrigin(stairs_pos3);
	offset2.setOrigin(stairs_pos2);
	for(int ten = 0; ten < 40; ten++){
		offset.setRotation(btQuaternion(PI_ / 70 * ten - PI_ / 4, 0, 0));
		ground_body2 = LocalCreateRigidBody(btScalar(0), offset, stairs);
		object_[num_] = object_num_;
		color_[num_++] = btVector3(1.0, 0.0, 0.0);
		offset = offset*offset2;
	}
	//	黄階段
	offset.setOrigin(stairs_pos4);
	offset2.setOrigin(stairs_pos2);
	for(int ten = 0; ten < 40; ten++){
		offset.setRotation(btQuaternion(PI_ / 70 * ten + PI_ / 4, 0, 0));
		ground_body2 = LocalCreateRigidBody(btScalar(0), offset, stairs);
		object_[num_] = object_num_;
		color_[num_++] = btVector3(1.0, 1.0, 0.0);
		offset = offset*offset2;
	}
	// 	青階段
	offset.setOrigin(stairs_pos5);
	offset2.setOrigin(stairs_pos2);
	for(int ten = 0; ten < 40; ten++){
		offset.setRotation(btQuaternion(PI_ / 70 * ten + PI_ * 3 / 4, 0, 0));
		ground_body2 = LocalCreateRigidBody(btScalar(0), offset, stairs);
		object_[num_] = object_num_;
		color_[num_++] = btVector3(0.0, 0.0, 1.0);
		offset = offset*offset2;
	}
	// 	緑階段
		offset.setOrigin(stairs_pos6);
		offset2.setOrigin(stairs_pos2);
		for(int ten = 0; ten < 40; ten++){
			offset.setRotation(btQuaternion(PI_ / 70 * ten - PI_ * 3 / 4, 0, 0));
			ground_body2 = LocalCreateRigidBody(btScalar(0), offset, stairs);
			object_[num_] = object_num_;
			color_[num_++] = btVector3(0.0, 1.0, 0.0);
			offset = offset*offset2;
		}

	//壁
	offset.setIdentity(); offset.setOrigin(wall_pos);
	wall_body[0] = LocalCreateRigidBody(btScalar(0.), offset, wall_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.94, 0.94, 0.82);
	offset.setIdentity(); offset.setOrigin(wall_pos2);
	wall_body[1] = LocalCreateRigidBody(btScalar(0.), offset, wall_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.94, 0.94, 0.82);
	offset.setIdentity(); offset.setOrigin(wall_pos3);
	offset.setRotation(btQuaternion(PI_ / 2, 0, 0));
	wall_body[2] = LocalCreateRigidBody(btScalar(0.), offset, wall_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.94, 0.94, 0.82);
	offset.setIdentity(); offset.setOrigin(wall_pos4);
	offset.setRotation(btQuaternion(-PI_ / 2, 0, 0));
	wall_body[3] = LocalCreateRigidBody(btScalar(0.), offset, wall_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.94, 0.94, 0.82);
	offset.setIdentity(); offset.setOrigin(wall_pos5);
	offset.setRotation(btQuaternion(PI_ / 2, 0, 0));
	wall_body[4] = LocalCreateRigidBody(btScalar(0.), offset, wall_shape2);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.9, 0.82, 0.63);
	offset.setIdentity(); offset.setOrigin(wall_pos6);
	offset.setRotation(btQuaternion(PI_ / 2, 0, 0));
	wall_body[5] = LocalCreateRigidBody(btScalar(0.), offset, wall_shape2);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.9, 0.82, 0.63);
	offset.setIdentity(); offset.setOrigin(wall_pos7);
	offset.setRotation(btQuaternion(PI_ / 2, 0, 0));
	wall_body[6] = LocalCreateRigidBody(btScalar(0.), offset, wall_shape3);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.9, 0.82, 0.63);
	offset.setIdentity(); offset.setOrigin(wall_pos8);
	offset.setRotation(btQuaternion(PI_ / 2, 0, 0));
	wall_body[7] = LocalCreateRigidBody(btScalar(0.), offset, wall_shape3);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.9, 0.82, 0.63);
	offset.setIdentity(); offset.setOrigin(wall_pos9);
	offset.setRotation(btQuaternion(0, 0, 0));
	wall_body[8] = LocalCreateRigidBody(btScalar(0.), offset, wall_shape3);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.9, 0.82, 0.63);
	offset.setIdentity(); offset.setOrigin(wall_pos10);
	offset.setRotation(btQuaternion(0, 0, 0));
	wall_body[9] = LocalCreateRigidBody(btScalar(0.), offset, wall_shape3);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.9, 0.82, 0.63);

	//柱
	offset.setIdentity(); offset.setOrigin(pole_pos);
	offset.setRotation(btQuaternion(0, 0, 0));
	pole_body = LocalCreateRigidBody(btScalar(0.), offset, pole_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.5, 0.5, 0.5);
	offset.setIdentity(); offset.setOrigin(pole_pos2);
	offset.setRotation(btQuaternion(0, 0, 0));
	pole_body = LocalCreateRigidBody(btScalar(0.), offset, pole_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.5, 0.5, 0.5);
	offset.setIdentity(); offset.setOrigin(pole_pos3);
	offset.setRotation(btQuaternion(0, 0, 0));
	pole_body = LocalCreateRigidBody(btScalar(0.), offset, pole_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.5, 0.5, 0.5);
	offset.setIdentity(); offset.setOrigin(pole_pos4);
	offset.setRotation(btQuaternion(0, 0, 0));
	pole_body = LocalCreateRigidBody(btScalar(0.), offset, pole_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.5, 0.5, 0.5);
	offset.setIdentity(); offset.setOrigin(pole_pos5);
	offset.setRotation(btQuaternion(0, 0, 0));
	pole_body = LocalCreateRigidBody(btScalar(0.), offset, pole_shape2);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.5, 0.5, 0.5);
	offset.setIdentity(); offset.setOrigin(pole_pos6);
	offset.setRotation(btQuaternion(0, 0, 0));
	pole_body = LocalCreateRigidBody(btScalar(0.), offset, pole_shape2);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.5, 0.5, 0.5);
	offset.setIdentity(); offset.setOrigin(pole_pos7);
	offset.setRotation(btQuaternion(0, 0, 0));
	pole_body = LocalCreateRigidBody(btScalar(0.), offset, pole_shape2);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.5, 0.5, 0.5);
	offset.setIdentity(); offset.setOrigin(pole_pos8);
	offset.setRotation(btQuaternion(0, 0, 0));
	pole_body = LocalCreateRigidBody(btScalar(0.), offset, pole_shape2);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.5, 0.5, 0.5);
	offset.setIdentity(); offset.setOrigin(pole_pos9);
	offset.setRotation(btQuaternion(0, 0, 0));
	pole_body = LocalCreateRigidBody(btScalar(0.), offset, pole_shape2);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.5, 0.5, 0.5);
	offset.setIdentity(); offset.setOrigin(pole_pos10);
	offset.setRotation(btQuaternion(0, 0, 0));
	pole_body = LocalCreateRigidBody(btScalar(0.), offset, pole_shape2);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.5, 0.5, 0.5);
	offset.setIdentity(); offset.setOrigin(pole_pos11);
	offset.setRotation(btQuaternion(0, 0, 0));
	pole_body = LocalCreateRigidBody(btScalar(0.), offset, pole_shape2);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.5, 0.5, 0.5);
	offset.setIdentity(); offset.setOrigin(pole_pos12);
	offset.setRotation(btQuaternion(0, 0, 0));
	pole_body = LocalCreateRigidBody(btScalar(0.), offset, pole_shape2);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.5, 0.5, 0.5);

	//観覧車
//	offset.setIdentity(); offset.setOrigin(kan_pos);
//	offset.setRotation(btQuaternion(0, 0, 0));
//	kan_body[0] = LocalCreateRigidBody(btScalar(0.001), offset, kan_shape);
//	object_[num_] = object_num_;
//	color_[num_++] = btVector3(0.5, 0.5, 0.5);
//	offset.setIdentity(); offset.setOrigin(kan_pos2);
//	offset.setRotation(btQuaternion(0, PI_ / 2, 0));
//	kan_body[1] = LocalCreateRigidBody(btScalar(0.001), offset, kan_shape);
//	object_[num_] = object_num_;
//	color_[num_++] = btVector3(0.5, 0.5, 0.5);
//	btHingeConstraint* hingeC;
//	btTransform localA, localB, localC;
//	btTypedConstraint* joint;
//	btVector3 btPivotA = btVector3(0, 0, 0);
//	btVector3 btAxisA = btVector3(1.0f, 0.0f, 0.0f);
//	localA.setIdentity(); localB.setIdentity();
//	localA.setIdentity(); localB.setIdentity();
//	localB = kan_body[0]->getWorldTransform().inverse() * kan_body[1]->getWorldTransform() * localA;
//	hingeC = new btHingeConstraint(*kan_body[0], *kan_body[1], localB, localA);
//	hingeC->setLimit(btScalar(0), btScalar(0));
//	joint = hingeC;
//	world_->addConstraint(joint, true);
//	hingeC = new btHingeConstraint(*kan_body[0], btPivotA, btAxisA);
//	hingeC->setLimit(btScalar(1), btScalar(-1));
//	joint = hingeC;
//	world_->addConstraint(joint, true);

	//反発係数
	btScalar ground_rest = 0.6;
	btScalar wall_rest = 0.4;

	//反発係数設定
	ground_body->setRestitution(ground_rest);
	for(int i = 0; i < 10; i++)
	{
		wall_body[i]->setRestitution(wall_rest);
	}


//	localB = kan_body[0]->getWorldTransform().inverse() * kan_body[1]->getWorldTransform() * localA;
//	hingeC = new btHingeConstraint(*kan_body[1], btPivotA, btAxisA);
//	hingeC->setLimit(btScalar(1), btScalar(-1));
//	joint = hingeC;
//	world_->addConstraint(joint, true);
/*
	//大きさ
	btVector3 ground_extents = btVector3(200, 0.00001, 200);
	double a = 200.0, b = 100.0, c = 1.0;
	btVector3 cube_extents = btVector3(a, b, c);
	btVector3 cube_extents2 = btVector3(c, b, a);
	btVector3 cube_extents3 = btVector3(3, 3, 3);



	//質量
	btScalar ground_mass = 0.0;
	btScalar cube_mass = 0.0;
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
	btCollisionShape *cube_shape2 = new btBoxShape(cube_extents2);
	btCollisionShape *cube_shape3 = new btBoxShape(cube_extents3);


	//初期位置姿勢
	btQuaternion qrot2(0, 0, 0, 1);
	btDefaultMotionState* ground_motion_state = new btDefaultMotionState(btTransform(qrot2, ground_pos));
	btDefaultMotionState* cube_motion_state = new btDefaultMotionState(btTransform(qrot2, cube_pos));
	btDefaultMotionState* cube_motion_state2 = new btDefaultMotionState(btTransform(qrot2, cube_pos2));
	btDefaultMotionState* cube_motion_state3 = new btDefaultMotionState(btTransform(qrot2, cube_pos3));
	btDefaultMotionState* cube_motion_state4 = new btDefaultMotionState(btTransform(qrot2, cube_pos4));
	btDefaultMotionState* cube_motion_state5 = new btDefaultMotionState(btTransform(qrot2, cube_pos5));

	//慣性モーメントの計算
	ground_shape->calculateLocalInertia(ground_mass, ground_inertia);
	cube_shape->calculateLocalInertia(cube_mass, cube_inertia);
	//剛体オブジェクト生成
	ground_body = new btRigidBody(ground_mass, ground_motion_state, ground_shape, ground_inertia);
	cube_body_ = new btRigidBody(cube_mass, cube_motion_state, cube_shape, cube_inertia);
	cube_body2_ = new btRigidBody(cube_mass, cube_motion_state2, cube_shape, cube_inertia);
	cube_body3_ = new btRigidBody(cube_mass, cube_motion_state3, cube_shape2, cube_inertia);
	cube_body4_ = new btRigidBody(cube_mass, cube_motion_state4, cube_shape2, cube_inertia);
	cube_body5_ = new btRigidBody(cube_mass, cube_motion_state5, cube_shape3, cube_inertia);

	//反発係数
	ground_body->setRestitution(ground_rest);
	cube_body_->setRestitution(cube_rest);
	cube_body2_->setRestitution(cube_rest);
	cube_body3_->setRestitution(cube_rest);
	cube_body4_->setRestitution(cube_rest);
	cube_body5_->setRestitution(cube_rest);

	//ワールドに剛体オブジェクトを追加

	world_->addRigidBody(ground_body);
	world_->addRigidBody(cube_body_);
	world_->addRigidBody(cube_body2_);
	world_->addRigidBody(cube_body3_);
	world_->addRigidBody(cube_body4_);
	world_->addRigidBody(cube_body5_);
*/
	int t;
	t = 25;
	btVector3 posision_q(t * cos(PI_ / 6), 10, t * sin(PI_ / 6));
	CreateSpider(posision_q);
	btVector3 posision_w(t * cos(PI_ * 2 / 6), 10, t * sin(PI_ * 2 / 6));
	CreateSpider(posision_w);
	btVector3 posision_r(t * cos(PI_ * 3/ 6), 10, t * sin(PI_ * 3 / 6));
	CreateSpider(posision_r);
	btVector3 posision_t(t * cos(PI_ * 4 / 6), 10, t * sin(PI_ * 4 / 6));
	CreateSpider(posision_t);
	btVector3 posision_y(t * cos(PI_ * 5 / 6), 10, t * sin(PI_ * 5 / 6));
	CreateSpider(posision_y);
	btVector3 posision_u(t * cos(PI_ * 6 / 6), 10, t * sin(PI_ * 6 / 6));
	CreateSpider(posision_u);
	btVector3 posision_i(t * cos(PI_ * 7 / 6), 10, t * sin(PI_ * 7 / 6));
	CreateSpider(posision_i);
	btVector3 posision_o(t * cos(PI_ * 8 / 6), 10, t * sin(PI_ * 8 / 6));
	CreateSpider(posision_o);
	btVector3 posision_p(t * cos(PI_ * 9 / 6), 10, t * sin(PI_ * 9 / 6));
	CreateSpider(posision_p);
	btVector3 posision_l(t * cos(PI_ * 10 / 6), 10, t * sin(PI_ * 10 / 6));
	CreateSpider(posision_l);
	btVector3 posision_k(t * cos(PI_ * 11 / 6), 10, t * sin(PI_ * 11 / 6));
	CreateSpider(posision_k);
	btVector3 posision_j(t * cos(PI_ * 12 / 6), 10, t * sin(PI_ * 12 / 6));
	CreateSpider(posision_j);


//
//	btVector3 position_a(10, 13, 25);
//	CreateSnowman(position_a, 2);
//
//	btVector3 position_b(10, 2.5, 40);
//	CreatePyramid(position_b);
//
//	btVector3 position_c(10, 20, 70);
//	CreateTriangle(position_c);

	//タワー
	btVector3 position_d(40, 10*sqrt(24)+2.0, 150);
	CreateTower(position_d);
	btVector3 position_e(-40, 10*sqrt(24)+2.0, -150);
	CreateTower(position_e);

	object_[num_] = ++object_num_;
	color_[num_++] = btVector3(1.0, 0, 0);

	//描画
	m_shapeDrawer = new GL_ShapeDrawer ();
	m_shapeDrawer->enableTexture(true);

}

//デストラクタ
StageMap::~StageMap()
{
	int i;

	//オブジェクトの破棄
	for (i = world_->getNumCollisionObjects() - 2; i >= 0; i--)
	{
		btCollisionObject* obj = world_->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		world_->removeCollisionObject( obj );
		delete obj;
	}
}

//更新
void StageMap::Update(){

}

//描画
void StageMap::Draw(){

//	glClear(GL_STENCIL_BUFFER_BIT);
//	glEnable(GL_CULL_FACE);
//	RenderScene(0);
//
	glDisable(GL_LIGHTING);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);
	//glEnable(GL_STENCIL_TEST);
	glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
	glStencilFunc(GL_ALWAYS,1,0xFFFFFFFFL);
	glFrontFace(GL_CCW);
	glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);
	RenderScene(1);
	glFrontFace(GL_CW);
	glStencilOp(GL_KEEP,GL_KEEP,GL_DECR);
	RenderScene(1);
	glFrontFace(GL_CCW);


	glPolygonMode(GL_FRONT,GL_FILL);
	glPolygonMode(GL_BACK,GL_FILL);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_LIGHTING);
	glDepthMask(GL_TRUE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);

	glDepthFunc(GL_LEQUAL);
	glStencilFunc( GL_NOTEQUAL, 0, 0xFFFFFFFFL );
	glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
	glDisable(GL_LIGHTING);

	RenderScene(2);

	glEnable(GL_LIGHTING);
	glDepthFunc(GL_LESS);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_CULL_FACE);

	glFlush();

}

void	StageMap::RenderScene(int pass)
{
	btScalar	m[16];
	btMatrix3x3	rot;rot.setIdentity();
	const int	numObjects=world_->getNumCollisionObjects();
	btVector3 wireColor(1,0,0);
	for(int i=0;i<numObjects;i++)
	{
		btCollisionObject*	colObj=world_->getCollisionObjectArray()[i];
		btRigidBody*		body=btRigidBody::upcast(colObj);
		if(body&&body->getMotionState())
		{
			btDefaultMotionState* myMotionState = (btDefaultMotionState*)body->getMotionState();
			myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(m);
			rot=myMotionState->m_graphicsWorldTrans.getBasis();
		}
		else
		{
			colObj->getWorldTransform().getOpenGLMatrix(m);
			rot=colObj->getWorldTransform().getBasis();
		}

		wireColor = color_[i];

		btVector3 aabbMin,aabbMax;
		world_->getBroadphase()->getBroadphaseAabb(aabbMin,aabbMax);

		aabbMin-=btVector3(BT_LARGE_FLOAT,BT_LARGE_FLOAT,BT_LARGE_FLOAT);
		aabbMax+=btVector3(BT_LARGE_FLOAT,BT_LARGE_FLOAT,BT_LARGE_FLOAT);
		//		printf("aabbMin=(%f,%f,%f)\n",aabbMin.getX(),aabbMin.getY(),aabbMin.getZ());
		//		printf("aabbMax=(%f,%f,%f)\n",aabbMax.getX(),aabbMax.getY(),aabbMax.getZ());
		//		m_dynamicsWorld->getDebugDrawer()->drawAabb(aabbMin,aabbMax,btVector3(1,1,1));

		switch(pass)
		{
			case	1:	m_shapeDrawer->drawShadow(m,m_sundirection*rot,colObj->getCollisionShape(),aabbMin,aabbMax);break;
			case	2:	m_shapeDrawer->drawOpenGL(m,colObj->getCollisionShape(),wireColor*btScalar(0.3),0,aabbMin,aabbMax);break;
		}
	}
}

//bulletに登録
btRigidBody* StageMap::LocalCreateRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape)
{
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0,0,0);
	if (isDynamic)
		shape->calculateLocalInertia(mass,localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,shape,localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	world_->addRigidBody(body);

	return body;
}

void StageMap::CreateSpider(const btVector3& position)
{

	btRigidBody* m_bodies[13];
	btCollisionShape* m_shapes[13];
	btTypedConstraint* m_joints[12];
	btVector3 vUP(0, 1, 0);

	float fBodySize =0.75f;
	float fLegLength = 2.45f;
	float fForeLegLength = 2.75f;

	//形状設定
	m_shapes[0] = new btCapsuleShape(btScalar(fBodySize), btScalar(0.10));
	int i;
	for(i = 0; i < 6; i++)
	{
		m_shapes[1 + 2*i] = new btCapsuleShape(btScalar(0.10), btScalar(fLegLength));
		m_shapes[2 + 2*i] = new btCapsuleShape(btScalar(0.08), btScalar(fForeLegLength));
	}
	float fHeight = 0.5;
	btTransform offset; offset.setIdentity();
	offset.setOrigin(position);
	btVector3 vRoot = btVector3(btScalar(0.), btScalar(fHeight), btScalar(0.));
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(vRoot);

	//bulletに登録
	m_bodies[0] = LocalCreateRigidBody(btScalar(0.1), offset*transform, m_shapes[0]);
	object_[num_] = ++object_num_;
	color_[num_++] = btVector3(0.6, 0.35, 0);

	for(i=0; i < 6; i++)
	{
		float fAngle = 2 * 3.14 * i / 6;
		float fSin = sin(fAngle);
		float fCos = cos(fAngle);

		transform.setIdentity();
		btVector3 vBoneOrigin = btVector3(btScalar(fCos*(fBodySize+0.5*fLegLength)), btScalar(fHeight), btScalar(fSin*(fBodySize+0.5*fLegLength)));
		transform.setOrigin(vBoneOrigin);

		btVector3 vToBone = (vBoneOrigin - vRoot).normalize();
		btVector3 vAxis = vToBone.cross(vUP);
		transform.setRotation(btQuaternion(vAxis, 1.57));
		m_bodies[1+2*i] = LocalCreateRigidBody(btScalar(0.1), offset*transform, m_shapes[2+2*i]);
		object_[num_] = object_num_;
		color_[num_++] = btVector3(0.2, 0.4, 0.4);

		transform.setIdentity();
		transform.setOrigin(btVector3(btScalar(fCos*(fBodySize+fLegLength)), btScalar(fHeight-0.5*fForeLegLength), btScalar(fSin*(fBodySize+fLegLength))));
		m_bodies[2+2*i] = LocalCreateRigidBody(btScalar(0.1), offset*transform, m_shapes[2+2*i]);
		object_[num_] = object_num_;
		color_[num_++] = btVector3(0.8, 0.6, 0.2);
	}

	for(i = 0; i < 6; ++i)
	{
		m_bodies[i]->setDamping(0.05, 0.85);
		m_bodies[i]->setDeactivationTime(0.8);
		m_bodies[i]->setSleepingThresholds(0.5f, 0.5f);
	}

	btHingeConstraint* hingeC;
	btTransform localA, localB, localC;

	for(i=0; i < 6; i++)
	{
		float fAngle = 2 * 3.14 * i / 6;
		float fSin = sin(fAngle);
		float fCos = cos(fAngle);

		localA.setIdentity(); localB.setIdentity();
		localA.getBasis().setEulerZYX(0, -fAngle, 0); localA.setOrigin(btVector3(btScalar(fCos*fBodySize), btScalar(0.), btScalar(fSin*fBodySize)));
		localB = m_bodies[1+2*i]->getWorldTransform().inverse() * m_bodies[0]->getWorldTransform() * localA;
		hingeC = new btHingeConstraint(*m_bodies[0], *m_bodies[1+2*i], localA, localB);
		hingeC->setLimit(btScalar(-0.75 * 0.78), btScalar(0.39));
		m_joints[2*i] = hingeC;
		world_->addConstraint(m_joints[2*i], true);

		localA.setIdentity(); localB.setIdentity(); localC.setIdentity();
		localA.getBasis().setEulerZYX(0, -fAngle, 0); localA.setOrigin(btVector3(btScalar(fCos*(fBodySize+fLegLength)), btScalar(0.), btScalar(fSin*(fBodySize+fLegLength))));
		localB = m_bodies[1+2*i]->getWorldTransform().inverse() * m_bodies[0]->getWorldTransform() * localA;
		localC = m_bodies[2+2*i]->getWorldTransform().inverse() * m_bodies[0]->getWorldTransform() * localA;
		hingeC = new btHingeConstraint(*m_bodies[1+2*i], *m_bodies[2+2*i], localB, localC);
		hingeC->setLimit(btScalar(-0.39), btScalar(0.2));
		m_joints[1+2*i] = hingeC;
		world_->addConstraint(m_joints[1+2*i], true);
	}

}


void StageMap::CreateSnowman(const btVector3& position, double size)
{
	btRigidBody* snow_bodies[3];
	btCollisionShape* snow_shapes[3];
	btTypedConstraint* snow_joints[2];
	btVector3 snow_position2(0, size*1.5, 0);
	btVector3 snow_position3(0, size*0.9, 0);

	//オブジェクト生成
	snow_shapes[0] = new btCapsuleShape(btScalar(size), btScalar(0.1));
	btTransform offset; offset.setIdentity();
	offset.setOrigin(position);
	snow_bodies[0] = LocalCreateRigidBody(btScalar(0.01), offset, snow_shapes[0]);
	object_[num_] = ++object_num_;
	color_[num_++] = btVector3(0.6, 0.8, 1.0);
	btTransform offset2; offset2.setIdentity();
	offset2.setOrigin(snow_position2);
	snow_shapes[1] = new btCapsuleShape(btScalar(size/2), btScalar(0.1));
	snow_bodies[1] = LocalCreateRigidBody(btScalar(0.01), offset*offset2, snow_shapes[1]);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.6, 0.8, 0.8);
	btTransform offset3; offset3.setIdentity();
	offset3.setOrigin(snow_position3);
	snow_shapes[2] = new btCapsuleShape(btScalar(size/3), btScalar(0.1));
	snow_bodies[2] = LocalCreateRigidBody(btScalar(0.01), offset*offset3, snow_shapes[2]);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(1.0, 1.0, 1.0);

	btHingeConstraint* hingeCC;
	btTransform localA, localB, localC;
	localA.setIdentity(); localB.setIdentity();
	//	localA.getBasis().setEulerZYX(0, 0, 0); localA.setOrigin(btVector3(btScalar(6), btScalar(6), btScalar(6)));
	localB = snow_bodies[0]->getWorldTransform().inverse() * snow_bodies[1]->getWorldTransform() * localA;

	hingeCC = new btHingeConstraint(*snow_bodies[0], *snow_bodies[1], localB, localA);
	hingeCC->setLimit(btScalar(0), btScalar(0));
	snow_joints[0] = hingeCC;
	world_->addConstraint(snow_joints[0], true);

	localC = snow_bodies[0]->getWorldTransform().inverse() * snow_bodies[2]->getWorldTransform() * localA;

	hingeCC = new btHingeConstraint(*snow_bodies[1], *snow_bodies[2], localC, localA);
	hingeCC->setLimit(btScalar(0), btScalar(0));
	snow_joints[1] = hingeCC;
	world_->addConstraint(snow_joints[1], true);

}

void StageMap::CreatePyramid(const btVector3& position)
{

	btRigidBody* box_body[14];
	float cube_size = 1.1;
	float cube_mass = 0.01;
	btTypedConstraint* joint;
	btVector3 position1(cube_size*2, cube_size*2, cube_size*2);
	btVector3 position2(cube_size*4, 0, 0);
	btVector3 position3(0, 0, cube_size*4);
	btVector3 position4(cube_size*4, 0, cube_size*4);
	btVector3 position5(cube_size*6, cube_size*2, cube_size*2);
	btVector3 position6(cube_size*8, 0, 0);
	btVector3 position7(cube_size*8, 0, cube_size*4);
	btVector3 position8(cube_size*2, cube_size*2, cube_size*6);
	btVector3 position9(0, 0, cube_size*8);
	btVector3 position10(cube_size*4 ,0, cube_size*8);
	btVector3 position11(cube_size*6, cube_size*2, cube_size*6);
	btVector3 position12(cube_size*8, 0, cube_size*8);
	btVector3 position13(cube_size*4, cube_size*4,cube_size*4);
	btCollisionShape *shape = new btBoxShape(btVector3(cube_size, cube_size, cube_size));

	btTransform offset; offset.setIdentity();
	btTransform offset2; offset2.setIdentity();
	offset2.setOrigin(position1);
	offset.setOrigin(position);

	//cubeをbulletに登録
	box_body[0] = LocalCreateRigidBody(btScalar(cube_mass), offset, shape);
	object_[num_] = ++object_num_;
	color_[num_++] = btVector3(0.71, 0, 0.48);
	box_body[1] = LocalCreateRigidBody(btScalar(cube_mass), offset*offset2, shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0, 0.6, 0.52);
	offset2.setOrigin(position2);
	box_body[2] = LocalCreateRigidBody(btScalar(cube_mass), offset*offset2, shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.22, 0.13, 0.52);
	offset2.setOrigin(position3);
	box_body[3] = LocalCreateRigidBody(btScalar(cube_mass), offset*offset2, shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.88, 0.78, 0.18);
	offset2.setOrigin(position4);
	box_body[4] = LocalCreateRigidBody(btScalar(cube_mass), offset*offset2, shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0., 0.61, 0.77);
	offset2.setOrigin(position5);
	box_body[5] = LocalCreateRigidBody(btScalar(cube_mass), offset*offset2, shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.77, 0.78, 0.16);
	offset2.setOrigin(position6);
	box_body[6] = LocalCreateRigidBody(btScalar(cube_mass), offset*offset2, shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.58, 0, 0.49);
	offset2.setOrigin(position7);
	box_body[7] = LocalCreateRigidBody(btScalar(cube_mass), offset*offset2, shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.84, 0.52, 0.69);
	offset2.setOrigin(position8);
	box_body[8] = LocalCreateRigidBody(btScalar(cube_mass), offset*offset2, shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.93, 0.4, 0.9);
	offset2.setOrigin(position9);
	box_body[9] = LocalCreateRigidBody(btScalar(cube_mass), offset*offset2, shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.87, 0.31, 0.08);
	offset2.setOrigin(position10);
	box_body[10] = LocalCreateRigidBody(btScalar(cube_mass), offset*offset2, shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.72, 0.05, 0.23);
	offset2.setOrigin(position11);
	box_body[11] = LocalCreateRigidBody(btScalar(cube_mass), offset*offset2, shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0, 0.47, 0.73);
	offset2.setOrigin(position12);
	box_body[12] = LocalCreateRigidBody(btScalar(cube_mass), offset*offset2, shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.62, 0.76, 0.22);
	offset2.setOrigin(position13);
	box_body[13] = LocalCreateRigidBody(btScalar(cube_mass), offset*offset2, shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.8, 0.32, 0.57);

	//cubeを繋げる
	btHingeConstraint* hingeC;
	btTransform localA, localB, localC;

	localA.setIdentity(); localB.setIdentity();
	localB = box_body[0]->getWorldTransform().inverse() * box_body[1]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[0], *box_body[1], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body[1]->getWorldTransform().inverse() * box_body[2]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[1], *box_body[2], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body[1]->getWorldTransform().inverse() * box_body[3]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[1], *box_body[3], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body[1]->getWorldTransform().inverse() * box_body[4]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[1], *box_body[4], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body[2]->getWorldTransform().inverse() * box_body[5]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[2], *box_body[5], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body[4]->getWorldTransform().inverse() * box_body[5]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[4], *box_body[5], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body[5]->getWorldTransform().inverse() * box_body[6]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[5], *box_body[6], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body[5]->getWorldTransform().inverse() * box_body[7]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[5], *box_body[7], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body[3]->getWorldTransform().inverse() * box_body[8]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[3], *box_body[8], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body[4]->getWorldTransform().inverse() * box_body[8]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[4], *box_body[8], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body[8]->getWorldTransform().inverse() * box_body[9]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[8], *box_body[9], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body[8]->getWorldTransform().inverse() * box_body[10]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[8], *box_body[10], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body[4]->getWorldTransform().inverse() * box_body[11]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[4], *box_body[11], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body[10]->getWorldTransform().inverse() * box_body[11]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[10], *box_body[11], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body[11]->getWorldTransform().inverse() * box_body[12]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[11], *box_body[12], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body[1]->getWorldTransform().inverse() * box_body[13]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[1], *box_body[13], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body[5]->getWorldTransform().inverse() * box_body[13]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[5], *box_body[13], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body[8]->getWorldTransform().inverse() * box_body[13]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[8], *box_body[13], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body[11]->getWorldTransform().inverse() * box_body[13]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[11], *box_body[13], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);
}

void StageMap::CreateTriangle(const btVector3& position)
{
	btRigidBody* triangle_sphere[4];
	btRigidBody* triangle_sides[6];
	btTypedConstraint* joints_triangel;
	float sphere_size = 0.5;
	float side_size = 5;
	btTransform trans;
	btVector3 position1(side_size/sqrt(3), -side_size*sqrt(3)/2, 0);
	btVector3 position2(-side_size/(2*sqrt(3)), -side_size*sqrt(3)/2, side_size/2);
	btVector3 position3(-side_size/(2*sqrt(3)), -side_size*sqrt(3)/2, -side_size/2);
	btVector3 position4(side_size/sqrt(3)/2, -side_size*sqrt(3)/4, 0);
	btVector3 position5(-side_size/(4*sqrt(3)), -side_size*sqrt(3)/4, side_size/4);
	btVector3 position6(-side_size/(4*sqrt(3)), -side_size*sqrt(3)/4, -side_size/4);
	btVector3 position7(-side_size/(2*sqrt(3)), -side_size*sqrt(3)/2, 0);
	btVector3 position8(side_size/(4*sqrt(3)), -side_size*sqrt(3)/2, side_size/4);
	btVector3 position9(side_size/(4*sqrt(3)), -side_size*sqrt(3)/2, -side_size/4);

	btCollisionShape *sphere_shape = new btSphereShape(sphere_size);
	btCollisionShape *side_shape = new btCapsuleShape(btScalar(0.1), btScalar(side_size));

	btTransform offset; offset.setIdentity();
	btTransform offset2; offset2.setIdentity();
	offset2.setOrigin(position1);
	offset.setOrigin(position);

	//cubeをbulletに登録
	triangle_sphere[0] = LocalCreateRigidBody(btScalar(0.01), offset, sphere_shape);
	object_[num_] = ++object_num_;
	color_[num_++] = btVector3(0.73, 0, 0);
	triangle_sphere[1] = LocalCreateRigidBody(btScalar(0.01), offset*offset2, sphere_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(1.0, 0.8, 0);
	offset2.setIdentity(); offset2.setOrigin(position2);
	triangle_sphere[2] = LocalCreateRigidBody(btScalar(0.01), offset*offset2, sphere_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0, 0, 0.5);
	offset2.setIdentity(); offset2.setOrigin(position3);
	triangle_sphere[3] = LocalCreateRigidBody(btScalar(0.01), offset*offset2, sphere_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.4, 0.65, 0.9);

	//ここからside
	offset2.setIdentity(); offset2.setOrigin(position4);
	offset2.setRotation(btQuaternion(0,0,PI_/6));
	triangle_sides[0] = LocalCreateRigidBody(btScalar(0.01), offset*offset2, side_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.9, 0.84, 0.71);
	offset2.setIdentity(); offset2.setOrigin(position5);
	offset2.setRotation(btQuaternion(0, -PI_/6, -PI_/12));
	triangle_sides[1] = LocalCreateRigidBody(btScalar(0.01), offset*offset2, side_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.9, 0.84, 0.71);
	offset2.setIdentity(); offset2.setOrigin(position6);
	offset2.setRotation(btQuaternion(0, PI_/6, -PI_/12));
	triangle_sides[2] = LocalCreateRigidBody(btScalar(0.01), offset*offset2, side_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.9, 0.84, 0.71);
	offset2.setIdentity(); offset2.setOrigin(position7);
	offset2.setRotation(btQuaternion(0, PI_/2, 0));
	triangle_sides[3] = LocalCreateRigidBody(btScalar(0.01), offset*offset2, side_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.9, 0.84, 0.71);
	offset2.setIdentity(); offset2.setOrigin(position8);
	offset2.setRotation(btQuaternion(0, PI_/2, PI_/3));
	triangle_sides[4] = LocalCreateRigidBody(btScalar(0.01), offset*offset2, side_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.9, 0.84, 0.71);
	offset2.setIdentity(); offset2.setOrigin(position9);
	offset2.setRotation(btQuaternion(0, PI_/2, -PI_/3));
	triangle_sides[5] = LocalCreateRigidBody(btScalar(0.01), offset*offset2, side_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0.9, 0.84, 0.71);

	btHingeConstraint* hingeC;
	btTransform localA, localB, localC;

	localA.setIdentity(); localB.setIdentity();
	localB = triangle_sphere[0]->getWorldTransform().inverse() * triangle_sides[0]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_sphere[0], *triangle_sides[0], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);

	localA.setIdentity(); localB.setIdentity();
	localB = triangle_sphere[0]->getWorldTransform().inverse() * triangle_sides[1]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_sphere[0], *triangle_sides[1], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);

	localA.setIdentity(); localB.setIdentity();
	localB = triangle_sphere[0]->getWorldTransform().inverse() * triangle_sides[2]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_sphere[0], *triangle_sides[2], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);

	localA.setIdentity(); localB.setIdentity();
	localB = triangle_sphere[1]->getWorldTransform().inverse() * triangle_sides[0]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_sphere[1], *triangle_sides[0], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);

	localA.setIdentity(); localB.setIdentity();
	localB = triangle_sphere[1]->getWorldTransform().inverse() * triangle_sides[4]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_sphere[1], *triangle_sides[4], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);

	localA.setIdentity(); localB.setIdentity();
	localB = triangle_sphere[1]->getWorldTransform().inverse() * triangle_sides[5]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_sphere[1], *triangle_sides[5], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);

	localA.setIdentity(); localB.setIdentity();
	localB = triangle_sphere[2]->getWorldTransform().inverse() * triangle_sides[1]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_sphere[2], *triangle_sides[1], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);

	localA.setIdentity(); localB.setIdentity();
	localB = triangle_sphere[2]->getWorldTransform().inverse() * triangle_sides[4]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_sphere[2], *triangle_sides[4], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);

	localA.setIdentity(); localB.setIdentity();
	localB = triangle_sphere[2]->getWorldTransform().inverse() * triangle_sides[3]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_sphere[2], *triangle_sides[3], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);

	localA.setIdentity(); localB.setIdentity();
	localB = triangle_sphere[3]->getWorldTransform().inverse() * triangle_sides[2]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_sphere[3], *triangle_sides[2], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);

	localA.setIdentity(); localB.setIdentity();
	localB = triangle_sphere[3]->getWorldTransform().inverse() * triangle_sides[5]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_sphere[3], *triangle_sides[5], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);

	localA.setIdentity(); localB.setIdentity();
	localB = triangle_sphere[3]->getWorldTransform().inverse() * triangle_sides[3]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_sphere[3], *triangle_sides[3], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);
}

void StageMap::CreateTower(const btVector3& position)
  {
	btRigidBody* tower_sphere[13];
	btRigidBody* tower_sides[18];
//	btTypedConstraint* joints_triangel;
	float sphere_size = 2;
	float side_size = 10;
	btTransform trans;
	//各オブジェクトの位置を決める
	btVector3 position1(side_size, -side_size*sqrt(24), 0);
	btVector3 position2(-side_size, -side_size*sqrt(24), 0);
	btVector3 position3(side_size/2, -side_size*sqrt(24), side_size*sqrt(3)/2);
	btVector3 position4(-side_size/2, -side_size*sqrt(24), side_size*sqrt(3)/2);
	btVector3 position5(side_size/2, -side_size*sqrt(24), -side_size*sqrt(3)/2);
	btVector3 position6(-side_size/2, -side_size*sqrt(24), -side_size*sqrt(3)/2);
	btVector3 position7(0, -side_size*sqrt(24), side_size*sqrt(3));
	btVector3 position8(0, -side_size*sqrt(24), -side_size*sqrt(3));
	btVector3 position9(side_size*3/2, -side_size*sqrt(24), side_size*sqrt(3)/2);
	btVector3 position10(-side_size*3/2, -side_size*sqrt(24), side_size*sqrt(3)/2);
	btVector3 position11(side_size*3/2, -side_size*sqrt(24), -side_size*sqrt(3)/2);
	btVector3 position12(-side_size*3/2, -side_size*sqrt(24), -side_size*sqrt(3)/2);
	btVector3 position13(0, -side_size*sqrt(24), side_size*sqrt(3)/2);
	btVector3 position14(0, -side_size*sqrt(24), -side_size*sqrt(3)/2);
	btVector3 position15(side_size*3/4, -side_size*sqrt(24), side_size*sqrt(3)/4);
	btVector3 position16(-side_size*3/4, -side_size*sqrt(24), side_size*sqrt(3)/4);
	btVector3 position17(side_size*3/4, -side_size*sqrt(24), -side_size*sqrt(3)/4);
	btVector3 position18(-side_size*3/4, -side_size*sqrt(24), -side_size*sqrt(3)/4);

	//形状設定
	btCollisionShape *sphere_shape = new btSphereShape(sphere_size);
	btCollisionShape *side_shape = new btCapsuleShape(btScalar(1.6), btScalar(side_size*5));
	btCollisionShape *side_shape2 = new btCapsuleShape(btScalar(1.0), btScalar(side_size*3));

	btTransform offset; offset.setIdentity();
	btTransform offset2; offset2.setIdentity();
	offset2.setOrigin(position1);
	offset.setOrigin(position);

	//cubeをbulletに登録
	tower_sphere[0] = LocalCreateRigidBody(btScalar(0.), offset, sphere_shape);
	object_[num_] = ++object_num_;
	color_[num_++] = btVector3(0, 0, 0.5);
	tower_sphere[1] = LocalCreateRigidBody(btScalar(0.), offset*offset2, sphere_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0, 0, 0.5);
	offset2.setIdentity(); offset2.setOrigin(position2);
	tower_sphere[2] = LocalCreateRigidBody(btScalar(0.), offset*offset2, sphere_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0, 0, 0.5);
	offset2.setIdentity(); offset2.setOrigin(position3);
	tower_sphere[3] = LocalCreateRigidBody(btScalar(0.), offset*offset2, sphere_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0, 0, 0.5);
	offset2.setIdentity(); offset2.setOrigin(position4);
	tower_sphere[4] = LocalCreateRigidBody(btScalar(0.), offset*offset2, sphere_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0, 0, 0.5);
	offset2.setIdentity(); offset2.setOrigin(position5);
	tower_sphere[5] = LocalCreateRigidBody(btScalar(0.), offset*offset2, sphere_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0, 0, 0.5);
	offset2.setIdentity(); offset2.setOrigin(position6);
	tower_sphere[6] = LocalCreateRigidBody(btScalar(0.), offset*offset2, sphere_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0, 0, 0.5);
	offset2.setIdentity(); offset2.setOrigin(position7);
	tower_sphere[7] = LocalCreateRigidBody(btScalar(0.), offset*offset2, sphere_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0, 0, 0.5);
	offset2.setIdentity(); offset2.setOrigin(position8);
	tower_sphere[8] = LocalCreateRigidBody(btScalar(0.), offset*offset2, sphere_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0, 0, 0.5);
	offset2.setIdentity(); offset2.setOrigin(position9);
	tower_sphere[9] = LocalCreateRigidBody(btScalar(0.), offset*offset2, sphere_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0, 0, 0.5);
	offset2.setIdentity(); offset2.setOrigin(position10);
	tower_sphere[10] = LocalCreateRigidBody(btScalar(0.), offset*offset2, sphere_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0, 0, 0.5);
	offset2.setIdentity(); offset2.setOrigin(position11);
	tower_sphere[11] = LocalCreateRigidBody(btScalar(0.), offset*offset2, sphere_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0, 0, 0.5);
	offset2.setIdentity(); offset2.setOrigin(position12);
	tower_sphere[12] = LocalCreateRigidBody(btScalar(0.), offset*offset2, sphere_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0, 0, 0.5);
	//sideをbulletに登録
	offset2.setIdentity(); offset2.setOrigin(position1/2);
	offset2.setRotation(btQuaternion(0, 0, 12 * PI_/ 180.0));
	tower_sides[0] = LocalCreateRigidBody(btScalar(0.), offset*offset2, side_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0, 1, 0.8);
	offset2.setIdentity(); offset2.setOrigin(position2/2);
	offset2.setRotation(btQuaternion(0, 0, -12 * PI_/ 180.0));
	tower_sides[1] = LocalCreateRigidBody(btScalar(0.), offset*offset2, side_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0, 1, 0.8);
	offset2.setIdentity(); offset2.setOrigin(position3/2);
	offset2.setRotation(btQuaternion(0, -10 * PI_ / 180.0, 6 * PI_/ 180.0));
	tower_sides[2] = LocalCreateRigidBody(btScalar(0.), offset*offset2, side_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0, 1, 0.8);
	offset2.setIdentity(); offset2.setOrigin(position4/2);
	offset2.setRotation(btQuaternion(0, -10 * PI_ / 180.0, -6 * PI_/ 180.0));
	tower_sides[3] = LocalCreateRigidBody(btScalar(0.), offset*offset2, side_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0, 1, 0.8);
	offset2.setIdentity(); offset2.setOrigin(position5/2);
	offset2.setRotation(btQuaternion(0, 10 * PI_ / 180.0, 6 * PI_/ 180.0));
	tower_sides[4] = LocalCreateRigidBody(btScalar(0.), offset*offset2, side_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0, 1, 0.8);
	offset2.setIdentity(); offset2.setOrigin(position6/2);
	offset2.setRotation(btQuaternion(0, 10 * PI_ / 180.0, -6 * PI_/ 180.0));
	tower_sides[5] = LocalCreateRigidBody(btScalar(0.), offset*offset2, side_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0, 1, 0.8);
	offset2.setIdentity(); offset2.setOrigin(position7/2);
	offset2.setRotation(btQuaternion(0, -20 * PI_ / 180.0, 0));
	tower_sides[6] = LocalCreateRigidBody(btScalar(0.), offset*offset2, side_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0, 0.8, 1);
	offset2.setIdentity(); offset2.setOrigin(position8/2);
	offset2.setRotation(btQuaternion(0, 20 * PI_ / 180.0, 0));
	tower_sides[7] = LocalCreateRigidBody(btScalar(0.), offset*offset2, side_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0, 0.8, 1);
	offset2.setIdentity(); offset2.setOrigin(position9/2);
	offset2.setRotation(btQuaternion(0, -10 * PI_ / 180.0, 17 * PI_ / 180.0));
	tower_sides[8] = LocalCreateRigidBody(btScalar(0.), offset*offset2, side_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0, 0.8, 1);
	offset2.setIdentity(); offset2.setOrigin(position10/2);
	offset2.setRotation(btQuaternion(0, -10 * PI_ / 180.0, -17 * PI_ / 180.0));
	tower_sides[9] = LocalCreateRigidBody(btScalar(0.), offset*offset2, side_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0, 0.8, 1);
	offset2.setIdentity(); offset2.setOrigin(position11/2);
	offset2.setRotation(btQuaternion(0, 10 * PI_ / 180.0, 17 * PI_ / 180.0));
	tower_sides[10] = LocalCreateRigidBody(btScalar(0.), offset*offset2, side_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0, 0.8, 1);
	offset2.setIdentity(); offset2.setOrigin(position12/2);
	offset2.setRotation(btQuaternion(0, 10 * PI_ / 180.0, -17 * PI_ / 180.0));
	tower_sides[11] = LocalCreateRigidBody(btScalar(0.), offset*offset2, side_shape);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0, 0.8, 1);
	offset2.setIdentity(); offset2.setOrigin(position13);
	offset2.setRotation(btQuaternion(0, 0, PI_ / 2));
	tower_sides[12] = LocalCreateRigidBody(btScalar(0.), offset*offset2, side_shape2);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0, 0.6, 1.0);
	offset2.setIdentity(); offset2.setOrigin(position14);
	offset2.setRotation(btQuaternion(0, 0.4, PI_ / 2));
	tower_sides[13] = LocalCreateRigidBody(btScalar(0.), offset*offset2, side_shape2);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0, 0.6, 1.0);
	offset2.setIdentity(); offset2.setOrigin(position15);
	offset2.setRotation(btQuaternion(PI_ / 3 , 0, PI_/ 2));
	tower_sides[14] = LocalCreateRigidBody(btScalar(0.), offset*offset2, side_shape2);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0, 0.6, 1.0);
	offset2.setIdentity(); offset2.setOrigin(position16);
	offset2.setRotation(btQuaternion(-PI_ / 3 , 0, PI_/ 2));
	tower_sides[15] = LocalCreateRigidBody(btScalar(0.), offset*offset2, side_shape2);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0, 0.6, 1.0);
	offset2.setIdentity(); offset2.setOrigin(position17);
	offset2.setRotation(btQuaternion(PI_ * 2 /3  , 0, PI_/ 2));
	tower_sides[16] = LocalCreateRigidBody(btScalar(0.), offset*offset2, side_shape2);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0, 0.6, 1.0);
	offset2.setIdentity(); offset2.setOrigin(position18);
	offset2.setRotation(btQuaternion(-PI_ * 2 /3  , 0, PI_/ 2));
	tower_sides[17] = LocalCreateRigidBody(btScalar(0.), offset*offset2, side_shape2);
	object_[num_] = object_num_;
	color_[num_++] = btVector3(0, 0.6, 1.0);


	btHingeConstraint* hingeC;
	btTransform localA, localB, localC;
	btTypedConstraint* joints_tower;

	localA.setIdentity(); localB.setIdentity();
	localB = tower_sphere[0]->getWorldTransform().inverse() * tower_sides[0]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*tower_sphere[0], *tower_sides[0], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_tower = hingeC;
	world_->addConstraint(joints_tower, true);

	localA.setIdentity(); localB.setIdentity();
	localB = tower_sphere[0]->getWorldTransform().inverse() * tower_sides[1]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*tower_sphere[0], *tower_sides[1], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_tower = hingeC;
	world_->addConstraint(joints_tower, true);

	localA.setIdentity(); localB.setIdentity();
	localB = tower_sphere[0]->getWorldTransform().inverse() * tower_sides[1]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*tower_sphere[0], *tower_sides[1], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_tower = hingeC;
	world_->addConstraint(joints_tower, true);

	localA.setIdentity(); localB.setIdentity();
	localB = tower_sphere[0]->getWorldTransform().inverse() * tower_sides[1]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*tower_sphere[0], *tower_sides[1], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_tower = hingeC;
	world_->addConstraint(joints_tower, true);

	localA.setIdentity(); localB.setIdentity();
	localB = tower_sphere[0]->getWorldTransform().inverse() * tower_sides[0]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*tower_sphere[0], *tower_sides[0], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_tower = hingeC;
	world_->addConstraint(joints_tower, true);

	localA.setIdentity(); localB.setIdentity();
	localB = tower_sphere[0]->getWorldTransform().inverse() * tower_sides[1]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*tower_sphere[0], *tower_sides[1], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_tower = hingeC;
	world_->addConstraint(joints_tower, true);

	localA.setIdentity(); localB.setIdentity();
	localB = tower_sphere[0]->getWorldTransform().inverse() * tower_sides[1]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*tower_sphere[0], *tower_sides[1], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_tower = hingeC;
	world_->addConstraint(joints_tower, true);

	localA.setIdentity(); localB.setIdentity();
	localB = tower_sphere[0]->getWorldTransform().inverse() * tower_sides[1]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*tower_sphere[0], *tower_sides[1], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_tower = hingeC;
	world_->addConstraint(joints_tower, true);

	localA.setIdentity(); localB.setIdentity();
	localB = tower_sphere[0]->getWorldTransform().inverse() * tower_sides[0]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*tower_sphere[0], *tower_sides[0], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_tower = hingeC;
	world_->addConstraint(joints_tower, true);

	localA.setIdentity(); localB.setIdentity();
	localB = tower_sphere[0]->getWorldTransform().inverse() * tower_sides[1]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*tower_sphere[0], *tower_sides[1], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_tower = hingeC;
	world_->addConstraint(joints_tower, true);

	localA.setIdentity(); localB.setIdentity();
	localB = tower_sphere[0]->getWorldTransform().inverse() * tower_sides[1]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*tower_sphere[0], *tower_sides[1], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_tower = hingeC;
	world_->addConstraint(joints_tower, true);

	localA.setIdentity(); localB.setIdentity();
	localB = tower_sphere[0]->getWorldTransform().inverse() * tower_sides[1]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*tower_sphere[0], *tower_sides[1], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_tower = hingeC;
	world_->addConstraint(joints_tower, true);

	localA.setIdentity(); localB.setIdentity();
	localB = tower_sphere[0]->getWorldTransform().inverse() * tower_sides[0]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*tower_sphere[0], *tower_sides[0], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_tower = hingeC;
	world_->addConstraint(joints_tower, true);

	localA.setIdentity(); localB.setIdentity();
	localB = tower_sphere[0]->getWorldTransform().inverse() * tower_sides[1]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*tower_sphere[0], *tower_sides[1], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_tower = hingeC;
	world_->addConstraint(joints_tower, true);

	localA.setIdentity(); localB.setIdentity();
	localB = tower_sphere[0]->getWorldTransform().inverse() * tower_sides[1]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*tower_sphere[0], *tower_sides[1], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_tower = hingeC;
	world_->addConstraint(joints_tower, true);

	localA.setIdentity(); localB.setIdentity();
	localB = tower_sphere[0]->getWorldTransform().inverse() * tower_sides[1]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*tower_sphere[0], *tower_sides[1], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_tower = hingeC;
	world_->addConstraint(joints_tower, true);

	localA.setIdentity(); localB.setIdentity();
	localB = tower_sphere[0]->getWorldTransform().inverse() * tower_sides[0]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*tower_sphere[0], *tower_sides[0], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_tower = hingeC;
	world_->addConstraint(joints_tower, true);

	localA.setIdentity(); localB.setIdentity();
	localB = tower_sphere[0]->getWorldTransform().inverse() * tower_sides[1]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*tower_sphere[0], *tower_sides[1], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_tower = hingeC;
	world_->addConstraint(joints_tower, true);

	localA.setIdentity(); localB.setIdentity();
	localB = tower_sphere[0]->getWorldTransform().inverse() * tower_sides[1]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*tower_sphere[0], *tower_sides[1], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_tower = hingeC;
	world_->addConstraint(joints_tower, true);

	localA.setIdentity(); localB.setIdentity();
	localB = tower_sphere[0]->getWorldTransform().inverse() * tower_sides[1]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*tower_sphere[0], *tower_sides[1], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_tower = hingeC;
	world_->addConstraint(joints_tower, true);

	localA.setIdentity(); localB.setIdentity();
	localB = tower_sphere[0]->getWorldTransform().inverse() * tower_sides[0]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*tower_sphere[0], *tower_sides[0], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_tower = hingeC;
	world_->addConstraint(joints_tower, true);

	localA.setIdentity(); localB.setIdentity();
	localB = tower_sphere[0]->getWorldTransform().inverse() * tower_sides[1]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*tower_sphere[0], *tower_sides[1], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_tower = hingeC;
	world_->addConstraint(joints_tower, true);

	localA.setIdentity(); localB.setIdentity();
	localB = tower_sphere[0]->getWorldTransform().inverse() * tower_sides[1]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*tower_sphere[0], *tower_sides[1], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_tower = hingeC;
	world_->addConstraint(joints_tower, true);

	localA.setIdentity(); localB.setIdentity();
	localB = tower_sphere[0]->getWorldTransform().inverse() * tower_sides[1]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*tower_sphere[0], *tower_sides[1], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_tower = hingeC;
	world_->addConstraint(joints_tower, true);

	localA.setIdentity(); localB.setIdentity();
	localB = tower_sphere[0]->getWorldTransform().inverse() * tower_sides[0]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*tower_sphere[0], *tower_sides[0], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_tower = hingeC;
	world_->addConstraint(joints_tower, true);

	localA.setIdentity(); localB.setIdentity();
	localB = tower_sphere[0]->getWorldTransform().inverse() * tower_sides[1]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*tower_sphere[0], *tower_sides[1], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_tower = hingeC;
	world_->addConstraint(joints_tower, true);

	localA.setIdentity(); localB.setIdentity();
	localB = tower_sphere[0]->getWorldTransform().inverse() * tower_sides[1]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*tower_sphere[0], *tower_sides[1], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_tower = hingeC;
	world_->addConstraint(joints_tower, true);

	localA.setIdentity(); localB.setIdentity();
	localB = tower_sphere[0]->getWorldTransform().inverse() * tower_sides[1]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*tower_sphere[0], *tower_sides[1], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_tower = hingeC;
	world_->addConstraint(joints_tower, true);

	localA.setIdentity(); localB.setIdentity();
	localB = tower_sphere[0]->getWorldTransform().inverse() * tower_sides[0]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*tower_sphere[0], *tower_sides[0], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_tower = hingeC;
	world_->addConstraint(joints_tower, true);

	localA.setIdentity(); localB.setIdentity();
	localB = tower_sphere[0]->getWorldTransform().inverse() * tower_sides[1]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*tower_sphere[0], *tower_sides[1], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_tower = hingeC;
	world_->addConstraint(joints_tower, true);

	localA.setIdentity(); localB.setIdentity();
	localB = tower_sphere[0]->getWorldTransform().inverse() * tower_sides[1]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*tower_sphere[0], *tower_sides[1], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_tower = hingeC;
	world_->addConstraint(joints_tower, true);

	localA.setIdentity(); localB.setIdentity();
	localB = tower_sphere[0]->getWorldTransform().inverse() * tower_sides[1]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*tower_sphere[0], *tower_sides[1], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_tower = hingeC;
	world_->addConstraint(joints_tower, true);

//	localA.setIdentity(); localB.setIdentity();
//	localB = triangle_sphere[0]->getWorldTransform().inverse() * triangle_sides[1]->getWorldTransform() * localA;
//	hingeC = new btHingeConstraint(*triangle_sphere[0], *triangle_sides[1], localB, localA);
//	hingeC->setLimit(btScalar(0), btScalar(0));
//	joints_triangel = hingeC;
//	world_->addConstraint(joints_triangel, true);
//
//	localA.setIdentity(); localB.setIdentity();
//	localB = triangle_sphere[0]->getWorldTransform().inverse() * triangle_sides[2]->getWorldTransform() * localA;
//	hingeC = new btHingeConstraint(*triangle_sphere[0], *triangle_sides[2], localB, localA);
//	hingeC->setLimit(btScalar(0), btScalar(0));
//	joints_triangel = hingeC;
//	world_->addConstraint(joints_triangel, true);
//
//	localA.setIdentity(); localB.setIdentity();
//	localB = triangle_sphere[1]->getWorldTransform().inverse() * triangle_sides[0]->getWorldTransform() * localA;
//	hingeC = new btHingeConstraint(*triangle_sphere[1], *triangle_sides[0], localB, localA);
//	hingeC->setLimit(btScalar(0), btScalar(0));
//	joints_triangel = hingeC;
//	world_->addConstraint(joints_triangel, true);
//
//	localA.setIdentity(); localB.setIdentity();
//	localB = triangle_sphere[1]->getWorldTransform().inverse() * triangle_sides[4]->getWorldTransform() * localA;
//	hingeC = new btHingeConstraint(*triangle_sphere[1], *triangle_sides[4], localB, localA);
//	hingeC->setLimit(btScalar(0), btScalar(0));
//	joints_triangel = hingeC;
//	world_->addConstraint(joints_triangel, true);
//
//	localA.setIdentity(); localB.setIdentity();
//	localB = triangle_sphere[1]->getWorldTransform().inverse() * triangle_sides[5]->getWorldTransform() * localA;
//	hingeC = new btHingeConstraint(*triangle_sphere[1], *triangle_sides[5], localB, localA);
//	hingeC->setLimit(btScalar(0), btScalar(0));
//	joints_triangel = hingeC;
//	world_->addConstraint(joints_triangel, true);
//
//	localA.setIdentity(); localB.setIdentity();
//	localB = triangle_sphere[2]->getWorldTransform().inverse() * triangle_sides[1]->getWorldTransform() * localA;
//	hingeC = new btHingeConstraint(*triangle_sphere[2], *triangle_sides[1], localB, localA);
//	hingeC->setLimit(btScalar(0), btScalar(0));
//	joints_triangel = hingeC;
//	world_->addConstraint(joints_triangel, true);
//
//	localA.setIdentity(); localB.setIdentity();
//	localB = triangle_sphere[2]->getWorldTransform().inverse() * triangle_sides[4]->getWorldTransform() * localA;
//	hingeC = new btHingeConstraint(*triangle_sphere[2], *triangle_sides[4], localB, localA);
//	hingeC->setLimit(btScalar(0), btScalar(0));
//	joints_triangel = hingeC;
//	world_->addConstraint(joints_triangel, true);
//
//	localA.setIdentity(); localB.setIdentity();
//	localB = triangle_sphere[2]->getWorldTransform().inverse() * triangle_sides[3]->getWorldTransform() * localA;
//	hingeC = new btHingeConstraint(*triangle_sphere[2], *triangle_sides[3], localB, localA);
//	hingeC->setLimit(btScalar(0), btScalar(0));
//	joints_triangel = hingeC;
//	world_->addConstraint(joints_triangel, true);
//
//	localA.setIdentity(); localB.setIdentity();
//	localB = triangle_sphere[3]->getWorldTransform().inverse() * triangle_sides[2]->getWorldTransform() * localA;
//	hingeC = new btHingeConstraint(*triangle_sphere[3], *triangle_sides[2], localB, localA);
//	hingeC->setLimit(btScalar(0), btScalar(0));
//	joints_triangel = hingeC;
//	world_->addConstraint(joints_triangel, true);
//
//	localA.setIdentity(); localB.setIdentity();
//	localB = triangle_sphere[3]->getWorldTransform().inverse() * triangle_sides[5]->getWorldTransform() * localA;
//	hingeC = new btHingeConstraint(*triangle_sphere[3], *triangle_sides[5], localB, localA);
//	hingeC->setLimit(btScalar(0), btScalar(0));
//	joints_triangel = hingeC;
//	world_->addConstraint(joints_triangel, true);
//
//	localA.setIdentity(); localB.setIdentity();
//	localB = triangle_sphere[3]->getWorldTransform().inverse() * triangle_sides[3]->getWorldTransform() * localA;
//	hingeC = new btHingeConstraint(*triangle_sphere[3], *triangle_sides[3], localB, localA);
//	hingeC->setLimit(btScalar(0), btScalar(0));
//	joints_triangel = hingeC;
//	world_->addConstraint(joints_triangel, true);

}

void StageMap::DestroyObject(int num){
	int i;
	btCollisionObject* obj;
	btRigidBody* body;
	obj = world_->getCollisionObjectArray()[num];
	body = btRigidBody::upcast(obj);
	if (body && body->getMotionState())
	{
		delete body->getMotionState();
	}
	world_->removeCollisionObject( obj );

	for(i = 1; i < num_; i++){
		if(object_[num] == object_[num+i]){
			obj = world_->getCollisionObjectArray()[num+i];
			body = btRigidBody::upcast(obj);
			if (body && body->getMotionState())
			{
				delete body->getMotionState();
			}
			world_->removeCollisionObject( obj );
		}else
			break;
	}

	for(i = 1; i < num_;i--){
		if(object_[num] == object_[num-i]){
			obj = world_->getCollisionObjectArray()[num-i];
			body = btRigidBody::upcast(obj);
			if (body && body->getMotionState())
			{
				delete body->getMotionState();
			}
			world_->removeCollisionObject( obj );
		}
		else
			break;
	}
}

