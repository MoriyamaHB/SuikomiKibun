/*
 * map.h
 *
 *  Created on: 2016/09/11
 *      Author: mi
 */

#ifndef SUIKOMIKIBUN_MAP_H_
#define SUIKOMIKIBUN_MAP_H_

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <GL/freeglut.h>
#include <math.h>
#include <LinearMath/btAlignedObjectArray.h>
#include "GL_ShapeDrawer.h"


#include "../util/uGL.h"
#include "../define.h"

class StageMap{
protected:
	GL_ShapeDrawer*	m_shapeDrawer;
	bool	 m_enableshadows;
	btVector3	m_sundirection;
	btScalar	m_defaultContactProcessingThreshold;
	btAlignedObjectArray<btCollisionShape*>	m_collisionShapes;

private:
	btDynamicsWorld* world_;
	btRigidBody* ground_body_;
	btRigidBody* cube_body_;
	btRigidBody* cube_body2_;
	btRigidBody* cube_body3_;
	btRigidBody* cube_body4_;
	btRigidBody* cube_body5_;

	void myinit();
	void renderscene(int pass);
	void render();
	void localCreateRigidBody (btScalar mass, const btTransform& startTransform, btCollisionShape* shape);
public:
	StageMap(btDynamicsWorld* world);
	~StageMap();
	void Update();
	void Draw();

};


#endif
