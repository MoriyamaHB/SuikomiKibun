#ifndef SUIKOMIKIBUN_PLAYER_H_
#define SUIKOMIKIBUN_PLAYER_H

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <GL/freeglut.h>
#include <math.h>

#include "../util/uGL.h"
#include "../define.h"
#include "../input/input.h"

//衝突コールバック
extern ContactProcessedCallback gContactProcessedCallback;

struct TestData {
	int count;
	TestData() : count(0){}
};

class Player{
private:
	btDynamicsWorld* world_;
	btRigidBody* sphere_body_;
	btRigidBody* sphere_body2_;
	btScalar player_radius_;
	TestData    m_BodyData1;
	static btRigidBody* delete_body_;
public:
	Player(btDynamicsWorld* world);
	~Player();
	void Update(double angle);
	void Draw() const;
	Vector3 get_center_pos();
	double get_camera_distance();
	void PlayerSize(double size);
	static bool HandleContactProcess(btManifoldPoint& p, void* a, void* b);
	void DeleteBody(btRigidBody** ppBody);
};


#endif /* MAP_MAP_H_ */
