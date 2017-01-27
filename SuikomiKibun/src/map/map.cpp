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
StageMap::StageMap(btDynamicsWorld* world) :
//	 m_sundirection(btVector3(1,-2,1)*1000),
//	 m_defaultContactProcessingThreshold(BT_LARGE_FLOAT),
		world_(world) {
	num_ = 0;
	object_num_ = 0;
	memset(object_, 0, 200);

	//オブジェクト
	btRigidBody* ground_body[7];

	//中心座標
	btVector3 ground_pos = btVector3(125, -16, 0);
	btVector3 ground_pos2 = btVector3(-125, -16, 125);
	btVector3 ground_pos3 = btVector3(-90, -8, -90);
	btVector3 ten_pos = btVector3(0, 500, 0);
	btVector3 stairs_pos = btVector3(-7, 108, 3);
	btVector3 stairs_pos2 = btVector3(0, -0.5, 5);
	btVector3 stairs_pos3 = btVector3(-10, 110, 10);
	btVector3 stairs_pos4 = btVector3(10, 110, 10);
	btVector3 stairs_pos5 = btVector3(10, 110, -10);
	btVector3 stairs_pos6 = btVector3(-10, 110, -10);
	btVector3 wall_pos = btVector3(0, 250, 250);
	btVector3 wall_pos2 = btVector3(0, 250, -250);
	btVector3 wall_pos3 = btVector3(250, 250, 0);
	btVector3 wall_pos4 = btVector3(-250, 250, 0);
	btVector3 kan_pos1 = btVector3(-83, 20, 104);
	btVector3 kan_pos2 = btVector3(-83, 20, 146);
	btVector3 kan_pos7 = btVector3(-41.5, 20, 147);
	btVector3 kan_pos8 = btVector3(-41.5, 20, 189);
	btVector3 kan_pos9 = btVector3(-83, 20, 21);
	btVector3 kan_pos10 = btVector3(-125, 20, 189);
	btVector3 kan_pos11 = btVector3(-208, 20, 63);
	btVector3 kan_pos13 = btVector3(-166.5, 20, 104);
	btVector3 kan_pos14 = btVector3(-166.5, 20, 146);
	btVector3 kan_pos15 = btVector3(-21, 20, 83);
	btVector3 kan_pos16 = btVector3(-62.5, 20, 41.5);
	btVector3 kan_pos17 = btVector3(-146, 20, 41.5);
	btVector3 kan_pos18 = btVector3(-187.5, 20, 41.5);
	btVector3 kan_pos20 = btVector3(-146, 20, 83);
	btVector3 kan_pos22 = btVector3(-188, 20, 167);
	btVector3 kan_pos23 = btVector3(-63, 20, 210);
	btVector3 kan_pos24 = btVector3(-105, 20, 210);
	btVector3 kan_pos26 = btVector3(-189, 20, 210);
	btVector3 kan_pos27 = btVector3(-231, 20, 210);
	btVector3 kan_pos32 = btVector3(-229, 20, 126);

	btVector3 taki_pos = btVector3(231, 35.5, 0);
	btVector3 taki_pos2 = btVector3(2, 0, 0);
	btVector3 taki_pos3 = btVector3(240, 76.5, 6);
	btVector3 taki_pos4 = btVector3(217, 76.5, 40);
	btVector3 taki_pos5 = btVector3(217, 76.5, -29);
	btVector3 taki_pos6 = btVector3(179, 40, 5);
	btVector3 hill_pos = btVector3(145.1, 0.4, -145.1);

	btVector3 hill_pos3 = btVector3(6, 10.0, 6);
	btVector3 hill_pos4 = btVector3(185, 5.8, -105);
	btVector3 hill_pos5 = btVector3(145.1, 0.4, 150.1);
	btVector3 hill_pos6 = btVector3(185, 5.8, 116);
	btVector3 hill_pos7 = btVector3(6, 10.0, -6);


	btTransform offset;
	offset.setIdentity();
	btTransform offset2;
	offset2.setIdentity();

	//形状を設定
	btCollisionShape *ground_shape = new btBoxShape(btVector3(125, 16, 250));
	btCollisionShape *ground_shape2 = new btBoxShape(btVector3(125, 16, 125));
	btCollisionShape *ten_shape = new btBoxShape(btVector3(250, 0.01, 250));
	btCollisionShape *stairs = new btBoxShape(btVector3(4, 0.3, 2));
	btCollisionShape *wall_shape = new btBoxShape(btVector3(250, 300, 0.1));
	btCollisionShape *kan_shape2 = new btBoxShape(btVector3(21, 20, 0.5));
	btCollisionShape *kan_shape3 = new btBoxShape(btVector3(0.5, 20, 21));
	btCollisionShape *taki_shape = new btBoxShape(btVector3(1, 37, 55));
	btCollisionShape *taki_shape2 = new btBoxShape(btVector3(10, 4, 46));
	btCollisionShape *taki_shape3 = new btBoxShape(btVector3(13, 4, 11));
	btCollisionShape *taki_shape4 = new btBoxShape(btVector3(0.1, 65, 15));
	btCollisionShape *hill_shape = new btBoxShape(btVector3(104.9, 0.4, 104.9));
	btCollisionShape *hill_shape3 = new btBoxShape(btVector3(104.9, 0.4, 99.9));


	//bulletに登録（地面&天井）
	offset.setOrigin(ground_pos);
	ground_body[0] = LocalCreateRigidBody(btScalar(0.), offset, ground_shape);
	object_[num_] = object_num_;
	level_[num_] = 0;
	color_[num_++] = btVector3(0, 1, 1);
//	color_[num_++] = btVector3(0, 0.63, 0.85);
	offset.setOrigin(ground_pos2);
	ground_body[1] = LocalCreateRigidBody(btScalar(0.), offset, ground_shape2);
	object_[num_] = object_num_;
	level_[num_] = 0;
//	color_[num_++] = btVector3(0, 0.63, 0.85);
	color_[num_++] = btVector3(1, 0.5, 0.5);
//	color_[num_++] = btVector3(1, 0, 0);
//	offset.setOrigin(ground_pos3);
//	ground_body[2] = LocalCreateRigidBody(btScalar(0.), offset, ground_shape2);
//	object_[num_] = object_num_;
//	level_[num_] = 0;
//	color_[num_++] = btVector3(0, 0.63, 0.85);
//	color_[num_++] = btVector3(1, 1, 1);
	offset.setIdentity();
	offset.setOrigin(ten_pos);
	ground_body[6] = LocalCreateRigidBody(btScalar(0.), offset, ten_shape);
	object_[num_] = -1;
	level_[num_] = 0;
//	color_[num_++] = btVector3(0.75, 1, 0.80);
	color_[num_++] = btVector3(1.0, 1.0, 1.0);

//	//螺旋階段
//	double h, r, g, b;
//	int h2;
//	h = 0;
//	offset.setOrigin(stairs_pos);
//	offset2.setOrigin(stairs_pos2);
//	for (int ten = 0; ten < 50; ten++) {
//		offset.setRotation(btQuaternion(PI_ / 5 * ten + PI_ / 4, 0, 0));
//		ground_body[6] = LocalCreateRigidBody(btScalar(0), offset, stairs);
//		h += 7.2;
//		h2 = h / 60;
//		if (h2 == 5) {
//			r = 255;
//			g = 0;
//			b = ((360 - h) / 60) * 255;
//		} else if (h2 == 4) {
//			r = ((h - 240) / 60) * 255;
//			g = 0;
//			b = 255;
//		} else if (h2 == 3) {
//			r = 0;
//			g = ((240 - h) / 60) * 255;
//			b = 255;
//		} else if (h2 == 2) {
//			r = 0;
//			g = 255;
//			b = ((h - 120) / 60) * 255;
//		} else if (h2) {
//			r = ((120 - h) / 60) * 255;
//			g = 255;
//			b = 0;
//		} else {
//			r = 255;
//			g = (h / 60) * 255;
//			b = 0;
//		}
//		object_[num_] = object_num_;
//		level_[num_] = 0;
//		color_[num_++] = btVector3(r / 255, g / 255, b / 255);
//		offset = offset * offset2;
//	}
//
////	赤階段
//	offset.setOrigin(stairs_pos3);
//	offset2.setOrigin(stairs_pos2);
//	for (int ten = 0; ten < 10; ten++) {
//		offset.setRotation(btQuaternion(PI_ / 20 * ten - PI_ / 4, 0, 0));
//		ground_body[6] = LocalCreateRigidBody(btScalar(0), offset, stairs);
//		object_[num_] = object_num_;
//		level_[num_] = 0;
//		color_[num_++] = btVector3(1.0, 0.0, 0.0);
//		offset = offset * offset2;
//	}
//
////	黄階段
//	offset.setOrigin(stairs_pos4);
//	offset2.setOrigin(stairs_pos2);
//	for (int ten = 0; ten < 10; ten++) {
//		offset.setRotation(btQuaternion(PI_ / 20 * ten + PI_ / 4, 0, 0));
//		ground_body[6] = LocalCreateRigidBody(btScalar(0), offset, stairs);
//		object_[num_] = object_num_;
//		level_[num_] = 0;
//		color_[num_++] = btVector3(1.0, 1.0, 0.0);
//		offset = offset * offset2;
//	}
//	// 	青階段
//	offset.setOrigin(stairs_pos5);
//	offset2.setOrigin(stairs_pos2);
//	for (int ten = 0; ten < 10; ten++) {
//		offset.setRotation(btQuaternion(PI_ / 20 * ten + PI_ * 3 / 4, 0, 0));
//		ground_body[6] = LocalCreateRigidBody(btScalar(0), offset, stairs);
//		object_[num_] = object_num_;
//		level_[num_] = 0;
//		color_[num_++] = btVector3(0.0, 0.0, 1.0);
//		offset = offset * offset2;
//	}
//
//// 	緑階段
//	offset.setOrigin(stairs_pos6);
//	offset2.setOrigin(stairs_pos2);
//	for (int ten = 0; ten < 10; ten++) {
//		offset.setRotation(btQuaternion(PI_ / 20 * ten - PI_ * 3 / 4, 0, 0));
//		ground_body[6] = LocalCreateRigidBody(btScalar(0), offset, stairs);
//		object_[num_] = object_num_;
//		level_[num_] = 0;
//		color_[num_++] = btVector3(0.0, 1.0, 0.0);
//		offset = offset * offset2;
//	}

	//滝
	offset.setIdentity();
	offset.setOrigin(taki_pos);
	offset2.setOrigin(taki_pos2);

//	for (int ten = 0; ten < 14; ten++) {
//		offset.setRotation(btQuaternion(PI_ * ten / 14, 0, 0));
//		ground_body[6] = LocalCreateRigidBody(btScalar(0), offset, taki_shape);
//		object_[num_] = object_num_;
//		level_[num_] = 0;
//		color_[num_++] = btVector3(1, 1, 1);
//
//		offset = offset * offset2;
//	}
	ground_body[6] = LocalCreateRigidBody(btScalar(0), offset, taki_shape);
	object_[num_] = object_num_;
	level_[num_] = 0;
	color_[num_++] = btVector3(1, 1, 1);

	offset.setIdentity();
	offset.setOrigin(taki_pos3);
	ground_body[6] = LocalCreateRigidBody(btScalar(0.), offset, taki_shape2);
	object_[num_] = object_num_;
	level_[num_] = 0;
	color_[num_++] = btVector3(0, 1, 1);
	offset.setIdentity();
	offset.setOrigin(taki_pos4);
	ground_body[6] = LocalCreateRigidBody(btScalar(0.), offset, taki_shape3);
	object_[num_] = object_num_;
	level_[num_] = 0;
	color_[num_++] = btVector3(0, 1, 1);
	offset.setIdentity();
	offset.setOrigin(taki_pos5);
	ground_body[6] = LocalCreateRigidBody(btScalar(0.), offset, taki_shape3);
	object_[num_] = object_num_;
	level_[num_] = 0;
	color_[num_++] = btVector3(0, 1, 1);
	offset.setIdentity();
	offset.setOrigin(taki_pos6);
	offset.setRotation(btQuaternion(0, 0, -PI_ / 3.5));
	ground_body[6] = LocalCreateRigidBody(btScalar(0), offset, taki_shape4);
	object_[num_] = object_num_;
	level_[num_] = 0;
	color_[num_++] = btVector3(0, 1, 1);

	//丘
	offset.setIdentity();
	offset2.setIdentity();
	offset.setOrigin(hill_pos);
	offset2.setOrigin(hill_pos3);
	offset.setRotation(btQuaternion(0, 0, 0));
	ground_body[2] = LocalCreateRigidBody(btScalar(0), offset, hill_shape);
	object_[num_] = object_num_;
	level_[num_] = 0;
	color_[num_++] = btVector3(0.5, 1, 0.5);
	offset.setOrigin(hill_pos4);
	for (int i = 0; i < 8; i++) {
		btCollisionShape *hill_shape = new btBoxShape(
				btVector3(64.9 - i * 6, 5.0, 64.9 - i * 6));
		ground_body[6] = LocalCreateRigidBody(btScalar(0), offset, hill_shape);
		object_[num_] = object_num_;
		level_[num_] = 0;
		if (i % 2)
			color_[num_++] = btVector3(0.5, 1, 0.5);
		else
			color_[num_++] = btVector3(0.76, 0.66, 0.09);
		offset = offset * offset2;
	}

	offset.setIdentity();
	offset2.setIdentity();
	offset.setOrigin(hill_pos5);
	offset2.setOrigin(hill_pos7);
	offset.setRotation(btQuaternion(0, 0, 0));
	ground_body[3] = LocalCreateRigidBody(btScalar(0), offset, hill_shape3);
	object_[num_] = object_num_;
	level_[num_] = 0;
	color_[num_++] = btVector3(0.5, 1, 0.5);
	offset.setOrigin(hill_pos6);
	for (int i = 0; i < 8; i++) {
		btCollisionShape *hill_shape = new btBoxShape(
				btVector3(64.9 - i * 6, 5.0, 64.9 - i * 6));
		ground_body[6] = LocalCreateRigidBody(btScalar(0), offset, hill_shape);
		object_[num_] = object_num_;
		level_[num_] = 0;
		if (i % 2)
			color_[num_++] = btVector3(0.5, 1, 0.5);
		else
			color_[num_++] = btVector3(0.76, 0.66, 0.09);
		offset = offset * offset2;
	}

	//壁
	offset.setIdentity();
	offset.setOrigin(wall_pos);
	ground_body[6] = LocalCreateRigidBody(btScalar(0.), offset, wall_shape);
//	object_[num_] = object_num_;
	object_[num_] = -1;
	level_[num_] = 0;
	color_[num_++] = btVector3(0.94, 0.94, 0.82);
	offset.setIdentity();
	offset.setOrigin(wall_pos2);
	ground_body[6] = LocalCreateRigidBody(btScalar(0.), offset, wall_shape);
//	object_[num_] = object_num_;
	object_[num_] = -1;
	level_[num_] = 0;
	color_[num_++] = btVector3(0.94, 0.94, 0.82);
	offset.setIdentity();
	offset.setOrigin(wall_pos3);
	offset.setRotation(btQuaternion(PI_ / 2, 0, 0));
	ground_body[6] = LocalCreateRigidBody(btScalar(0.), offset, wall_shape);
//	object_[num_] = object_num_;
	object_[num_] = -1;
	level_[num_] = 0;
	color_[num_++] = btVector3(0.94, 0.94, 0.82);
	offset.setIdentity();
	offset.setOrigin(wall_pos4);
	offset.setRotation(btQuaternion(-PI_ / 2, 0, 0));
	ground_body[6] = LocalCreateRigidBody(btScalar(0.), offset, wall_shape);
//	object_[num_] = object_num_;
	object_[num_] = -1;
	level_[num_] = 0;
	color_[num_++] = btVector3(0.94, 0.94, 0.82);

//迷路
	offset.setIdentity();
	offset.setOrigin(kan_pos1);
	offset.setRotation(btQuaternion(0, 0, 0));
	ground_body[6] = LocalCreateRigidBody(btScalar(0), offset, kan_shape3);
	object_[num_] = object_num_;
	level_[num_] = 0;
	color_[num_++] = btVector3(1, 0., 0.);
	offset.setIdentity();
	offset.setOrigin(kan_pos2);
	offset.setRotation(btQuaternion(0, 0, 0));
	ground_body[6] = LocalCreateRigidBody(btScalar(0), offset, kan_shape3);
	object_[num_] = object_num_;
	level_[num_] = 0;
	color_[num_++] = btVector3(1, 0., 0.);
	offset.setIdentity();
	offset.setOrigin(kan_pos7);
	offset.setRotation(btQuaternion(0, 0, 0));
	ground_body[6] = LocalCreateRigidBody(btScalar(0), offset, kan_shape3);
	object_[num_] = object_num_;
	level_[num_] = 0;
	color_[num_++] = btVector3(1, 0., 0.);
	offset.setIdentity();
	offset.setOrigin(kan_pos8);
	offset.setRotation(btQuaternion(0, 0, 0));
	ground_body[6] = LocalCreateRigidBody(btScalar(0), offset, kan_shape3);
	object_[num_] = object_num_;
	level_[num_] = 0;
	color_[num_++] = btVector3(1, 0., 0.);
	offset.setIdentity();
	offset.setOrigin(kan_pos9);
	offset.setRotation(btQuaternion(0, 0, 0));
	ground_body[6] = LocalCreateRigidBody(btScalar(0), offset, kan_shape3);
	object_[num_] = object_num_;
	level_[num_] = 0;
	color_[num_++] = btVector3(1, 0., 0.);
	offset.setIdentity();
	offset.setOrigin(kan_pos10);
	offset.setRotation(btQuaternion(0, 0, 0));
	ground_body[6] = LocalCreateRigidBody(btScalar(0), offset, kan_shape3);
	object_[num_] = object_num_;
	level_[num_] = 0;
	color_[num_++] = btVector3(1, 0., 0.);
	offset.setIdentity();
	offset.setOrigin(kan_pos11);
	offset.setRotation(btQuaternion(0, 0, 0));
	ground_body[6] = LocalCreateRigidBody(btScalar(0), offset, kan_shape3);
	object_[num_] = object_num_;
	level_[num_] = 0;
	color_[num_++] = btVector3(1, 0., 0.);
	offset.setIdentity();
	offset.setOrigin(kan_pos13);
	offset.setRotation(btQuaternion(0, 0, 0));
	ground_body[6] = LocalCreateRigidBody(btScalar(0), offset, kan_shape3);
	object_[num_] = object_num_;
	level_[num_] = 0;
	color_[num_++] = btVector3(1, 0., 0.);
	offset.setIdentity();
	offset.setOrigin(kan_pos14);
	offset.setRotation(btQuaternion(0, 0, 0));
	ground_body[6] = LocalCreateRigidBody(btScalar(0), offset, kan_shape3);
	object_[num_] = object_num_;
	level_[num_] = 0;
	color_[num_++] = btVector3(1, 0., 0.);
	offset.setIdentity();
	offset.setOrigin(kan_pos15);
	offset.setRotation(btQuaternion(0, 0, 0));
	ground_body[6] = LocalCreateRigidBody(btScalar(0), offset, kan_shape2);
	object_[num_] = object_num_;
	level_[num_] = 0;
	color_[num_++] = btVector3(1, 0., 0.);
	offset.setIdentity();
	offset.setOrigin(kan_pos16);
	offset.setRotation(btQuaternion(0, 0, 0));
	ground_body[6] = LocalCreateRigidBody(btScalar(0), offset, kan_shape2);
	object_[num_] = object_num_;
	level_[num_] = 0;
	color_[num_++] = btVector3(1, 0., 0.);
	offset.setIdentity();
	offset.setOrigin(kan_pos17);
	offset.setRotation(btQuaternion(0, 0, 0));
	ground_body[6] = LocalCreateRigidBody(btScalar(0), offset, kan_shape2);
	object_[num_] = object_num_;
	level_[num_] = 0;
	color_[num_++] = btVector3(1, 0., 0.);
	offset.setIdentity();
	offset.setOrigin(kan_pos18);
	offset.setRotation(btQuaternion(0, 0, 0));
	ground_body[6] = LocalCreateRigidBody(btScalar(0), offset, kan_shape2);
	object_[num_] = object_num_;
	level_[num_] = 0;
	color_[num_++] = btVector3(1, 0., 0.);
	offset.setIdentity();
	offset.setOrigin(kan_pos20);
	offset.setRotation(btQuaternion(0, 0, 0));
	ground_body[6] = LocalCreateRigidBody(btScalar(0), offset, kan_shape2);
	object_[num_] = object_num_;
	level_[num_] = 0;
	color_[num_++] = btVector3(1, 0., 0.);
	offset.setIdentity();
	offset.setOrigin(kan_pos22);
	offset.setRotation(btQuaternion(0, 0, 0));
	ground_body[6] = LocalCreateRigidBody(btScalar(0), offset, kan_shape2);
	object_[num_] = object_num_;
	level_[num_] = 0;
	color_[num_++] = btVector3(1, 0., 0.);
	offset.setIdentity();
	offset.setOrigin(kan_pos23);
	offset.setRotation(btQuaternion(0, 0, 0));
	ground_body[6] = LocalCreateRigidBody(btScalar(0), offset, kan_shape2);
	object_[num_] = object_num_;
	level_[num_] = 0;
	color_[num_++] = btVector3(1, 0., 0.);
	offset.setIdentity();
	offset.setOrigin(kan_pos24);
	offset.setRotation(btQuaternion(0, 0, 0));
	ground_body[6] = LocalCreateRigidBody(btScalar(0), offset, kan_shape2);
	object_[num_] = object_num_;
	level_[num_] = 0;
	color_[num_++] = btVector3(1, 0., 0.);
	offset.setIdentity();
	offset.setOrigin(kan_pos26);
	offset.setRotation(btQuaternion(0, 0, 0));
	ground_body[6] = LocalCreateRigidBody(btScalar(0), offset, kan_shape2);
	object_[num_] = object_num_;
	level_[num_] = 0;
	color_[num_++] = btVector3(1, 0., 0.);
	offset.setIdentity();
	offset.setOrigin(kan_pos27);
	offset.setRotation(btQuaternion(0, 0, 0));
	ground_body[6] = LocalCreateRigidBody(btScalar(0), offset, kan_shape2);
	object_[num_] = object_num_;
	level_[num_] = 0;
	color_[num_++] = btVector3(1, 0., 0.);
	offset.setIdentity();
	offset.setOrigin(kan_pos32);
	offset.setRotation(btQuaternion(0, 0, 0));
	ground_body[6] = LocalCreateRigidBody(btScalar(0), offset, kan_shape2);
	object_[num_] = object_num_;
	level_[num_] = 0;
	color_[num_++] = btVector3(1, 0., 0.);

//流動体
	int total = 2 * (5 - 1) * (5 - 1);
	int verstride = sizeof(btVector3);
	int indexstride = 3 * sizeof(int);
	vertices_ = new btVector3[total];
	indices_ = new int[3 * total];
	SetVertexPositions(10, 1);
	btTriangleIndexVertexArray* arrays;

	int index = 0;
	for (int i = 0; i < 5 - 1; i++) {
		for (int j = 0; j < 5 - 1; j++) {
			indices_[index++] = j * 5 + i;
			indices_[index++] = j * 5 + i + 1;
			indices_[index++] = (j + 1) * 5 + i + 1;
			indices_[index++] = j * 5 + i;
			indices_[index++] = (j + 1) * 5 + i + 1;
			indices_[index++] = (j + 1) * 5 + i;
		}
	}

	arrays = new btTriangleIndexVertexArray(total, indices_, indexstride,
			2 * (5 - 1) * (5 - 1), (btScalar*) &vertices_[0].x(), verstride);
	trimeshShape_ = new btBvhTriangleMeshShape(arrays, true);
	btCollisionShape* groundShape4 = trimeshShape_;
	offset.setIdentity();
	offset.setOrigin(ground_pos3);
	staticBody_ = LocalCreateRigidBody(btScalar(0), offset, groundShape4);
	object_[num_] = 0;
	level_[num_] = 0;
	color_[num_++] = btVector3(0, 0.63, 0.85);

//反発係数
	btScalar ground_rest = 0.4;
//	btScalar wall_rest = 0;
//	btScalar kan_rest = 0;

	//反発係数設定
	for (int i = 0; i < 4; i++) {
		ground_body[i]->setRestitution(ground_rest);
	}

//	for (int i = 0; i < 4; i++) {
//		wall_body[i]->setRestitution(wall_rest);
//	}

//	for (int i = 1; i < 21; i++) {
//		kan_body[i]->setRestitution(kan_rest);
//	}

	printf("%d ", world_->getNumCollisionObjects());
//クモ
	btVector3 posision_ka(130, 20, -205);
	CreateSpider(posision_ka, 5);
	btVector3 posision_kb(-64, 10, -64);
	CreateSpider(posision_kb, 2);
	btVector3 posision_kc(-9, 10, -121);
	CreateSpider(posision_kc, 2);
	btVector3 posision_kd(-111, 10, -77);
	CreateSpider(posision_kd, 2);
	btVector3 posision_ke(-233, 10, -200);
	CreateSpider(posision_ke, 2);
	btVector3 posision_kf(-144, 10, -237);
	CreateSpider(posision_kf, 2);
	btVector3 posision_kg(-77, 3, 151);
	CreateSpider(posision_kg, 2);
	btVector3 posision_kh(-8, 3, 73);
	CreateSpider(posision_kh, 2);
	btVector3 posision_ki(-244, 3, 9);
	CreateSpider(posision_ki, 2);
	btVector3 posision_kj(-155, 3, 178);
	CreateSpider(posision_kj, 2);
	btVector3 posision_kk(-210, 3, 240);
	CreateSpider(posision_kk, 2);
	btVector3 posision_kl(13, 3, 177);
	CreateSpider(posision_kl, 2);
	btVector3 posision_km(77, 3, 198);
	CreateSpider(posision_km, 2);
	btVector3 posision_kn(40, 3, 0);
	CreateSpider(posision_kn, 2);
	btVector3 posision_ko(90, 3, -89);
	CreateSpider(posision_ko, 2);
	btVector3 posision_kp(105, 3, -190);
	CreateSpider(posision_kp, 2);
	btVector3 posision_kq(70, 3, -230);
	CreateSpider(posision_kq, 2);

//雪だるま
	btVector3 position_sa(220, 110, 80);
	CreateSnowman(position_sa, 3);
	btVector3 position_sb(-10, 10, -15);
	CreateSnowman(position_sb, 2);
	btVector3 position_sc(-72, 10, -20);
	CreateSnowman(position_sc, 2);
	btVector3 position_sd(-172, 10, -172);
	CreateSnowman(position_sd, 2);
	btVector3 position_se(-240, 10, -123);
	CreateSnowman(position_se, 2);
	btVector3 position_sf(-8, 10, -233);
	CreateSnowman(position_sf, 2);
	btVector3 position_sg(-33, 1, 130);
	CreateSnowman(position_sg, 2);
	btVector3 position_sh(-62, 1, 80);
	CreateSnowman(position_sh, 2);
	btVector3 position_si(-111, 1, 220);
	CreateSnowman(position_si, 2);
	btVector3 position_sj(-166, 1, 230);
	CreateSnowman(position_sj, 2);
	btVector3 position_sk(-198, 1, 55);
	CreateSnowman(position_sk, 2);
	btVector3 position_sl(33, 1, 190);
	CreateSnowman(position_sl, 2);
	btVector3 position_sm(90, 1, 80);
	CreateSnowman(position_sm, 2);
	btVector3 position_sn(70, 1, -133);
	CreateSnowman(position_sn, 2);
	btVector3 position_so(70, 1, -30);
	CreateSnowman(position_so, 2);
	btVector3 position_sp(11, 1, -90);
	CreateSnowman(position_sp, 2);
	btVector3 position_sq(100, 1, 150);
	CreateSnowman(position_sq, 2);

//ピラミッド
	btVector3 position_pa(-230, 20, -100);
	CreatePyramid(position_pa, 4);
	btVector3 position_pb(220, 5, 215);
	CreatePyramid(position_pb, 4);
	btVector3 position_pc(-70, 0, 13);
	CreatePyramid(position_pc, 4);

//トライアングル
	btVector3 position_ta(180, 0, -5);
	CreateTriangle(position_ta, -3);
	btVector3 position_tb(-220, 0, 95);
	CreateTriangle(position_tb, -1);
	btVector3 position_tc(-112, 0, 190);
	CreateTriangle(position_tc, -4);
	btVector3 position_td(-50, 10, -200);
	CreateTriangle(position_td, -3);
	btVector3 position_te(215, 100, 40);
	CreateTriangle(position_te, -1);
	btVector3 position_tf(215, 100, -30);
	CreateTriangle(position_tf, -2);
	btVector3 position_tg(150, 5, 210);
	CreateTriangle(position_tg, -4);
	btVector3 position_th(-125, 10, -50);
	CreateTriangle(position_th, -2);

//きのこ
//	btVector3 position_ma(160, 41, -130);
//	CreateMush(position_ma, 3, 2);
//	btVector3 position_mb(128, 10, -162);
//	CreateMush(position_mb, 3, 1);
//	btVector3 position_md(185, 61, 113);
//	CreateMush(position_md, 3, 3);
//	btVector3 position_me(160, 41, 80);
//	CreateMush(position_me, 3, 3);
//	btVector3 position_mf(135, 21, -60);
//	CreateMush(position_mf, 3, 2);
//	btVector3 position_mg(135, 21, 100);
//	CreateMush(position_mg, 3, 1);
//	btVector3 position_mh(135, 21, 60);
//	CreateMush(position_mh, 3, 2);
//	btVector3 position_mi(140, 11, 175);
//	CreateMush(position_mi, 3, 1);
//	btVector3 position_mj(190, 31, 155);
//	CreateMush(position_mj, 3, 2);
//	btVector3 position_mk(220, 21, 163);
//	CreateMush(position_mk, 3, 2);
//	btVector3 position_ml(125, 11, 130);
//	CreateMush(position_ml, 3, 3);
//	btVector3 position_mm(150, 31, 140);
//	CreateMush(position_mm, 3, 1);
//	btVector3 position_mn(195, 71, 60);
//	CreateMush(position_mn, 3, 1);
//	btVector3 position_mo(210, 51, 130);
//	CreateMush(position_mo, 3, 1);
//	btVector3 position_mp(125, 11, -90);
//	CreateMush(position_mp, 3, 3);
//	btVector3 position_mq(150, 31, -115);
//	CreateMush(position_mq, 3, 1);
//	btVector3 position_mr(170, 51, -70);
//	CreateMush(position_mr, 3, 3);
//	btVector3 position_ms(180, 21, -155);
//	CreateMush(position_ms, 3, 3);
//	btVector3 position_mt(220, 31, -140);
//	CreateMush(position_mt, 3, 1);
//	btVector3 position_mu(195, 71, -85);
//	CreateMush(position_mu, 3, 1);
//りんご
	btVector3 position_ab(220, 110, -50);
	CreateApple(position_ab, 3);
	btVector3 position_ac(-30, 10, -30);
	CreateApple(position_ac, 1);
	btVector3 position_ad(-50, 10, -70);
	CreateApple(position_ad, 1);
	btVector3 position_ae(-70, 10, -10);
	CreateApple(position_ae, 1);
	btVector3 position_af(-120, 10, -120);
	CreateApple(position_af, 1);
	btVector3 position_ag(-120, 10, -130);
	CreateApple(position_ag, 1);
	btVector3 position_ah(-60, 10, -240);
	CreateApple(position_ah, 1);
	btVector3 position_ai(-245, 10, -245);
	CreateApple(position_ai, 1);
	btVector3 position_aj(-150, 10, -150);
	CreateApple(position_aj, 1);
	btVector3 position_ak(-160, 10, -180);
	CreateApple(position_ak, 1);
	btVector3 position_al(-190, 10, -170);
	CreateApple(position_al, 1);
	btVector3 position_an(-30, 0, 20);
	CreateApple(position_an, 1);
	btVector3 position_am(-50, 0, 100);
	CreateApple(position_am, 1);
	btVector3 position_ao(-60, 0, 240);
	CreateApple(position_ao, 1);
	btVector3 position_ap(-70, 0, 230);
	CreateApple(position_ap, 1);
	btVector3 position_aq(-140, 0, 200);
	CreateApple(position_aq, 1);
	btVector3 position_ar(-220, 0, 30);
	CreateApple(position_ar, 1);
	btVector3 position_as(-150, 0, 20);
	CreateApple(position_as, 1);
	btVector3 position_at(-230, 0, 160);
	CreateApple(position_at, 1);
	btVector3 position_au(-210, 0, 100);
	CreateApple(position_au, 1);
	btVector3 position_av(-220, 0, 80);
	CreateApple(position_av, 1);
	btVector3 position_aw(20, 1, 22);
	CreateApple(position_aw, 1);
	btVector3 position_ax(80, 1, 125);
	CreateApple(position_ax, 1);
	btVector3 position_ay(100, 1, 10);
	CreateApple(position_ay, 1);
	btVector3 position_az(100, 1, 200);
	CreateApple(position_az, 1);
	btVector3 position_aaa(160, 1, 240);
	CreateApple(position_aaa, 1);
	btVector3 position_aab(10, 1, -190);
	CreateApple(position_aab, 1);
	btVector3 position_aac(80, 1, -50);
	CreateApple(position_aac, 1);
	btVector3 position_aad(10, 1, -20);
	CreateApple(position_aad, 1);
	btVector3 position_aae(60, 1, -150);
	CreateApple(position_aae, 1);
	btVector3 position_aaf(240, 1, -240);
	CreateApple(position_aaf, 1);

//タワー
	btVector3 position_wa(-125, 0, 125);
	CreateTower(position_wa, 5);

//ポンデリング
	btVector3 position_oa(-60, 20, -170);
	CreatePonde(position_oa, 4);
	btVector3 position_ob(65, 10, 65);
	CreatePonde(position_ob, 4);
	btVector3 position_oc(-220, 10, 230);
	CreatePonde(position_oc, 4);

//プレイヤー

//描画
	m_shapeDrawer = new GL_ShapeDrawer();
	m_shapeDrawer->enableTexture(true);

	printf("%d\n", world_->getNumCollisionObjects());

	//効果音
	se_absorb_ = new Sound("sound/absorb.wav");

}

