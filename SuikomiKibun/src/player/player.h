#ifndef SUIKOMIKIBUN_PLAYER_H_
#define SUIKOMIKIBUN_PLAYER_H

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <GL/freeglut.h>
#include <math.h>

#include "../util/uGL.h"
#include "../define.h"
#include "../input/input.h"


class Player{
private:
	btDynamicsWorld* world_;
	btRigidBody* sphere_body_;
	btScalar player_radius_;
public:
	Player(btDynamicsWorld* world);
	~Player();
	void Update(double angle);
	void Draw() const;
	Vector3 get_center_pos();
	double get_camera_distance();
	void PlayerSize(double size);
};


#endif /* MAP_MAP_H_ */
