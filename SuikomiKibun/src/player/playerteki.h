

#ifndef SUIKOMIKIBUN_PLAYERTEKI_H_
#define SUIKOMIKIBUN_PLAYERTEKI_H

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <GL/freeglut.h>
#include <math.h>

#include "../util/uGL.h"
#include "../define.h"
#include "../input/input.h"
#include "../map/map.h"

#include "../map/GL_ShapeDrawer.h"



class PlayerTeki{
private:
	btDynamicsWorld* world_;
	btRigidBody* sphere_body_;
	btScalar player_radius_;
	GL_ShapeDrawer* m_shapeDrawer;
	int playerteki_num_;
	int color_judge_;
	btVector3 color_[3];
public:
	PlayerTeki(btDynamicsWorld* world,btVector3 pos);
	~PlayerTeki();
	void Update(btVector3 pos);
	void Draw();
	void RenderScene();
	void PlayerTekiMove(btVector3 pos);
	void PlayerTekiResize(int size);
};


#endif /*SUIKOMIKIBUN_PLAYERTEKI_H */