//デストラクタ
StageMap::~StageMap() {
	//オブジェクトの破棄
	for (int i = world_->getNumCollisionObjects() - 1; i >= 0; i--) {
		btCollisionObject* obj = world_->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState()) {
			delete body->getMotionState();
		}
		world_->removeCollisionObject(obj);
		delete obj;
	}

	delete m_shapeDrawer;
	delete[] vertices_;
	delete trimeshShape_;
	delete[] indices_;

	//効果音
	delete se_absorb_;
}

//更新
void StageMap::Update() {
	//流動体
//	static float offset = 0;
//	offset += 0.03;
//	SetVertexPositions(10, offset);
//	btVector3 worldMin(-1000, -1000, -1000);
//	btVector3 worldMax(1000, 1000, 1000);
//	trimeshShape_->refitTree(worldMin, worldMax);
	//効果音
	se_absorb_->SetSourceToListener();
}

//描画
void StageMap::Draw() {

//	glClear(GL_STENCIL_BUFFER_BIT);
//	glEnable(GL_CULL_FACE);
//	RenderScene(0);

//
	glDisable(GL_LIGHTING);
//	glDepthMask(GL_FALSE);
//	glDepthFunc(GL_LEQUAL);
//	//glEnable(GL_STENCIL_TEST);
//	glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
//	glStencilFunc(GL_ALWAYS,1,0xFFFFFFFFL);
//	glFrontFace(GL_CCW);
//	glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);
//	RenderScene(1);
//	glFrontFace(GL_CW);
//	glStencilOp(GL_KEEP,GL_KEEP,GL_DECR);
//	RenderScene(1);
//	glFrontFace(GL_CCW);
//
//
//	glPolygonMode(GL_FRONT,GL_FILL);
//	glPolygonMode(GL_BACK,GL_FILL);
//	glShadeModel(GL_SMOOTH);
//	glEnable(GL_DEPTH_TEST);
//	glDepthFunc(GL_LESS);
//	glEnable(GL_LIGHTING);
//	glDepthMask(GL_TRUE);
//	glCullFace(GL_BACK);
//	glFrontFace(GL_CCW);
//	glEnable(GL_CULL_FACE);
//	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
//
//	glDepthFunc(GL_LEQUAL);
//	glStencilFunc( GL_NOTEQUAL, 0, 0xFFFFFFFFL );
//	glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
//	glDisable(GL_LIGHTING);

	RenderScene();
//
//	glEnable(GL_LIGHTING);
//	glDepthFunc(GL_LESS);
//	glDisable(GL_STENCIL_TEST);
//	glDisable(GL_CULL_FACE);

	glFlush();

}

