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
	//中心座標
	btVector3 ground_pos = btVector3(0, 0, 0);
	btVector3 wall1_pos = btVector3(0, 100, 200);
	btVector3 wall2_pos = btVector3(0, 100, -200);
	btVector3 wall3_pos = btVector3(200, 100, 0);
	btVector3 wall4_pos = btVector3(-200, 100, 0);

	btTransform offset; offset.setIdentity();

	//形状を設定
	btCollisionShape *ground_shape = new btBoxShape(btVector3(200, 0.01, 200));
	btCollisionShape *wall_shape = new btBoxShape(btVector3(200, 100, 0.5));
	btCollisionShape *wall_shape2 = new btBoxShape(btVector3(0.5, 100, 200));

	//bulletに登録（地面＆壁）
	offset.setOrigin(ground_pos);
	ground_body_ = LocalCreateRigidBody(btScalar(0.), offset, ground_shape);
	offset.setIdentity(); offset.setOrigin(wall1_pos);
	wall_body_[0] = LocalCreateRigidBody(btScalar(0.), offset, wall_shape);
	offset.setIdentity(); offset.setOrigin(wall2_pos);
	wall_body_[1] = LocalCreateRigidBody(btScalar(0.), offset, wall_shape);
	offset.setIdentity(); offset.setOrigin(wall3_pos);
	wall_body_[2] = LocalCreateRigidBody(btScalar(0.), offset, wall_shape2);
	offset.setIdentity(); offset.setOrigin(wall4_pos);
	wall_body_[3] = LocalCreateRigidBody(btScalar(0.), offset, wall_shape2);

	//反発係数
	btScalar ground_rest = 0.6;
	btScalar wall_rest = 0.4;

	//反発係数設定
	ground_body_->setRestitution(ground_rest);
	for(int i = 0; i < 4; i++)
	{
		wall_body_[i]->setRestitution(wall_rest);
	}

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
	ground_body_ = new btRigidBody(ground_mass, ground_motion_state, ground_shape, ground_inertia);
	cube_body_ = new btRigidBody(cube_mass, cube_motion_state, cube_shape, cube_inertia);
	cube_body2_ = new btRigidBody(cube_mass, cube_motion_state2, cube_shape, cube_inertia);
	cube_body3_ = new btRigidBody(cube_mass, cube_motion_state3, cube_shape2, cube_inertia);
	cube_body4_ = new btRigidBody(cube_mass, cube_motion_state4, cube_shape2, cube_inertia);
	cube_body5_ = new btRigidBody(cube_mass, cube_motion_state5, cube_shape3, cube_inertia);

	//反発係数
	ground_body_->setRestitution(ground_rest);
	cube_body_->setRestitution(cube_rest);
	cube_body2_->setRestitution(cube_rest);
	cube_body3_->setRestitution(cube_rest);
	cube_body4_->setRestitution(cube_rest);
	cube_body5_->setRestitution(cube_rest);

	//ワールドに剛体オブジェクトを追加

	world_->addRigidBody(ground_body_);
	world_->addRigidBody(cube_body_);
	world_->addRigidBody(cube_body2_);
	world_->addRigidBody(cube_body3_);
	world_->addRigidBody(cube_body4_);
	world_->addRigidBody(cube_body5_);
*/

	btVector3 positionOffset(10, 10, 5);
	CreateSpider(positionOffset);

	btVector3 position_a(10, 13, 25);
	Create(position_a);

	btVector3 position_b(10, 2.5, 40);
	CreatePyramid(position_b);

	btVector3 position_c(10, 20, 50);
	CreateTriangle(position_c);

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

	RenderScene(2);

