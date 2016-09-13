/*
 * map.h
 *
 *  Created on: 2016/09/11
 *      Author: mi
 */

#ifndef MAP_MAP_H_
#define MAP_MAP_H_

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <GL/freeglut.h>
#include <math.h>

#include "../util/uGL.h"
#include "../define.h"

class StageMap{
private:
	btDynamicsWorld* world_;
	btRigidBody* ground_body_;
	btRigidBody* cube_body_;
	btRigidBody* cube_body2_;
	btRigidBody* cube_body3_;
	btRigidBody* cube_body4_;
public:
	StageMap(btDynamicsWorld* world);
	~StageMap();
	void Update();
	void Draw() const;
};


#endif /* MAP_MAP_H_ */