//オブジェクトの描画
void StageMap::RenderScene() {
	btScalar m[16];
	btMatrix3x3 rot;
	rot.setIdentity();
	const int numObjects = world_->getNumCollisionObjects();
	btVector3 wireColor(1, 0, 0);
	for (int i = 0; i < numObjects - 3; i++) {
		btCollisionObject* colObj = world_->getCollisionObjectArray()[i];
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

		wireColor = color_[i];

		btVector3 aabbMin, aabbMax;
		world_->getBroadphase()->getBroadphaseAabb(aabbMin, aabbMax);

		aabbMin -= btVector3(BT_LARGE_FLOAT, BT_LARGE_FLOAT, BT_LARGE_FLOAT);
		aabbMax += btVector3(BT_LARGE_FLOAT, BT_LARGE_FLOAT, BT_LARGE_FLOAT);

		if (object_[i] != -1)
			m_shapeDrawer->drawOpenGL(m, colObj->getCollisionShape(),
					wireColor * btScalar(0.3), 0, aabbMin, aabbMax);
	}

}

//bulletに登録
btRigidBody* StageMap::LocalCreateRigidBody(float mass,
		const btTransform& startTransform, btCollisionShape* shape) {
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		shape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(
			startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape,
			localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);

	world_->addRigidBody(body);

	return body;
}

