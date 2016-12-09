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
private:
	GL_ShapeDrawer*	m_shapeDrawer;
	btDynamicsWorld* world_;

	int num_;
	btVector3 color_[2000];
	int object_[2000];
	int level_[2000];
	int object_num_;

	void RenderScene(void);
	btRigidBody* LocalCreateRigidBody (float mass, const btTransform& startTransform, btCollisionShape* shape);
	void CreateSpider(const btVector3& position, int level);
	void CreateSnowman(const btVector3& position, int level);
	void CreatePyramid(const btVector3& position, int level);
	void CreateTriangle(const btVector3& position, int level);
	void CreateTower(const btVector3& position, int level);
	void CreatePonde(const btVector3& position, int level);
	void CreateMush(const btVector3& position, int level);
	void CreateApple(const btVector3& position, int level);
	void CreateDango(const btVector3& position, int level);
public:
	StageMap(btDynamicsWorld* world);
	~StageMap();
	void Update();
	void Draw();
	int DestroyObject(int num, int level);
};


#endif
