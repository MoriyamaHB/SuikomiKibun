#ifndef SUIKOMIKIBUN_PLAYERTEKI_H_
#define SUIKOMIKIBUN_PLAYERTEKI_H

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <GL/freeglut.h>
#include <math.h>
#include <string>

#include "../util/uGL.h"
#include "../define.h"
#include "../input/input.h"
#include "../map/map.h"

#include "../map/GL_ShapeDrawer.h"

//衝突コールバック
extern ContactProcessedCallback gContactProcessedCallback;

class PlayerTeki {
private:
	btDynamicsWorld* world_;
	btRigidBody* sphere_body_;
	btScalar player_radius_;
	GL_ShapeDrawer* m_shapeDrawer;
	int playerteki_num_;
	int color_judge_;
	int level_;
	static btRigidBody* delete_body_;
	static btRigidBody* delete_body2_;
	btVector3 color_[3];
	//名前
	std::string name_;
	FTPixmapFont name_font_;
	btVector3 pos_;
public:
	PlayerTeki(btDynamicsWorld* world, btVector3 pos);
	~PlayerTeki();
	void Update(btVector3 pos, int level, int color_change, StageMap* map, std::string name);
	void Draw();
	void RenderScene();
	void PlayerTekiMove(btVector3 pos);
	void PlayerTekiResize(double size);
	void PlayerColorChange(int color_change);
	int get_color() const;
	static bool HandleContactProcess(btManifoldPoint& p, void* a, void* b);
};

#endif /*SUIKOMIKIBUN_PLAYERTEKI_H */