//クモの作成
void StageMap::CreateSpider(const btVector3& position, int level) {
	btRigidBody* m_bodies[13];
	btCollisionShape* m_shapes[13];
	btTypedConstraint* m_joints[12];
	btVector3 vUP(0, 1, 0);

	float fBodySize;
	float fLegLength;
	float fForeLegLength;
	float mass;
	float wide;
	float wide2;

	//レベル分け
	if (level == 5) {
		fBodySize = 6.75;
		fLegLength = 15.65;
		fForeLegLength = 15.95;
		mass = 0.001;
		wide = 0.7;
		wide2 = 0.78;
	} else if (level == 4) {
		fBodySize = 3.75;
		fLegLength = 10.45;
		fForeLegLength = 10.75;
		mass = 0.001;
		wide = 0.4;
		wide2 = 0.48;
	} else if (level == 3) {
		fBodySize = 1.75;
		fLegLength = 6.45;
		fForeLegLength = 6.75;
		mass = 0.001;
		wide = 0.2;
		wide2 = 0.28;
	} else if (level == 2) {
		fBodySize = 1.6;
		fLegLength = 3.75;
		fForeLegLength = 4.05;
		mass = 0.01;
		wide = 0.2;
		wide2 = 0.18;
	} else {
		fBodySize = 0.55;
		fLegLength = 1.45;
		fForeLegLength = 1.75;
		mass = 0.01;
		wide = 0.05;
		wide2 = 0.04;
	}

	//形状設定
	m_shapes[0] = new btCapsuleShape(btScalar(fBodySize), btScalar(0.10));
	int i;
	for (i = 0; i < 6; i++) {
		m_shapes[1 + 2 * i] = new btCapsuleShape(btScalar(wide),
				btScalar(fLegLength));
		m_shapes[2 + 2 * i] = new btCapsuleShape(btScalar(wide2),
				btScalar(fForeLegLength));
	}
	float fHeight = 0.5;
	btTransform offset;
	offset.setIdentity();
	offset.setOrigin(position);
	btVector3 vRoot = btVector3(btScalar(0.), btScalar(fHeight), btScalar(0.));
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(vRoot);

	//bulletに登録
	m_bodies[0] = LocalCreateRigidBody(btScalar(mass), offset * transform,
			m_shapes[0]);
	object_[num_] = ++object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0.6, 0.35, 0);

	for (i = 0; i < 6; i++) {
		float fAngle = 2 * 3.14 * i / 6;
		float fSin = sin(fAngle);
		float fCos = cos(fAngle);

		transform.setIdentity();
		btVector3 vBoneOrigin = btVector3(
				btScalar(fCos * (fBodySize + 0.5 * fLegLength)),
				btScalar(fHeight),
				btScalar(fSin * (fBodySize + 0.5 * fLegLength)));
		transform.setOrigin(vBoneOrigin);

		btVector3 vToBone = (vBoneOrigin - vRoot).normalize();
		btVector3 vAxis = vToBone.cross(vUP);
		transform.setRotation(btQuaternion(vAxis, 1.57));
		m_bodies[1 + 2 * i] = LocalCreateRigidBody(btScalar(mass),
				offset * transform, m_shapes[2 + 2 * i]);
		object_[num_] = object_num_;
		level_[num_] = level;
		color_[num_++] = btVector3(0.2, 0.4, 0.4);

		transform.setIdentity();
		transform.setOrigin(
				btVector3(btScalar(fCos * (fBodySize + fLegLength)),
						btScalar(fHeight - 0.5 * fForeLegLength),
						btScalar(fSin * (fBodySize + fLegLength))));
		m_bodies[2 + 2 * i] = LocalCreateRigidBody(btScalar(mass),
				offset * transform, m_shapes[2 + 2 * i]);
		object_[num_] = object_num_;
		level_[num_] = level;
		color_[num_++] = btVector3(0.8, 0.6, 0.2);
	}

	for (i = 0; i < 6; ++i) {
		m_bodies[i]->setDamping(0.05, 0.85);
		m_bodies[i]->setDeactivationTime(0.8);
		m_bodies[i]->setSleepingThresholds(0.5f, 0.5f);
	}

	//オブジェクトを繋げる
	btHingeConstraint* hingeC;
	btTransform localA, localB, localC;

	for (i = 0; i < 6; i++) {
		float fAngle = 2 * 3.14 * i / 6;
		float fSin = sin(fAngle);
		float fCos = cos(fAngle);

		localA.setIdentity();
		localB.setIdentity();
		localA.getBasis().setEulerZYX(0, -fAngle, 0);
		localA.setOrigin(
				btVector3(btScalar(fCos * fBodySize), btScalar(0.),
						btScalar(fSin * fBodySize)));
		localB = m_bodies[1 + 2 * i]->getWorldTransform().inverse()
				* m_bodies[0]->getWorldTransform() * localA;
		hingeC = new btHingeConstraint(*m_bodies[0], *m_bodies[1 + 2 * i],
				localA, localB);
		hingeC->setLimit(btScalar(-0.75 * 0.78), btScalar(0.39));
		m_joints[2 * i] = hingeC;
		world_->addConstraint(m_joints[2 * i], true);

		localA.setIdentity();
		localB.setIdentity();
		localC.setIdentity();
		localA.getBasis().setEulerZYX(0, -fAngle, 0);
		localA.setOrigin(
				btVector3(btScalar(fCos * (fBodySize + fLegLength)),
						btScalar(0.),
						btScalar(fSin * (fBodySize + fLegLength))));
		localB = m_bodies[1 + 2 * i]->getWorldTransform().inverse()
				* m_bodies[0]->getWorldTransform() * localA;
		localC = m_bodies[2 + 2 * i]->getWorldTransform().inverse()
				* m_bodies[0]->getWorldTransform() * localA;
		hingeC = new btHingeConstraint(*m_bodies[1 + 2 * i],
				*m_bodies[2 + 2 * i], localB, localC);
		hingeC->setLimit(btScalar(-0.39), btScalar(0.2));
		m_joints[1 + 2 * i] = hingeC;
		world_->addConstraint(m_joints[1 + 2 * i], true);
	}

}