//	glEnable(GL_LIGHTING);
//	glDepthFunc(GL_LESS);
//	glDisable(GL_STENCIL_TEST);
//	glDisable(GL_CULL_FACE);

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
		btVector3 wireColor(1,1,1); //wants deactivation


		if(i == 0)
			wireColor = btVector3(0.5,0.5,0.5);

		if(i > 4)
			wireColor = btVector3(1.0, 0, 1.0);

		if(i == numObjects - 1)
			wireColor = btVector3(1.0, 0.3, 0);




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
	btCollisionShape* m_shapes[13];
	btTypedConstraint* m_joints[12];
	btVector3 vUP(0, 1, 0);

	float fBodySize =0.75f;
	float fLegLength = 5.45f;
	float fForeLegLength = 5.75f;

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
	m_bodies[0] = LocalCreateRigidBody(btScalar(0.01), offset*transform, m_shapes[0]);

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
		m_bodies[1+2*i] = LocalCreateRigidBody(btScalar(0.01), offset*transform, m_shapes[2+2*i]);

		transform.setIdentity();
		transform.setOrigin(btVector3(btScalar(fCos*(fBodySize+fLegLength)), btScalar(fHeight-0.5*fForeLegLength), btScalar(fSin*(fBodySize+fLegLength))));
		m_bodies[2+2*i] = LocalCreateRigidBody(btScalar(0.01), offset*transform, m_shapes[2+2*i]);
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


void StageMap::Create(const btVector3& position)
{
	btCollisionShape* a_shapes[3];
	btTypedConstraint* a_joints[2];
	btVector3 position_a(10, 10, 25);
	btVector3 position2(10, 14.6, 25);

	a_shapes[0] = new btCapsuleShape(btScalar(1), btScalar(0.1));
	btTransform offset; offset.setIdentity();
	offset.setOrigin(position);
	a_bodies[0] = LocalCreateRigidBody(btScalar(0.01), offset, a_shapes[0]);
	btTransform offset2; offset2.setIdentity();
	offset2.setOrigin(position_a);
	a_shapes[1] = new btCapsuleShape(btScalar(2), btScalar(0.1));
	a_bodies[1] = LocalCreateRigidBody(btScalar(0.01), offset2, a_shapes[1]);

	btTransform offset3; offset3.setIdentity();
	offset3.setOrigin(position2);
	a_shapes[2] = new btCapsuleShape(btScalar(0.5), btScalar(0.1));
	a_bodies[2] = LocalCreateRigidBody(btScalar(0.01), offset3, a_shapes[2]);

	btHingeConstraint* hingeCC;
	btTransform localA, localB, localC;
	localA.setIdentity(); localB.setIdentity();
	//	localA.getBasis().setEulerZYX(0, 0, 0); localA.setOrigin(btVector3(btScalar(6), btScalar(6), btScalar(6)));
	localB = a_bodies[0]->getWorldTransform().inverse() * a_bodies[1]->getWorldTransform() * localA;

	hingeCC = new btHingeConstraint(*a_bodies[0], *a_bodies[1], localB, localA);
	hingeCC->setLimit(btScalar(0), btScalar(0));
	a_joints[0] = hingeCC;
	world_->addConstraint(a_joints[0], true);

	localC = a_bodies[0]->getWorldTransform().inverse() * a_bodies[2]->getWorldTransform() * localA;

	hingeCC = new btHingeConstraint(*a_bodies[0], *a_bodies[2], localC, localA);
	hingeCC->setLimit(btScalar(0), btScalar(0));
	a_joints[1] = hingeCC;
	world_->addConstraint(a_joints[1], true);

}

