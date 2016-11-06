/*
 * map.cpp
 *
 *  Created on: 2016/09/11
 *      Author: mi
 */
#include "map.h"
#include "GlutStuff.h"
#include "LinearMath/btIDebugDraw.h"
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


void StageMap::Myinit(void)
{

	GLfloat light_ambient[] = { btScalar(0.2), btScalar(0.2), btScalar(0.2), btScalar(1.0) };
	GLfloat light_diffuse[] = { btScalar(1.0), btScalar(1.0), btScalar(1.0), btScalar(1.0) };
	GLfloat light_specular[] = { btScalar(1.0), btScalar(1.0), btScalar(1.0), btScalar(1.0 )};
	//		light_position is NOT default value
	GLfloat light_position0[] = { btScalar(1.0), btScalar(10.0), btScalar(1.0), btScalar(0.0 )};
	GLfloat light_position1[] = { btScalar(-1.0), btScalar(-10.0), btScalar(-1.0), btScalar(0.0) };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);

	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);


	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glClearColor(btScalar(0.7),btScalar(0.7),btScalar(0.7),btScalar(0));

	//  glEnable(GL_CULL_FACE);
	//  glCullFace(GL_BACK);
}

//コンストラクタ
StageMap::StageMap(btDynamicsWorld* world)
	:world_(world)
	 ,m_enableshadows(true),
	 m_sundirection(btVector3(1,-2,1)*1000),
	 m_defaultContactProcessingThreshold(BT_LARGE_FLOAT)

{
	//中心座標
	btVector3 ground_pos = btVector3(0, 0, 0);
	btVector3 cube_pos = btVector3(0, 100, 200);
	btVector3 cube_pos2 = btVector3(0, 100, -200);
	btVector3 cube_pos3 = btVector3(200, 100, 0);
	btVector3 cube_pos4 = btVector3(-200, 100, 0);
	btVector3 cube_pos5 = btVector3(27, 3, 55);

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

	btVector3 positionOffset(1, 5, 0);

	CreateSpider(positionOffset);

	btVector3 position_a(10, 13, 10);

	Create(position_a);

	m_shapeDrawer = new GL_ShapeDrawer ();
	m_shapeDrawer->enableTexture(true);
	m_enableshadows = false;

}

//デストラクタ
StageMap::~StageMap(){

	delete ground_body_->getMotionState();
	delete cube_body_->getMotionState();
	delete cube_body2_->getMotionState();
	delete cube_body3_->getMotionState();
	delete cube_body4_->getMotionState();
	delete cube_body5_->getMotionState();
	world_->removeRigidBody(ground_body_);
	world_->removeRigidBody(cube_body_);
	world_->removeRigidBody(cube_body2_);
	world_->removeRigidBody(cube_body3_);
	world_->removeRigidBody(cube_body4_);
	world_->removeRigidBody(cube_body5_);
	delete ground_body_;
	delete cube_body_;
	delete cube_body2_;
	delete cube_body3_;
	delete cube_body4_;
	delete cube_body5_;


	int i;

	// Remove all constraints
	for ( i = 0; i < 6; ++i)
	{
		world_->removeConstraint(m_joints[i]);
		delete m_joints[i]; m_joints[i] = 0;
	}

	// Remove all bodies and shapes
	for ( i = 0; i < 13; ++i)
	{
		world_->removeRigidBody(m_bodies[i]);

		delete m_bodies[i]->getMotionState();

		delete m_bodies[i]; m_bodies[i] = 0;
		delete m_shapes[i]; m_shapes[i] = 0;
	}

	world_->removeConstraint(a_joints[0]);
	world_->removeConstraint(a_joints[1]);
	delete a_joints[0]; a_joints[0] = 0;
	delete a_joints[1]; a_joints[1] = 0;

	for(i = 0; i < 3; i++)
	{
		world_->removeRigidBody(a_bodies[i]);
		delete a_bodies[i]->getMotionState();
		delete a_bodies[i]; a_bodies[i] = 0;
		delete a_shapes[i]; a_shapes[i] = 0;
	}



}

//更新
void StageMap::Update(){

}

//描画
void StageMap::Draw(){
	//	myinit();

	//`glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	world_->debugDrawWorld();

	glClear(GL_STENCIL_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	RenderScene(0);

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
		btVector3 wireColor(1,1,1); //wants deactivation


		if(i == 0)
			wireColor = btVector3(0.5,0.5,0.5);

		if(i > 4)
			wireColor = btVector3(0.27, 0.70, 1.0);


		if(i == numObjects - 1)
			wireColor = btVector3(0, 0.3, 1.0);


		btVector3 aabbMin,aabbMax;
		world_->getBroadphase()->getBroadphaseAabb(aabbMin,aabbMax);

		aabbMin-=btVector3(BT_LARGE_FLOAT,BT_LARGE_FLOAT,BT_LARGE_FLOAT);
		aabbMax+=btVector3(BT_LARGE_FLOAT,BT_LARGE_FLOAT,BT_LARGE_FLOAT);
		//		printf("aabbMin=(%f,%f,%f)\n",aabbMin.getX(),aabbMin.getY(),aabbMin.getZ());
		//		printf("aabbMax=(%f,%f,%f)\n",aabbMax.getX(),aabbMax.getY(),aabbMax.getZ());
		//		m_dynamicsWorld->getDebugDrawer()->drawAabb(aabbMin,aabbMax,btVector3(1,1,1));


		switch(pass)
		{
			case	0:	m_shapeDrawer->drawOpenGL(m,colObj->getCollisionShape(),wireColor,1,aabbMin,aabbMax);break;
			case	1:	m_shapeDrawer->drawShadow(m,m_sundirection*rot,colObj->getCollisionShape(),aabbMin,aabbMax);break;
			case	2:	m_shapeDrawer->drawOpenGL(m,colObj->getCollisionShape(),wireColor*btScalar(0.3),0,aabbMin,aabbMax);break;
		}


	}
}

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
	btVector3 vUP(0, 1, 0);

	float fBodySize =0.5f;
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
	m_bodies[0] = LocalCreateRigidBody(btScalar(1.), offset*transform, m_shapes[0]);

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
		m_bodies[1+2*i] = LocalCreateRigidBody(btScalar(1.), offset*transform, m_shapes[2+2*i]);

		transform.setIdentity();
		transform.setOrigin(btVector3(btScalar(fCos*(fBodySize+fLegLength)), btScalar(fHeight-0.5*fForeLegLength), btScalar(fSin*(fBodySize+fLegLength))));
		m_bodies[2+2*i] = LocalCreateRigidBody(btScalar(1.), offset*transform, m_shapes[2+2*i]);
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
	btVector3 position_a(10, 10, 10);
	btVector3 position2(10, 14.6, 10);
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








