//雪だるま作成
void StageMap::CreateSnowman(const btVector3& position, int level) {
	btRigidBody* snow_bodies[3];
	btCollisionShape* snow_shapes[3];
	btTypedConstraint* snow_joints[2];
	float size;
	float mass;

	//レベル分け
	if (level == 5) {
		size = 1;
		mass = 0.01;
	} else if (level == 4) {
		size = 5;
		mass = 0.01;
	} else if (level == 3) {
		size = 5;
		mass = 0.01;
	} else if (level == 2) {
		size = 1.7;
		mass = 0.01;
	} else {
		size = 0.7;
		mass = 0.01;
	}

	//各オブジェクトの位置を決める
	btVector3 snow_position1(0, size, 0);
	btVector3 snow_position2(0, size * 2 + size / 2, 0);
	btVector3 snow_position3(0, size * 2 + size + size / 3, 0);

	//オブジェクトをbulletに登録
	snow_shapes[0] = new btSphereShape(btScalar(size));
	btTransform offset;
	btTransform offset2;
	offset.setIdentity();
	offset.setOrigin(position);
	offset2.setIdentity();
	offset2.setOrigin(snow_position1);
	snow_bodies[0] = LocalCreateRigidBody(btScalar(mass), offset * offset2,
			snow_shapes[0]);
	object_[num_] = ++object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0.58, 0.30, 0.73);
	offset2.setIdentity();
	offset2.setOrigin(snow_position2);
	snow_shapes[1] = new btSphereShape(btScalar(size / 2));
	snow_bodies[1] = LocalCreateRigidBody(btScalar(mass), offset * offset2,
			snow_shapes[1]);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(1, 0.47, 0);
	offset2.setIdentity();
	offset2.setOrigin(snow_position3);
	snow_shapes[2] = new btSphereShape(btScalar(size / 3));
	snow_bodies[2] = LocalCreateRigidBody(btScalar(mass), offset * offset2,
			snow_shapes[2]);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0.94, 0.87, 0.55);

	//オブジェクトを繋げる
	btHingeConstraint* hingeCC;
	btTransform localA, localB, localC;
	localA.setIdentity();
	localB.setIdentity();
	localB = snow_bodies[0]->getWorldTransform().inverse()
			* snow_bodies[1]->getWorldTransform() * localA;

	hingeCC = new btHingeConstraint(*snow_bodies[0], *snow_bodies[1], localB,
			localA);
	hingeCC->setLimit(btScalar(0), btScalar(0));
	snow_joints[0] = hingeCC;
	world_->addConstraint(snow_joints[0], true);

	localC = snow_bodies[1]->getWorldTransform().inverse()
			* snow_bodies[2]->getWorldTransform() * localA;

	hingeCC = new btHingeConstraint(*snow_bodies[1], *snow_bodies[2], localC,
			localA);
	hingeCC->setLimit(btScalar(0), btScalar(0));
	snow_joints[1] = hingeCC;
	world_->addConstraint(snow_joints[1], true);

}

void StageMap::CreatePyramid(const btVector3& position, int level) {
	btRigidBody* box_body[14];
	float cube_size;
	float cube_mass;

	//レベル分け
	if (level == 5) {
		cube_size = 2.3;
		cube_mass = 8;
	} else if (level == 4) {
		cube_size = 2.3;
		cube_mass = 0.01;
	} else if (level == 3) {
		cube_size = 2.3;
		cube_mass = 0.01;
	} else if (level == 2) {

	} else if (level == 1) {

	} else {
		cube_size = 7;
		cube_mass = 0;
	}
	btTypedConstraint* joint;

	//オブジェクトの位置を決める
	btVector3 position1(cube_size * 2, cube_size * 3, cube_size * 2);
	btVector3 position2(cube_size * 4, cube_size, 0);
	btVector3 position3(0, cube_size, cube_size * 4);
	btVector3 position4(cube_size * 4, cube_size, cube_size * 4);
	btVector3 position5(cube_size * 6, cube_size * 3, cube_size * 2);
	btVector3 position6(cube_size * 8, cube_size, 0);
	btVector3 position7(cube_size * 8, cube_size, cube_size * 4);
	btVector3 position8(cube_size * 2, cube_size * 3, cube_size * 6);
	btVector3 position9(0, cube_size, cube_size * 8);
	btVector3 position10(cube_size * 4, cube_size, cube_size * 8);
	btVector3 position11(cube_size * 6, cube_size * 3, cube_size * 6);
	btVector3 position12(cube_size * 8, cube_size, cube_size * 8);
	btVector3 position13(cube_size * 4, cube_size * 5, cube_size * 4);
	btVector3 position14(0, cube_size, 0);
	btCollisionShape *shape = new btBoxShape(
			btVector3(cube_size, cube_size, cube_size));

	btTransform offset;
	offset.setIdentity();
	btTransform offset2;
	offset2.setIdentity();
	offset2.setOrigin(position14);
	offset.setOrigin(position);

	//cubeをbulletに登録
	box_body[0] = LocalCreateRigidBody(btScalar(cube_mass), offset * offset2,
			shape);
	object_[num_] = ++object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0.71, 0, 0.48);
	offset2.setOrigin(position1);
	box_body[1] = LocalCreateRigidBody(btScalar(cube_mass), offset * offset2,
			shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0, 0.6, 0.52);
	offset2.setOrigin(position2);
	box_body[2] = LocalCreateRigidBody(btScalar(cube_mass), offset * offset2,
			shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0.22, 0.13, 0.52);
	offset2.setOrigin(position3);
	box_body[3] = LocalCreateRigidBody(btScalar(cube_mass), offset * offset2,
			shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0.88, 0.78, 0.18);
	offset2.setOrigin(position4);
	box_body[4] = LocalCreateRigidBody(btScalar(cube_mass), offset * offset2,
			shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0., 0.61, 0.77);
	offset2.setOrigin(position5);
	box_body[5] = LocalCreateRigidBody(btScalar(cube_mass), offset * offset2,
			shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0.77, 0.78, 0.16);
	offset2.setOrigin(position6);
	box_body[6] = LocalCreateRigidBody(btScalar(cube_mass), offset * offset2,
			shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0.58, 0, 0.49);
	offset2.setOrigin(position7);
	box_body[7] = LocalCreateRigidBody(btScalar(cube_mass), offset * offset2,
			shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0.84, 0.52, 0.69);
	offset2.setOrigin(position8);
	box_body[8] = LocalCreateRigidBody(btScalar(cube_mass), offset * offset2,
			shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0.93, 0.4, 0.9);
	offset2.setOrigin(position9);
	box_body[9] = LocalCreateRigidBody(btScalar(cube_mass), offset * offset2,
			shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0.87, 0.31, 0.08);
	offset2.setOrigin(position10);
	box_body[10] = LocalCreateRigidBody(btScalar(cube_mass), offset * offset2,
			shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0.72, 0.05, 0.23);
	offset2.setOrigin(position11);
	box_body[11] = LocalCreateRigidBody(btScalar(cube_mass), offset * offset2,
			shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0, 0.47, 0.73);
	offset2.setOrigin(position12);
	box_body[12] = LocalCreateRigidBody(btScalar(cube_mass), offset * offset2,
			shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0.62, 0.76, 0.22);
	offset2.setOrigin(position13);
	box_body[13] = LocalCreateRigidBody(btScalar(cube_mass), offset * offset2,
			shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0.8, 0.32, 0.57);

	//cubeを繋げる
	btHingeConstraint* hingeC;
	btTransform localA, localB, localC;

	localA.setIdentity();
	localB.setIdentity();
	localB = box_body[0]->getWorldTransform().inverse()
			* box_body[1]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[0], *box_body[1], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity();
	localB.setIdentity();
	localB = box_body[1]->getWorldTransform().inverse()
			* box_body[2]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[1], *box_body[2], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity();
	localB.setIdentity();
	localB = box_body[1]->getWorldTransform().inverse()
			* box_body[3]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[1], *box_body[3], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity();
	localB.setIdentity();
	localB = box_body[1]->getWorldTransform().inverse()
			* box_body[4]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[1], *box_body[4], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity();
	localB.setIdentity();
	localB = box_body[2]->getWorldTransform().inverse()
			* box_body[5]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[2], *box_body[5], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity();
	localB.setIdentity();
	localB = box_body[4]->getWorldTransform().inverse()
			* box_body[5]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[4], *box_body[5], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity();
	localB.setIdentity();
	localB = box_body[5]->getWorldTransform().inverse()
			* box_body[6]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[5], *box_body[6], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity();
	localB.setIdentity();
	localB = box_body[5]->getWorldTransform().inverse()
			* box_body[7]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[5], *box_body[7], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity();
	localB.setIdentity();
	localB = box_body[3]->getWorldTransform().inverse()
			* box_body[8]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[3], *box_body[8], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity();
	localB.setIdentity();
	localB = box_body[4]->getWorldTransform().inverse()
			* box_body[8]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[4], *box_body[8], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity();
	localB.setIdentity();
	localB = box_body[8]->getWorldTransform().inverse()
			* box_body[9]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[8], *box_body[9], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity();
	localB.setIdentity();
	localB = box_body[8]->getWorldTransform().inverse()
			* box_body[10]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[8], *box_body[10], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity();
	localB.setIdentity();
	localB = box_body[4]->getWorldTransform().inverse()
			* box_body[11]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[4], *box_body[11], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity();
	localB.setIdentity();
	localB = box_body[10]->getWorldTransform().inverse()
			* box_body[11]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[10], *box_body[11], localB,
			localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity();
	localB.setIdentity();
	localB = box_body[11]->getWorldTransform().inverse()
			* box_body[12]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[11], *box_body[12], localB,
			localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity();
	localB.setIdentity();
	localB = box_body[1]->getWorldTransform().inverse()
			* box_body[13]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[1], *box_body[13], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity();
	localB.setIdentity();
	localB = box_body[5]->getWorldTransform().inverse()
			* box_body[13]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[5], *box_body[13], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity();
	localB.setIdentity();
	localB = box_body[8]->getWorldTransform().inverse()
			* box_body[13]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[8], *box_body[13], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity();
	localB.setIdentity();
	localB = box_body[11]->getWorldTransform().inverse()
			* box_body[13]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body[11], *box_body[13], localB,
			localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);
}

