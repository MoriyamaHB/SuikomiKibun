#ifndef SUIKOMIKIBUN_PLAYER_H_
#define SUIKOMIKIBUN_PLAYER_H

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <GL/freeglut.h>
#include <math.h>

#include "../util/uGL.h"
#include "../define.h"
#include "../input/input.h"
#include "../map/map.h"

#include "../map/GL_ShapeDrawer.h"


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
	static btRigidBody* delete_body2_;
	int pcount;
	double level_;
	GL_ShapeDrawer*	m_shapeDrawer;
	int color_judge_;
	btVector3 color_[3];
	void RenderScene();
public:
	Player(btDynamicsWorld* world);
	~Player();
	void Update(double angle, StageMap* map);
	void Draw();
	Vector3 get_center_pos();
	double get_camera_distance();
	void PlayerSize(double size);
	void PlayerMove(btVector3 pos);
	static bool HandleContactProcess(btManifoldPoint& p, void* a, void* b);
	void DeleteBody(btRigidBody** ppBody);
};

class PlayerTeki{
private:
	btDynamicsWorld* world_;
	btRigidBody* sphere_body_;
	btScalar player_radius_;
	static btRigidBody* delete_body_;
	static btRigidBody* delete_body2_;
public:
	PlayerTeki(btDynamicsWorld* world,btVector3 pos);
	~PlayerTeki();
	void Update(btVector3 pos);
	void PlayerTekiMove(btVector3 pos);
};


#endif /* MAP_MAP_H_ */
