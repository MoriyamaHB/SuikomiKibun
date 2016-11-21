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

#define PI_ 3.1415926

class StageMap{
protected:
	GL_ShapeDrawer*	m_shapeDrawer;
	bool	 m_enableshadows;
	btVector3	m_sundirection;
	btScalar	m_defaultContactProcessingThreshold;
	btAlignedObjectArray<btCollisionShape*>	m_collisionShapes;

private:
	btDynamicsWorld* world_;

	int num_;
	btVector3 color_[1000];
	int object_[1000];
	int object_num_;

	void RenderScene(int pass);
	btRigidBody* LocalCreateRigidBody (float mass, const btTransform& startTransform, btCollisionShape* shape);
	void CreateSpider(const btVector3& position);
	void CreateSnowman(const btVector3& position, double size);
	void CreatePyramid(const btVector3& position);
	void CreateTriangle(const btVector3& position);
	void CreateTower(const btVector3& position);

public:
	StageMap(btDynamicsWorld* world);
	~StageMap();
	void Update();
	void Draw();
	void DestroyObject(int num);
};


#endif