void StageMap::CreateTriangle(const btVector3& position, int level) {
	btRigidBody* triangle_sphere[5];
	btRigidBody* triangle_sides[6];
	btTypedConstraint* joints_triangel;

	float sphere_size;
	float side_size;
	float mass;
	//レベル分け

	sphere_size = 0.5;
	side_size = 4;
	mass = 100;

	btTransform trans;

	//オブジェクトの位置を決める
	btVector3 position1(side_size / sqrt(3), -side_size * sqrt(3) / 2, 0);
	btVector3 position2(-side_size / (2 * sqrt(3)), -side_size * sqrt(3) / 2,
			side_size / 2);
	btVector3 position3(-side_size / (2 * sqrt(3)), -side_size * sqrt(3) / 2,
			-side_size / 2);
	btVector3 position4(side_size / sqrt(3) / 2, -side_size * sqrt(3) / 4, 0);
	btVector3 position5(-side_size / (4 * sqrt(3)), -side_size * sqrt(3) / 4,
			side_size / 4);
	btVector3 position6(-side_size / (4 * sqrt(3)), -side_size * sqrt(3) / 4,
			-side_size / 4);
	btVector3 position7(-side_size / (2 * sqrt(3)), -side_size * sqrt(3) / 2,
			0);
	btVector3 position8(side_size / (4 * sqrt(3)), -side_size * sqrt(3) / 2,
			side_size / 4);
	btVector3 position9(side_size / (4 * sqrt(3)), -side_size * sqrt(3) / 2,
			-side_size / 4);
	btVector3 position10(0, -side_size * sqrt(3) / 3, 0);

	//形状設定
	btCollisionShape *sphere_shape = new btSphereShape(sphere_size);
	btCollisionShape *sphere_shape2 = new btSphereShape(sphere_size * 1.7);
	btCollisionShape *side_shape = new btCapsuleShape(btScalar(0.1),
			btScalar(side_size));

	btTransform offset;
	offset.setIdentity();
	btTransform offset2;
	offset2.setIdentity();
	offset2.setOrigin(position1);
	offset.setOrigin(position);

	//cubeをbulletに登録
	triangle_sphere[0] = LocalCreateRigidBody(btScalar(mass), offset,
			sphere_shape);
	object_[num_] = ++object_num_;
	level_[num_] = level;
	if (level == -1) {
		color_[num_++] = btVector3(1, 0, 0);
	} else if (level == -2) {
		color_[num_++] = btVector3(0, 1, 0);
	} else if(level == -3){
		color_[num_++] = btVector3(0, 0, 1);
	} else {
		color_[num_++] = btVector3(1, 1, 0);
	}
	triangle_sphere[1] = LocalCreateRigidBody(btScalar(mass), offset * offset2,
			sphere_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	if (level == -1) {
		color_[num_++] = btVector3(1, 0, 0);
	} else if (level == -2) {
		color_[num_++] = btVector3(0, 1, 0);
	} else if(level == -3){
		color_[num_++] = btVector3(0, 0, 1);
	} else {
		color_[num_++] = btVector3(1, 0, 0);
	}
	offset2.setIdentity();
	offset2.setOrigin(position2);
	triangle_sphere[2] = LocalCreateRigidBody(btScalar(mass), offset * offset2,
			sphere_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	if (level == -1) {
		color_[num_++] = btVector3(1, 0, 0);
	} else if (level == -2) {
		color_[num_++] = btVector3(0, 1, 0);
	} else if(level == -3){
		color_[num_++] = btVector3(0, 0, 1);
	} else {
		color_[num_++] = btVector3(0, 1, 0);
	}
	offset2.setIdentity();
	offset2.setOrigin(position3);
	triangle_sphere[3] = LocalCreateRigidBody(btScalar(mass), offset * offset2,
			sphere_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	if (level == -1) {
		color_[num_++] = btVector3(1, 0, 0);
	} else if (level == -2) {
		color_[num_++] = btVector3(0, 1, 0);
	} else if(level == -3){
		color_[num_++] = btVector3(0, 0, 1);
	} else {
		color_[num_++] = btVector3(0, 0, 1);
	}
	offset2.setIdentity();
	offset2.setOrigin(position10);
	triangle_sphere[4] = LocalCreateRigidBody(btScalar(mass), offset * offset2,
			sphere_shape2);
	object_[num_] = object_num_;
	level_[num_] = level;
	if (level == -1) {
		color_[num_++] = btVector3(1, 0, 0);
	} else if (level == -2) {
		color_[num_++] = btVector3(0, 1, 0);
	} else if(level == -3){
		color_[num_++] = btVector3(0, 0, 1);
	} else {
		color_[num_++] = btVector3(1, 1, 1);
	}

	//ここからside
	offset2.setIdentity();
	offset2.setOrigin(position4);
	offset2.setRotation(btQuaternion(0, 0, PI_ / 6));
	triangle_sides[0] = LocalCreateRigidBody(btScalar(mass), offset * offset2,
			side_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0.9, 0.84, 0.71);
	offset2.setIdentity();
	offset2.setOrigin(position5);
	offset2.setRotation(btQuaternion(0, -PI_ / 6, -PI_ / 12));
	triangle_sides[1] = LocalCreateRigidBody(btScalar(mass), offset * offset2,
			side_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0.9, 0.84, 0.71);
	offset2.setIdentity();
	offset2.setOrigin(position6);
	offset2.setRotation(btQuaternion(0, PI_ / 6, -PI_ / 12));
	triangle_sides[2] = LocalCreateRigidBody(btScalar(mass), offset * offset2,
			side_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0.9, 0.84, 0.71);
	offset2.setIdentity();
	offset2.setOrigin(position7);
	offset2.setRotation(btQuaternion(0, PI_ / 2, 0));
	triangle_sides[3] = LocalCreateRigidBody(btScalar(mass), offset * offset2,
			side_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0.9, 0.84, 0.71);
	offset2.setIdentity();
	offset2.setOrigin(position8);
	offset2.setRotation(btQuaternion(0, PI_ / 2, PI_ / 3));
	triangle_sides[4] = LocalCreateRigidBody(btScalar(mass), offset * offset2,
			side_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0.9, 0.84, 0.71);
	offset2.setIdentity();
	offset2.setOrigin(position9);
	offset2.setRotation(btQuaternion(0, PI_ / 2, -PI_ / 3));
	triangle_sides[5] = LocalCreateRigidBody(btScalar(mass), offset * offset2,
			side_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0.9, 0.84, 0.71);

	//オブジェクトを繋げる
	btHingeConstraint* hingeC;
	btTransform localA, localB, localC;

	localA.setIdentity();
	localB.setIdentity();
	localB = triangle_sphere[0]->getWorldTransform().inverse()
			* triangle_sides[0]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_sphere[0], *triangle_sides[0],
			localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);

	localA.setIdentity();
	localB.setIdentity();
	localB = triangle_sphere[0]->getWorldTransform().inverse()
			* triangle_sides[1]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_sphere[0], *triangle_sides[1],
			localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);

	localA.setIdentity();
	localB.setIdentity();
	localB = triangle_sphere[0]->getWorldTransform().inverse()
			* triangle_sides[2]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_sphere[0], *triangle_sides[2],
			localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);

	localA.setIdentity();
	localB.setIdentity();
	localB = triangle_sphere[1]->getWorldTransform().inverse()
			* triangle_sides[0]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_sphere[1], *triangle_sides[0],
			localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);

	localA.setIdentity();
	localB.setIdentity();
	localB = triangle_sphere[1]->getWorldTransform().inverse()
			* triangle_sides[4]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_sphere[1], *triangle_sides[4],
			localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);

	localA.setIdentity();
	localB.setIdentity();
	localB = triangle_sphere[1]->getWorldTransform().inverse()
			* triangle_sides[5]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_sphere[1], *triangle_sides[5],
			localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);

	localA.setIdentity();
	localB.setIdentity();
	localB = triangle_sphere[2]->getWorldTransform().inverse()
			* triangle_sides[1]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_sphere[2], *triangle_sides[1],
			localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);

	localA.setIdentity();
	localB.setIdentity();
	localB = triangle_sphere[2]->getWorldTransform().inverse()
			* triangle_sides[4]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_sphere[2], *triangle_sides[4],
			localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);

	localA.setIdentity();
	localB.setIdentity();
	localB = triangle_sphere[2]->getWorldTransform().inverse()
			* triangle_sides[3]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_sphere[2], *triangle_sides[3],
			localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);

	localA.setIdentity();
	localB.setIdentity();
	localB = triangle_sphere[3]->getWorldTransform().inverse()
			* triangle_sides[2]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_sphere[3], *triangle_sides[2],
			localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);

	localA.setIdentity();
	localB.setIdentity();
	localB = triangle_sphere[3]->getWorldTransform().inverse()
			* triangle_sides[5]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_sphere[3], *triangle_sides[5],
			localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);

	localA.setIdentity();
	localB.setIdentity();
	localB = triangle_sphere[3]->getWorldTransform().inverse()
			* triangle_sides[3]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_sphere[3], *triangle_sides[3],
			localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);

	localA.setIdentity();
	localB.setIdentity();
	localB = triangle_sphere[0]->getWorldTransform().inverse()
			* triangle_sphere[4]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_sphere[0], *triangle_sphere[4],
			localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);
}