void StageMap::CreatePyramid(const btVector3& position)
{
	float cube_size = 0.3;
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
	box_body_[0] = LocalCreateRigidBody(btScalar(cube_mass), offset, shape);
	box_body_[1] = LocalCreateRigidBody(btScalar(cube_mass), offset*offset2, shape);
	offset2.setOrigin(position2);
	box_body_[2] = LocalCreateRigidBody(btScalar(cube_mass), offset*offset2, shape);
	offset2.setOrigin(position3);
	box_body_[3] = LocalCreateRigidBody(btScalar(cube_mass), offset*offset2, shape);
	offset2.setOrigin(position4);
	box_body_[4] = LocalCreateRigidBody(btScalar(cube_mass), offset*offset2, shape);
	offset2.setOrigin(position5);
	box_body_[5] = LocalCreateRigidBody(btScalar(cube_mass), offset*offset2, shape);
	offset2.setOrigin(position6);
	box_body_[6] = LocalCreateRigidBody(btScalar(cube_mass), offset*offset2, shape);
	offset2.setOrigin(position7);
	box_body_[7] = LocalCreateRigidBody(btScalar(cube_mass), offset*offset2, shape);
	offset2.setOrigin(position8);
	box_body_[8] = LocalCreateRigidBody(btScalar(cube_mass), offset*offset2, shape);
	offset2.setOrigin(position9);
	box_body_[9] = LocalCreateRigidBody(btScalar(cube_mass), offset*offset2, shape);
	offset2.setOrigin(position10);
	box_body_[10] = LocalCreateRigidBody(btScalar(cube_mass), offset*offset2, shape);
	offset2.setOrigin(position11);
	box_body_[11] = LocalCreateRigidBody(btScalar(cube_mass), offset*offset2, shape);
	offset2.setOrigin(position12);
	box_body_[12] = LocalCreateRigidBody(btScalar(cube_mass), offset*offset2, shape);
	offset2.setOrigin(position13);
	box_body_[13] = LocalCreateRigidBody(btScalar(cube_mass), offset*offset2, shape);

	//cubeを繋げる
	btHingeConstraint* hingeC;
	btTransform localA, localB, localC;

	localA.setIdentity(); localB.setIdentity();
	localB = box_body_[0]->getWorldTransform().inverse() * box_body_[1]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body_[0], *box_body_[1], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body_[1]->getWorldTransform().inverse() * box_body_[2]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body_[1], *box_body_[2], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body_[1]->getWorldTransform().inverse() * box_body_[3]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body_[1], *box_body_[3], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body_[1]->getWorldTransform().inverse() * box_body_[4]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body_[1], *box_body_[4], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body_[2]->getWorldTransform().inverse() * box_body_[5]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body_[2], *box_body_[5], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body_[4]->getWorldTransform().inverse() * box_body_[5]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body_[4], *box_body_[5], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body_[5]->getWorldTransform().inverse() * box_body_[6]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body_[5], *box_body_[6], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body_[5]->getWorldTransform().inverse() * box_body_[7]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body_[5], *box_body_[7], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body_[3]->getWorldTransform().inverse() * box_body_[8]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body_[3], *box_body_[8], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body_[4]->getWorldTransform().inverse() * box_body_[8]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body_[4], *box_body_[8], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body_[8]->getWorldTransform().inverse() * box_body_[9]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body_[8], *box_body_[9], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body_[8]->getWorldTransform().inverse() * box_body_[10]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body_[8], *box_body_[10], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body_[4]->getWorldTransform().inverse() * box_body_[11]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body_[4], *box_body_[11], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body_[10]->getWorldTransform().inverse() * box_body_[11]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body_[10], *box_body_[11], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body_[11]->getWorldTransform().inverse() * box_body_[12]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body_[11], *box_body_[12], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body_[1]->getWorldTransform().inverse() * box_body_[13]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body_[1], *box_body_[13], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body_[5]->getWorldTransform().inverse() * box_body_[13]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body_[5], *box_body_[13], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body_[8]->getWorldTransform().inverse() * box_body_[13]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body_[8], *box_body_[13], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);

	localA.setIdentity(); localB.setIdentity();
	localB = box_body_[11]->getWorldTransform().inverse() * box_body_[13]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*box_body_[11], *box_body_[13], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joint = hingeC;
	world_->addConstraint(joint, true);
}

