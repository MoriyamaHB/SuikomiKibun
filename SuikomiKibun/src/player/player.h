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

class Player {
private:
	btDynamicsWorld* world_;
	btRigidBody* sphere_body_;
	btScalar player_radius_;
	static btRigidBody* delete_body_;
	static btRigidBody* delete_body2_;
	int pcount;
	int level_;
	GL_ShapeDrawer* m_shapeDrawer;
	int color_judge_;
	btVector3 color_[3];
	int player_num_;
	btCollisionObject* colObj1;
	btRigidBody* sphere_tekibody1_;
	btCollisionObject* colObj2;
	btRigidBody* sphere_tekibody2_;

	void RenderScene();
public:
	Player(btDynamicsWorld* world);
	~Player();
	void Update(double angle, StageMap* map,int color_judge1,int color_judge2,int teki1_level, int teki2_level);
	void Draw();
	btVector3 get_center_pos()const;
	double get_camera_distance()const;
	int get_level()const;
	int get_color()const;
	void PlayerSize(double size);
	void PlayerMove(btVector3 pos);
	void Pwinlosejudge(int color1,int color2,int tekilevel);
	int Pwinlosejudge2(int color1,int color2, int tekilevel);
	void ResMove(btRigidBody* sphere_res_body);
	static bool HandleContactProcess(btManifoldPoint& p, void* a, void* b);
	void DeleteBody(btRigidBody** ppBody);
};

#endif /* MAP_MAP_H_ */