void StageMap::CreateTower(const btVector3& position, int level) {

	btRigidBody* tower;
	float sphere_size;
	float side_size;
	float wide;
	float wide2;
	btTransform trans;

	//レベル分け
	if (level == 5) {
		sphere_size = 1.5;
		side_size = 7;
		wide = 1.0;
		wide2 = 0.6;
	} else if (level == 4) {
		sphere_size = 1.5;
		side_size = 7;
		wide = 1.0;
		wide2 = 0.6;
	} else if (level == 3) {
		sphere_size = 0.8;
		side_size = 2.2;
		wide = 0.6;
		wide2 = 0.3;
	} else if (level == 2) {
		sphere_size = 0.5;
		side_size = 3;
		wide = 0.3;
		wide2 = 0.1;
	} else {
		sphere_size = 0.2;
		side_size = 1;
		wide = 0.1;
		wide2 = 0.05;
	}

	//各オブジェクトの位置を決める
	btVector3 position1(side_size, sphere_size, 0);
	btVector3 position2(-side_size, sphere_size, 0);
	btVector3 position3(side_size / 2, sphere_size, side_size * sqrt(3) / 2);
	btVector3 position4(-side_size / 2, sphere_size, side_size * sqrt(3) / 2);
	btVector3 position5(side_size / 2, sphere_size, -side_size * sqrt(3) / 2);
	btVector3 position6(-side_size / 2, sphere_size, -side_size * sqrt(3) / 2);
	btVector3 position7(0, sphere_size, side_size * sqrt(3));
	btVector3 position8(0, sphere_size, -side_size * sqrt(3));
	btVector3 position9(side_size * 3 / 2, sphere_size,
			side_size * sqrt(3) / 2);
	btVector3 position10(-side_size * 3 / 2, sphere_size,
			side_size * sqrt(3) / 2);
	btVector3 position11(side_size * 3 / 2, sphere_size,
			-side_size * sqrt(3) / 2);
	btVector3 position12(-side_size * 3 / 2, sphere_size,
			-side_size * sqrt(3) / 2);
	btVector3 position13(0, sphere_size, side_size * sqrt(3) / 2);
	btVector3 position14(0, sphere_size, -side_size * sqrt(3) / 2);
	btVector3 position15(side_size * 3 / 4, sphere_size,
			side_size * sqrt(3) / 4);
	btVector3 position16(-side_size * 3 / 4, sphere_size,
			side_size * sqrt(3) / 4);
	btVector3 position17(side_size * 3 / 4, sphere_size,
			-side_size * sqrt(3) / 4);
	btVector3 position18(-side_size * 3 / 4, sphere_size,
			-side_size * sqrt(3) / 4);
	btVector3 position19(0, side_size * sqrt(24) + sphere_size, 0);
	btVector3 position20(side_size / 2, side_size * sqrt(24) / 2 + sphere_size,
			0);
	btVector3 position21(-side_size / 2, side_size * sqrt(24) / 2 + sphere_size,
			0);
	btVector3 position22(side_size / 4, side_size * sqrt(24) / 2 + sphere_size,
			side_size * sqrt(3) / 4);
	btVector3 position23(-side_size / 4, side_size * sqrt(24) / 2 + sphere_size,
			side_size * sqrt(3) / 4);
	btVector3 position24(side_size / 4, side_size * sqrt(24) / 2 + sphere_size,
			-side_size * sqrt(3) / 4);
	btVector3 position25(-side_size / 4, side_size * sqrt(24) / 2 + sphere_size,
			-side_size * sqrt(3) / 4);
	btVector3 position26(0, side_size * sqrt(24) / 2 + sphere_size,
			side_size * sqrt(3) / 2);
	btVector3 position27(0, side_size * sqrt(24) / 2 + sphere_size,
			-side_size * sqrt(3) / 2);
	btVector3 position28(side_size * 3 / 4,
			side_size * sqrt(24) / 2 + sphere_size, side_size * sqrt(3) / 4);
	btVector3 position29(-side_size * 3 / 4,
			side_size * sqrt(24) / 2 + sphere_size, side_size * sqrt(3) / 4);
	btVector3 position30(side_size * 3 / 4,
			side_size * sqrt(24) / 2 + sphere_size, -side_size * sqrt(3) / 4);
	btVector3 position31(-side_size * 3 / 4,
			side_size * sqrt(24) / 2 + sphere_size, -side_size * sqrt(3) / 4);

	//形状設定
	btCollisionShape *sphere_shape = new btSphereShape(sphere_size);
	btCollisionShape *side_shape = new btCapsuleShape(btScalar(wide),
			btScalar(side_size * 5));
	btCollisionShape *side_shape2 = new btCapsuleShape(btScalar(wide2),
			btScalar(side_size * 3));

	btTransform offset;
	offset.setIdentity();
	offset.setOrigin(position);
	btTransform offset2;
	offset2.setIdentity();
	offset2.setOrigin(position19);

	//cubeをbulletに登録
	tower = LocalCreateRigidBody(btScalar(0), offset * offset2, sphere_shape);
	object_[num_] = ++object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0, 0, 0.5);
	offset2.setIdentity();
	offset2.setOrigin(position1);
	tower = LocalCreateRigidBody(btScalar(0), offset * offset2, sphere_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0, 0, 0.5);
	offset2.setIdentity();
	offset2.setOrigin(position2);
	tower = LocalCreateRigidBody(btScalar(0), offset * offset2, sphere_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0, 0, 0.5);
	offset2.setIdentity();
	offset2.setOrigin(position3);
	tower = LocalCreateRigidBody(btScalar(0), offset * offset2, sphere_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0, 0, 0.5);
	offset2.setIdentity();
	offset2.setOrigin(position4);
	tower = LocalCreateRigidBody(btScalar(0), offset * offset2, sphere_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0, 0, 0.5);
	offset2.setIdentity();
	offset2.setOrigin(position5);
	tower = LocalCreateRigidBody(btScalar(0), offset * offset2, sphere_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0, 0, 0.5);
	offset2.setIdentity();
	offset2.setOrigin(position6);
	tower = LocalCreateRigidBody(btScalar(0), offset * offset2, sphere_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0, 0, 0.5);
	offset2.setIdentity();
	offset2.setOrigin(position7);
	tower = LocalCreateRigidBody(btScalar(0), offset * offset2, sphere_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0, 0, 0.5);
	offset2.setIdentity();
	offset2.setOrigin(position8);
	tower = LocalCreateRigidBody(btScalar(0), offset * offset2, sphere_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0, 0, 0.5);
	offset2.setIdentity();
	offset2.setOrigin(position9);
	tower = LocalCreateRigidBody(btScalar(0), offset * offset2, sphere_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0, 0, 0.5);
	offset2.setIdentity();
	offset2.setOrigin(position10);
	tower = LocalCreateRigidBody(btScalar(0), offset * offset2, sphere_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0, 0, 0.5);
	offset2.setIdentity();
	offset2.setOrigin(position11);
	tower = LocalCreateRigidBody(btScalar(0), offset * offset2, sphere_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0, 0, 0.5);
	offset2.setIdentity();
	offset2.setOrigin(position12);
	tower = LocalCreateRigidBody(btScalar(0), offset * offset2, sphere_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0, 0, 0.5);

	//sideをbulletに登録
	offset2.setIdentity();
	offset2.setOrigin(position20);
	offset2.setRotation(btQuaternion(0, 0, 12 * PI_ / 180.0));
	tower = LocalCreateRigidBody(btScalar(0), offset * offset2, side_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0, 1, 0.8);
	offset2.setIdentity();
	offset2.setOrigin(position21);
	offset2.setRotation(btQuaternion(0, 0, -12 * PI_ / 180.0));
	tower = LocalCreateRigidBody(btScalar(0), offset * offset2, side_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0, 1, 0.8);
	offset2.setIdentity();
	offset2.setOrigin(position22);
	offset2.setRotation(btQuaternion(0, -10 * PI_ / 180.0, 6 * PI_ / 180.0));
	tower = LocalCreateRigidBody(btScalar(0), offset * offset2, side_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0, 1, 0.8);
	offset2.setIdentity();
	offset2.setOrigin(position23);
	offset2.setRotation(btQuaternion(0, -10 * PI_ / 180.0, -6 * PI_ / 180.0));
	tower = LocalCreateRigidBody(btScalar(0), offset * offset2, side_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0, 1, 0.8);
	offset2.setIdentity();
	offset2.setOrigin(position24);
	offset2.setRotation(btQuaternion(0, 10 * PI_ / 180.0, 6 * PI_ / 180.0));
	tower = LocalCreateRigidBody(btScalar(0), offset * offset2, side_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0, 1, 0.8);
	offset2.setIdentity();
	offset2.setOrigin(position25);
	offset2.setRotation(btQuaternion(0, 10 * PI_ / 180.0, -6 * PI_ / 180.0));
	tower = LocalCreateRigidBody(btScalar(0), offset * offset2, side_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0, 1, 0.8);
	offset2.setIdentity();
	offset2.setOrigin(position26);
	offset2.setRotation(btQuaternion(0, -20 * PI_ / 180.0, 0));
	tower = LocalCreateRigidBody(btScalar(0), offset * offset2, side_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0, 0.8, 1);
	offset2.setIdentity();
	offset2.setOrigin(position27);
	offset2.setRotation(btQuaternion(0, 20 * PI_ / 180.0, 0));
	tower = LocalCreateRigidBody(btScalar(0), offset * offset2, side_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0, 0.8, 1);
	offset2.setIdentity();
	offset2.setOrigin(position28);
	offset2.setRotation(btQuaternion(0, -10 * PI_ / 180.0, 17 * PI_ / 180.0));
	tower = LocalCreateRigidBody(btScalar(0), offset * offset2, side_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0, 0.8, 1);
	offset2.setIdentity();
	offset2.setOrigin(position29);
	offset2.setRotation(btQuaternion(0, -10 * PI_ / 180.0, -17 * PI_ / 180.0));
	tower = LocalCreateRigidBody(btScalar(0), offset * offset2, side_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0, 0.8, 1);
	offset2.setIdentity();
	offset2.setOrigin(position30);
	offset2.setRotation(btQuaternion(0, 10 * PI_ / 180.0, 17 * PI_ / 180.0));
	tower = LocalCreateRigidBody(btScalar(0), offset * offset2, side_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0, 0.8, 1);
	offset2.setIdentity();
	offset2.setOrigin(position31);
	offset2.setRotation(btQuaternion(0, 10 * PI_ / 180.0, -17 * PI_ / 180.0));
	tower = LocalCreateRigidBody(btScalar(0), offset * offset2, side_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0, 0.8, 1);
	offset2.setIdentity();
	offset2.setOrigin(position13);
	offset2.setRotation(btQuaternion(0, 0, PI_ / 2));
	tower = LocalCreateRigidBody(btScalar(0), offset * offset2, side_shape2);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0, 0.6, 1.0);
	offset2.setIdentity();
	offset2.setOrigin(position14);
	offset2.setRotation(btQuaternion(0, 0.4, PI_ / 2));
	tower = LocalCreateRigidBody(btScalar(0), offset * offset2, side_shape2);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0, 0.6, 1.0);
	offset2.setIdentity();
	offset2.setOrigin(position15);
	offset2.setRotation(btQuaternion(PI_ / 3, 0, PI_ / 2));
	tower = LocalCreateRigidBody(btScalar(0), offset * offset2, side_shape2);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0, 0.6, 1.0);
	offset2.setIdentity();
	offset2.setOrigin(position16);
	offset2.setRotation(btQuaternion(-PI_ / 3, 0, PI_ / 2));
	tower = LocalCreateRigidBody(btScalar(0), offset * offset2, side_shape2);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0, 0.6, 1.0);
	offset2.setIdentity();
	offset2.setOrigin(position17);
	offset2.setRotation(btQuaternion(PI_ * 2 / 3, 0, PI_ / 2));
	tower = LocalCreateRigidBody(btScalar(0), offset * offset2, side_shape2);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0, 0.6, 1.0);
	offset2.setIdentity();
	offset2.setOrigin(position18);
	offset2.setRotation(btQuaternion(-PI_ * 2 / 3, 0, PI_ / 2));
	tower = LocalCreateRigidBody(btScalar(0), offset * offset2, side_shape2);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0, 0.6, 1.0);

	//反発係数
	btScalar rest = 0.0;
	tower->setRestitution(rest);

}

void StageMap::CreatePonde(const btVector3& position, int level) {
	btRigidBody* ponde_sphere[13];
	float r;
	float size;
	float mass;

	//レベル分け
	if (level == 5) {
		r = 8;
		size = 3.5;
		mass = 0.01;
	} else if (level == 4) {
		r = 6.0;
		size = 3.6;
		mass = 0.01;
	} else if (level == 3) {
		r = 9;
		size = 4.5;
		mass = 0.01;
	} else if (level == 2) {
		r = 1.2;
		size = 0.7;
		mass = 0.001;
	} else {
		r = 0.7;
		size = 0.4;
		mass = 0.01;
	}

	btTypedConstraint* joint;

	//位置を設定
	btVector3 position1(r * cos(PI_ * 0 / 4), 0, r * sin(PI_ * 0 / 4));
	btVector3 position2(r * cos(PI_ * 1 / 4), 0, r * sin(PI_ * 1 / 4));
	btVector3 position3(r * cos(PI_ * 2 / 4), 0, r * sin(PI_ * 2 / 4));
	btVector3 position4(r * cos(PI_ * 3 / 4), 0, r * sin(PI_ * 3 / 4));
	btVector3 position5(r * cos(PI_ * 4 / 4), 0, r * sin(PI_ * 4 / 4));
	btVector3 position6(r * cos(PI_ * 5 / 4), 0, r * sin(PI_ * 5 / 4));
	btVector3 position7(r * cos(PI_ * 6 / 4), 0, r * sin(PI_ * 6 / 4));
	btVector3 position8(r * cos(PI_ * 7 / 4), 0, r * sin(PI_ * 7 / 4));

	//形状設定
	btCollisionShape *sphere_shape = new btSphereShape(size);

	//bulletに登
	btTransform offset;
	offset.setIdentity();
	btTransform offset2;
	offset2.setIdentity();
	offset.setOrigin(position);
	offset2.setOrigin(position1);
	ponde_sphere[0] = LocalCreateRigidBody(btScalar(mass), offset * offset2,
			sphere_shape);
	object_[num_] = ++object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0.97, 0.84, 0.52);
	offset2.setOrigin(position2);
	ponde_sphere[1] = LocalCreateRigidBody(btScalar(mass), offset * offset2,
			sphere_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0.97, 0.84, 0.52);
	offset2.setOrigin(position3);
	ponde_sphere[2] = LocalCreateRigidBody(btScalar(mass), offset * offset2,
			sphere_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0.97, 0.84, 0.52);
	offset2.setOrigin(position4);
	ponde_sphere[3] = LocalCreateRigidBody(btScalar(mass), offset * offset2,
			sphere_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0.97, 0.84, 0.52);
	offset2.setOrigin(position5);
	ponde_sphere[4] = LocalCreateRigidBody(btScalar(mass), offset * offset2,
			sphere_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0.97, 0.84, 0.52);
	offset2.setOrigin(position6);
	ponde_sphere[5] = LocalCreateRigidBody(btScalar(mass), offset * offset2,
			sphere_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0.97, 0.84, 0.52);
	offset2.setOrigin(position7);
	ponde_sphere[6] = LocalCreateRigidBody(btScalar(mass), offset * offset2,
			sphere_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0.97, 0.84, 0.52);
	offset2.setOrigin(position8);
	ponde_sphere[7] = LocalCreateRigidBody(btScalar(mass), offset * offset2,
			sphere_shape);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0.97, 0.84, 0.52);

	//オブジェクトを繋げる
	btHingeConstraint* hingeC;
	btTransform localA, localB, localC;

	localA.setIdentity();
	localB.setIdentity();
	localB = ponde_sphere[0]->getWorldTransform().inverse()
			* ponde_sphere[1]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*ponde_sphere[0], *ponde_sphere[1], localB,
			localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);
	localB = ponde_sphere[1]->getWorldTransform().inverse()
			* ponde_sphere[2]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*ponde_sphere[1], *ponde_sphere[2], localB,
			localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);
	localB = ponde_sphere[2]->getWorldTransform().inverse()
			* ponde_sphere[3]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*ponde_sphere[2], *ponde_sphere[3], localB,
			localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);
	localB = ponde_sphere[3]->getWorldTransform().inverse()
			* ponde_sphere[4]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*ponde_sphere[3], *ponde_sphere[4], localB,
			localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);
	localB = ponde_sphere[4]->getWorldTransform().inverse()
			* ponde_sphere[5]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*ponde_sphere[4], *ponde_sphere[5], localB,
			localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);
	localB = ponde_sphere[5]->getWorldTransform().inverse()
			* ponde_sphere[6]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*ponde_sphere[5], *ponde_sphere[6], localB,
			localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);
	localB = ponde_sphere[6]->getWorldTransform().inverse()
			* ponde_sphere[7]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*ponde_sphere[6], *ponde_sphere[7], localB,
			localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);
	localB = ponde_sphere[7]->getWorldTransform().inverse()
			* ponde_sphere[0]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*ponde_sphere[7], *ponde_sphere[0], localB,
			localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

}