void StageMap::CreateTriangle(const btVector3& position)
{
	btTypedConstraint* joints_triangel;
	float sphere_size = 0.7;
	float side_size = 8;
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
	triangle_shere_[0] = LocalCreateRigidBody(btScalar(10), offset, sphere_shape);
	triangle_shere_[1] = LocalCreateRigidBody(btScalar(10), offset*offset2, sphere_shape);
	offset2.setIdentity(); offset2.setOrigin(position2);
	triangle_shere_[2] = LocalCreateRigidBody(btScalar(10), offset*offset2, sphere_shape);
	offset2.setIdentity(); offset2.setOrigin(position3);
	triangle_shere_[3] = LocalCreateRigidBody(btScalar(10), offset*offset2, sphere_shape);
	offset2.setIdentity(); offset2.setOrigin(position4);
	offset2.setRotation(btQuaternion(0,0,PI_/6));
	triangle_sides_[0] = LocalCreateRigidBody(btScalar(10), offset*offset2, side_shape);
	offset2.setIdentity(); offset2.setOrigin(position5);
	offset2.setRotation(btQuaternion(0, -PI_/6, -PI_/12));
	triangle_sides_[1] = LocalCreateRigidBody(btScalar(10), offset*offset2, side_shape);
	offset2.setIdentity(); offset2.setOrigin(position6);
	offset2.setRotation(btQuaternion(0, PI_/6, -PI_/12));
	triangle_sides_[2] = LocalCreateRigidBody(btScalar(10), offset*offset2, side_shape);
	offset2.setIdentity(); offset2.setOrigin(position7);
	offset2.setRotation(btQuaternion(0, PI_/2, 0));
	triangle_sides_[3] = LocalCreateRigidBody(btScalar(10), offset*offset2, side_shape);
	offset2.setIdentity(); offset2.setOrigin(position8);
	offset2.setRotation(btQuaternion(0, PI_/2, PI_/3));
	triangle_sides_[4] = LocalCreateRigidBody(btScalar(10), offset*offset2, side_shape);
	offset2.setIdentity(); offset2.setOrigin(position9);
	offset2.setRotation(btQuaternion(0, PI_/2, -PI_/3));
	triangle_sides_[5] = LocalCreateRigidBody(btScalar(10), offset*offset2, side_shape);

	btHingeConstraint* hingeC;
	btTransform localA, localB, localC;

	localA.setIdentity(); localB.setIdentity();
	localB = triangle_shere_[0]->getWorldTransform().inverse() * triangle_sides_[0]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_shere_[0], *triangle_sides_[0], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);

	localA.setIdentity(); localB.setIdentity();
	localB = triangle_shere_[0]->getWorldTransform().inverse() * triangle_sides_[1]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_shere_[0], *triangle_sides_[1], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);

	localA.setIdentity(); localB.setIdentity();
	localB = triangle_shere_[0]->getWorldTransform().inverse() * triangle_sides_[2]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_shere_[0], *triangle_sides_[2], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);

	localA.setIdentity(); localB.setIdentity();
	localB = triangle_shere_[1]->getWorldTransform().inverse() * triangle_sides_[0]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_shere_[1], *triangle_sides_[0], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);

	localA.setIdentity(); localB.setIdentity();
	localB = triangle_shere_[1]->getWorldTransform().inverse() * triangle_sides_[4]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_shere_[1], *triangle_sides_[4], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);

	localA.setIdentity(); localB.setIdentity();
	localB = triangle_shere_[1]->getWorldTransform().inverse() * triangle_sides_[5]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_shere_[1], *triangle_sides_[5], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);

	localA.setIdentity(); localB.setIdentity();
	localB = triangle_shere_[2]->getWorldTransform().inverse() * triangle_sides_[1]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_shere_[2], *triangle_sides_[1], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);

	localA.setIdentity(); localB.setIdentity();
	localB = triangle_shere_[2]->getWorldTransform().inverse() * triangle_sides_[4]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_shere_[2], *triangle_sides_[4], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);

	localA.setIdentity(); localB.setIdentity();
	localB = triangle_shere_[2]->getWorldTransform().inverse() * triangle_sides_[3]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_shere_[2], *triangle_sides_[3], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);

	localA.setIdentity(); localB.setIdentity();
	localB = triangle_shere_[3]->getWorldTransform().inverse() * triangle_sides_[2]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_shere_[3], *triangle_sides_[2], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);

	localA.setIdentity(); localB.setIdentity();
	localB = triangle_shere_[3]->getWorldTransform().inverse() * triangle_sides_[5]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_shere_[3], *triangle_sides_[5], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);

	localA.setIdentity(); localB.setIdentity();
	localB = triangle_shere_[3]->getWorldTransform().inverse() * triangle_sides_[3]->getWorldTransform() * localA;
	hingeC = new btHingeConstraint(*triangle_shere_[3], *triangle_sides_[3], localB, localA);
	hingeC->setLimit(btScalar(0), btScalar(0));
	joints_triangel = hingeC;
	world_->addConstraint(joints_triangel, true);
}


