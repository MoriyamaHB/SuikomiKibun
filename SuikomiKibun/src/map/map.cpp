/*
 * map.cpp
 *
 *  Created on: 2016/09/11
 *      Author: mi
 */
#include "map.h"

void StageMap::myinit(void)
{

	GLfloat light_ambient[] = { btScalar(0.2), btScalar(0.2), btScalar(0.2), btScalar(1.0) };
	GLfloat light_diffuse[] = { btScalar(1.0), btScalar(1.0), btScalar(1.0), btScalar(1.0) };
	GLfloat light_specular[] = { btScalar(1.0), btScalar(1.0), btScalar(1.0), btScalar(1.0 )};
	/*	light_position is NOT default value	*/
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
StageMap::StageMap(btDynamicsWorld* world):world_(world)
{

	//中心座標
	btVector3 ground_pos = btVector3(0, 0, 0);
	btVector3 cube_pos = btVector3(0, 0, 2);
	btVector3 cube_pos2 = btVector3(2,0, 0);
	btVector3 cube_pos3 = btVector3(-2, 0, 0);
	btVector3 cube_pos4 = btVector3(0,0, -2);
	//大きさ
	btVector3 ground_extents = btVector3(30, 0.00001, 30);
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
	btCollisionShape* colShape = new btBoxShape(cube_extents);


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
	m_collisionShapes.push_back(colShape);
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

	m_shapeDrawer = new GL_ShapeDrawer ();
	m_shapeDrawer->enableTexture(true);

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
void StageMap::Draw(){
/*
	btVector3 pos;
	//地面
	glPushMatrix();
	pos = ground_body_->getCenterOfMassPosition();
	glTranslatef(pos[0], pos[1], pos[2]);
	const btBoxShape* ground_shape = static_cast<const btBoxShape*>(ground_body_->getCollisionShape());
	btVector3 ground_half_extent = ground_shape->getHalfExtentsWithMargin();
	uDrawGround(ground_half_extent[0] * 2);
	glPopMatrix();
*/
	myinit();
	renderscene(1);

	glClear(GL_STENCIL_BUFFER_BIT);
	glEnable(GL_CULL_FACE);

	glDisable(GL_LIGHTING);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);
//	glEnable(GL_STENCIL_TEST);
	glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
	glStencilFunc(GL_ALWAYS,1,0xFFFFFFFFL);
	glFrontFace(GL_CCW);
	glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);
	renderscene(1);
	glFrontFace(GL_CW);
	glStencilOp(GL_KEEP,GL_KEEP,GL_DECR);
	renderscene(1);
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
	renderscene(2);
	glEnable(GL_LIGHTING);
	glDepthFunc(GL_LESS);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_CULL_FACE);


/*
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
*/
	glFlush();

}

void	StageMap::renderscene(int pass)
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
		btVector3 wireColor(1.f,1.0f,0.5f); //wants deactivation
		if(i&1) wireColor=btVector3(0.f,0.0f,1.f);
		///color differently for active, sleeping, wantsdeactivation states
		if (colObj->getActivationState() == 1) //active
		{
			if (i & 1)
			{
				wireColor += btVector3 (1.f,0.f,0.f);
			}
			else
			{
				wireColor += btVector3 (.5f,0.f,0.f);
			}
		}
		if(colObj->getActivationState()==2) //ISLAND_SLEEPING
		{
			if(i&1)
			{
				wireColor += btVector3 (0.f,1.f, 0.f);
			}
			else
			{
				wireColor += btVector3 (0.f,0.5f,0.f);
			}
		}

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