void StageMap::CreateMush(const btVector3 &position, int level, int co) {
	btRigidBody* cube_body[5];
	//各oオブジェクトの位置を設定
	float size;
	float mass;

	//レベル分け
	if (level == 5) {
		size = 11;
		mass = 0.0;
	} else if (level == 4) {
		size = 7;
		mass = 0;
	} else if (level == 3) {
		size = 4.8;
		mass = 0;
	} else if (level == 2) {
		size = 2;
		mass = 0;
	} else {
		size = 1;
		mass = 0.1;
	}

	//各オブジェクトの位置設定
	btVector3 position1(0, size * 1.5, 0);
	btVector3 position2(0, size * 2.5, 0);
	btVector3 position3(size, size * 2.5, 0);
	btVector3 position4(-size, size * 2.5, 0);
	btVector3 position5(size * 3.5, 0, 0);

	//形状設定
	btCollisionShape *cube_shape = new btBoxShape(
			btVector3(size / 4, size * 1.5, size / 4));
	btCollisionShape *cube_shape3 = new btBoxShape(
			btVector3(size / 5, size - size / 5, size - size / 5));

	//bulletに登録
	btTransform offset;
	offset.setIdentity();
	btTransform offset2;
	offset2.setIdentity();
	offset.setOrigin(position);
	offset2.setOrigin(position1);
	cube_body[0] = LocalCreateRigidBody(btScalar(mass), offset * offset2,
			cube_shape);
	object_[num_] = ++object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(1, 1, 0);
	offset2.setIdentity();
	offset2.setOrigin(position3);
	cube_body[1] = LocalCreateRigidBody(btScalar(mass), offset * offset2,
			cube_shape3);
	object_[num_] = object_num_;
	level_[num_] = level;
	if (co == 1)
		color_[num_++] = btVector3(1, 0, 0);
	else if (co == 2)
		color_[num_++] = btVector3(1, 0, 1);
	else if (co == 3)
		color_[num_++] = btVector3(1, 1, 0);
	offset2.setIdentity();
	offset2.setOrigin(position4);
	cube_body[2] = LocalCreateRigidBody(btScalar(mass), offset * offset2,
			cube_shape3);
	object_[num_] = object_num_;
	level_[num_] = level;
	if (co == 1)
		color_[num_++] = btVector3(1, 0, 0);
	else if (co == 2)
		color_[num_++] = btVector3(1, 0, 1);
	else if (co == 3)
		color_[num_++] = btVector3(1, 1, 0);
	offset.setRotation(btQuaternion(PI_ / 2, 0, 0));
	offset2.setOrigin(position3);
	cube_body[3] = LocalCreateRigidBody(btScalar(mass), offset * offset2,
			cube_shape3);
	object_[num_] = object_num_;
	level_[num_] = level;
	if (co == 1)
		color_[num_++] = btVector3(1, 0, 0);
	else if (co == 2)
		color_[num_++] = btVector3(1, 0, 1);
	else if (co == 3)
		color_[num_++] = btVector3(1, 1, 0);
	offset2.setOrigin(position4);
	cube_body[4] = LocalCreateRigidBody(btScalar(mass), offset * offset2,
			cube_shape3);
	object_[num_] = object_num_;
	level_[num_] = level;
	if (co == 1)
		color_[num_++] = btVector3(1, 0, 0);
	else if (co == 2)
		color_[num_++] = btVector3(1, 0, 1);
	else if (co == 3)
		color_[num_++] = btVector3(1, 1, 0);
	offset.setIdentity();
	offset.setOrigin(position);
	offset.setRotation(btQuaternion(0, 0, PI_ / 2));
	offset2.setOrigin(position5);
	cube_body[5] = LocalCreateRigidBody(btScalar(mass), offset * offset2,
			cube_shape3);
	object_[num_] = object_num_;
	level_[num_] = level;
	if (co == 1)
		color_[num_++] = btVector3(1, 0, 0);
	else if (co == 2)
		color_[num_++] = btVector3(1, 0, 1);
	else if (co == 3)
		color_[num_++] = btVector3(1, 1, 0);

	//オブジェクトを繋げる
	btHingeConstraint* hingeC;
	btTransform localA, localB, localC;
	btTypedConstraint* joint;

	localA.setIdentity();
	localB.setIdentity();
	localB = cube_body[0]->getWorldTransform().inverse()
			* cube_body[5]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*cube_body[0], *cube_body[5], localB,
			localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);
	localB = cube_body[1]->getWorldTransform().inverse()
			* cube_body[5]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*cube_body[1], *cube_body[5], localB,
			localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);
	localB = cube_body[2]->getWorldTransform().inverse()
			* cube_body[5]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*cube_body[2], *cube_body[5], localB,
			localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);
	localB = cube_body[3]->getWorldTransform().inverse()
			* cube_body[5]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*cube_body[3], *cube_body[5], localB,
			localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);
	localB = cube_body[4]->getWorldTransform().inverse()
			* cube_body[5]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*cube_body[4], *cube_body[5], localB,
			localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	//反発係数
	btScalar cube_rest = 1.3;

	//反発係数設定
	cube_body[5]->setRestitution(cube_rest);

}

void StageMap::CreateApple(const btVector3& position, int level) {
	btRigidBody* apple_body[3];
	float size;
	float size2;
	float mass;

	//レベル分け
	if (level == 5) {
		size = 12;
		size2 = 1;
		mass = 0.01;
	} else if (level == 4) {
		size = 7;
		size2 = 0.8;
		mass = 0.01;
	} else if (level == 3) {
		size = 7;
		size2 = 0.8;
		mass = 0.01;
	} else if (level == 2) {
		size = 1.5;
		size2 = 0.2;
		mass = 0.01;
	} else {
		size = 0.8;
		size2 = 0.1;
		mass = 0.01;
	}

	//各オブジェクトの位置設定
	btVector3 position1(0, size, 0);
	btVector3 position2(0, size * 2 + size2, 0);
	btVector3 position3(0, size * 2 + size2 * 3, 0);

	//形状設定
	btCollisionShape *cube_shape = new btSphereShape(size);
	btCollisionShape *cube_shape2 = new btBoxShape(
			btVector3(size2, size2, size2));
	btCollisionShape *cube_shape3 = new btBoxShape(
			btVector3(size2, size2, size2 * 3));

	//bulletに登録
	btTransform offset;
	offset.setIdentity();
	btTransform offset2;
	offset2.setIdentity();
	offset.setOrigin(position);
	offset2.setOrigin(position1);
	apple_body[0] = LocalCreateRigidBody(btScalar(mass), offset * offset2,
			cube_shape);
	object_[num_] = ++object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(1, 0, 0);
	offset2.setIdentity();
	offset2.setOrigin(position2);
	apple_body[1] = LocalCreateRigidBody(btScalar(mass), offset * offset2,
			cube_shape2);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0.5, 1, 0);
	offset2.setIdentity();
	offset2.setOrigin(position3);
	apple_body[2] = LocalCreateRigidBody(btScalar(mass), offset * offset2,
			cube_shape3);
	object_[num_] = object_num_;
	level_[num_] = level;
	color_[num_++] = btVector3(0.5, 1, 0);

	//オブジェクトを繋げる
	btHingeConstraint* hingeC;
	btTransform localA, localB, localC;
	btTypedConstraint* joint;

	localA.setIdentity();
	localB.setIdentity();
	localB = apple_body[0]->getWorldTransform().inverse()
			* apple_body[1]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*apple_body[0], *apple_body[1], localB,
			localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);
	localB = apple_body[1]->getWorldTransform().inverse()
			* apple_body[2]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*apple_body[1], *apple_body[2], localB,
			localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);
}

//オブジェクトの破壊
int StageMap::DestroyObject(int num, int level) {
	int i;
	int l=0;
	int num2 = num;
	int objectnum = object_[num];
	btCollisionObject* obj;
	btRigidBody* body;

	//オブジェクトのレベルを取得
	if (level_[num] == 5) {
		l = 88;
	} else if (level_[num] == 4) {
		l = 55;
	} else if (level_[num] == 3) {
		l = 40;
	} else if (level_[num] == 2) {
		l = 15;
	}

	//プレイヤーがオブジェクトよりレベルが高いならオブジェクトを破壊
	if (level > l && level_[num] != 0) {
		for (i = 1; num - i >= 0; i++) {
			if (object_[num] == object_[num - i]) {
				num2 = num - i;
			} else
				break;
		}
		//レベル１，２のオブジェクトは再配置
		if (l == 0 || l == 5) {
			//乱数の初期化
			srand((unsigned) time(NULL));
			btVector3 vec = btVector3((rand() % 480 - 240), 160, rand() % 480 - 240);
			for (i = 0; num2 + i <= num_; i++) {
				if (objectnum == object_[num2 + i]) {

					obj = world_->getCollisionObjectArray()[num2 + i];
					body = btRigidBody::upcast(obj);

					btQuaternion qrot(0, 0, 0, 1);
					btDefaultMotionState* sphere_motion_state =
							new btDefaultMotionState(btTransform(qrot, vec));
					body->setMotionState(sphere_motion_state);
					body->activate(true);
				} else
					break;
			}
		} else {
			btVector3 vec2 = btVector3(500, 500, 500);
			//ワールド外へ転送
			for (i = 0; num2 + i <= num_; i++) {
				if (objectnum == object_[num2 + i]) {
					obj = world_->getCollisionObjectArray()[num2 + i];
					body = btRigidBody::upcast(obj);
					btQuaternion qrot(0, 0, 0, 1);
					btDefaultMotionState* sphere_motion_state =
							new btDefaultMotionState(btTransform(qrot, vec2));
					body->setMotionState(sphere_motion_state);
					body->activate(true);
					object_[num2 + i] = -1;
				} else
					break;
			}
		}
		//効果音
		se_absorb_->Play();
	} else {
		return 0;
	}
	if(level_[num] == 1)
		return 3;
	else if(level_[num] == 2)
		return 6;
	else if(level_[num] == 3)
		return 12;
	else if(level_[num] == 4)
		return 24;
	else if(level_[num] == 5)
		return 40;
	else
		return level_[num];
}

//流動体の計算
void StageMap::SetVertexPositions(float waveheight, float offset) {
	int i, j;
	int verts = 5;
	for (i = 0; i < verts; i++) {
		for (j = 0; j < verts; j++) {
			vertices_[i + j * verts].setValue((i - verts * 0.5f) * 66,
					waveheight * sinf((float) i + offset)
							* cosf((float) j + offset),
					(j - verts * 0.5f) * 66);

		}
	}
}

