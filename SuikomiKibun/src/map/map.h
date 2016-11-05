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
	btCollisionShape* m_shapes[13];
	btRigidBody* m_bodies[13];
	btTypedConstraint* m_joints[12];
	btCollisionShape* a_shapes[3];
	btRigidBody* a_bodies[3];
	btTypedConstraint* a_joints[2];

	void Myinit();
	void RenderScene(int pass);
	btRigidBody* LocalCreateRigidBody (float mass, const btTransform& startTransform, btCollisionShape* shape);
	void CreateSpider(const btVector3& position);
	void Create(const btVector3& position);
public:
	StageMap(btDynamicsWorld* world);
	~StageMap();
	void Update();
	void Draw();

};


#endif
