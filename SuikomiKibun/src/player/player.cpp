#include "player.h"

btRigidBody* Player::delete_body_ = NULL;
btRigidBody* Player::delete_body2_ = NULL;

//コンストラクタ
Player::Player(btDynamicsWorld* world) :
		world_(world) {

	//プレイヤーレベル
	level_ = 1;

	//中心座標
	btVector3 sphere_pos = btVector3(0, 120, 0);
	//大きさ
	player_radius_ = 1.0;
	//質量
	sphere_mass = 0.03;
	//反発係数
	btScalar sphere_rest = 1.0;
	//慣性モーメント
	btVector3 sphere_inertia(0, 0, 0);
	//形状を設定
	btCollisionShape *sphere_shape = new btSphereShape(player_radius_);
	//球体の初期位置、姿勢
	btQuaternion qrot(0, 0, 0, 1);
	btDefaultMotionState* sphere_motion_state = new btDefaultMotionState(
			btTransform(qrot, sphere_pos));

	//慣性モーメントの計算
	sphere_shape->calculateLocalInertia(sphere_mass, sphere_inertia);

	//剛体オブジェクト生成
	sphere_body_ = new btRigidBody(sphere_mass, sphere_motion_state,
			sphere_shape, sphere_inertia);
	//反発係数
	sphere_body_->setRestitution(sphere_rest);
	//すり抜け防止
	sphere_body_->setCcdSweptSphereRadius(player_radius_);
	sphere_body_->setCcdMotionThreshold(player_radius_ * 2.0);

	//ワールドに剛体オブジェクトを追加
	world_->addRigidBody(sphere_body_);

	// 衝突のコールバック関数をセット
	gContactProcessedCallback = Player::HandleContactProcess;

	//撃力を加える回数最大数をセット
	pcount = 5;

	color_[0] = btVector3(1, 0, 0); //赤
	color_[1] = btVector3(0, 1, 0); //緑
	color_[2] = btVector3(0, 0, 1); //青
	color_judge_ = 1;

	player_num_ = world_->getNumCollisionObjects() - 1;

	btCollisionObject* colObj1 = world_->getCollisionObjectArray()[player_num_
			- 1];
	sphere_tekibody1_ = btRigidBody::upcast(colObj1);
	btCollisionObject* colObj2 = world_->getCollisionObjectArray()[player_num_
			- 2];
	sphere_tekibody2_ = btRigidBody::upcast(colObj2);

	//描画
	m_shapeDrawer = new GL_ShapeDrawer();
	m_shapeDrawer->enableTexture(true);

	//効果音
	se_win_ = new Sound("sound/win.wav");
	se_lose_ = new Sound("sound/lose.wav");
	se_draw_ = new Sound("sound/draw.wav");
	se_change_color_ = new Sound("sound/change_color.wav");
}

//デストラクタ
Player::~Player() {
	//オブジェクト破壊
	delete sphere_body_->getMotionState();
	world_->removeRigidBody(sphere_body_);
	delete sphere_body_;
	//効果音
	delete se_win_;
	delete se_lose_;
	delete se_draw_;
	delete se_change_color_;
}

void Player::RenderScene() {
	btScalar m[16];
	btMatrix3x3 rot;
	rot.setIdentity();
	btVector3 wireColor(1, 0, 0);
	btCollisionObject* colObj = world_->getCollisionObjectArray()[player_num_];
	btRigidBody* body = btRigidBody::upcast(colObj);
	if (body && body->getMotionState()) {
		btDefaultMotionState* myMotionState =
				(btDefaultMotionState*) body->getMotionState();
		myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(m);
		rot = myMotionState->m_graphicsWorldTrans.getBasis();
	} else {
		colObj->getWorldTransform().getOpenGLMatrix(m);
		rot = colObj->getWorldTransform().getBasis();
	}

	if (color_judge_ == 1)
		wireColor = color_[0];
	else if (color_judge_ == 2)
		wireColor = color_[1];
	else if (color_judge_ == 3)
		wireColor = color_[2];

	btVector3 aabbMin, aabbMax;
	world_->getBroadphase()->getBroadphaseAabb(aabbMin, aabbMax);

	aabbMin -= btVector3(BT_LARGE_FLOAT, BT_LARGE_FLOAT, BT_LARGE_FLOAT);
	aabbMax += btVector3(BT_LARGE_FLOAT, BT_LARGE_FLOAT, BT_LARGE_FLOAT);

	m_shapeDrawer->drawOpenGL(m, colObj->getCollisionShape(),
			wireColor * btScalar(0.3), 0, aabbMin, aabbMax);

}

//更新
void Player::Update(double angle, StageMap* map, int color_judge1,
		int color_judge2, int teki1_level, int teki2_level, Vector3 teki1_pos,
		Vector3 teki2_pos, double teki1_radius, double teki2_radius) {
	//撃力を加える
	btVector3 impulse;
	static btVector3 pos;
	double t = 0.2;
	static int upcount = 0;
	static int pflug = 1;



	if (input::get_keyboard_frame('w') >= 1) {
		if (pcount > 0 && pflug == 1) {
			sphere_body_->activate(true);
			impulse.setValue(t * cos(angle), 0, t * sin(angle));
			sphere_body_->applyCentralImpulse(impulse);
			//pcount--;
			upcount = 0;
		}
	}else{
	    sphere_body_->setLinearVelocity(btVector3(0,0,0));
		impulse.setValue(0,-2 * sphere_mass * 9.8, 0);
		sphere_body_->applyCentralForce(impulse);
	}


	if (input::get_keyboard_frame('s') >= 1) {
		sphere_body_->activate(true);
		impulse.setValue(t * cos(angle + M_PI), 0, t * sin(angle + M_PI));
		sphere_body_->applyCentralImpulse(impulse);
	}
	if (input::get_keyboard_frame('a') >= 1) {
		sphere_body_->activate(true);
		impulse.setValue(t * cos(angle - M_PI / 2.0), 0,
				t * sin(angle - M_PI / 2.0));
		sphere_body_->applyCentralImpulse(impulse);
	}
	if (input::get_keyboard_frame('d') >= 1) {
		sphere_body_->activate(true);
		impulse.setValue(t * cos(angle + M_PI / 2.0), 0,
				t * sin(angle + M_PI / 2.0));
		sphere_body_->applyCentralImpulse(impulse);
	}
	if (input::get_keyboard_frame(' ') == 1) {
		sphere_body_->activate(true);
		impulse.setValue(0, t, 0);
		sphere_body_->applyCentralImpulse(impulse);
	}

	//効果音
	se_win_->SetSourceToListener();
	se_lose_->SetSourceToListener();
	se_draw_->SetSourceToListener();
	se_change_color_->SetSourceToListener();

	if (pflug == 0)
		upcount++;

	if (pcount <= 0)
		pflug = 0;
	if (upcount == 10) {
		upcount = 0;
		if (pcount < 5)
			pcount++;
		if (pcount == 5)
			pflug = 1;
	}

	int i;
	btCollisionObject* obj;
	btRigidBody* body;

	if (uIsCollisionBallAndBall(sphere_body_->getCenterOfMassPosition(),
			(double) level_ / 5, teki1_pos, teki1_radius, NULL))
		Player::Pwinlosejudge(color_judge_, color_judge1, teki1_level);
	if (uIsCollisionBallAndBall(sphere_body_->getCenterOfMassPosition(),
			(double) level_ / 5, teki2_pos, teki2_radius, NULL))
		Player::Pwinlosejudge(color_judge_, color_judge1, teki2_level);

	if (sphere_body_ == delete_body_ && sphere_tekibody1_ == delete_body2_) {
		//Player::Pwinlosejudge(color_judge_,color_judge1,teki1_level);
	} else if (sphere_body_ == delete_body2_
			&& sphere_tekibody1_ == delete_body_) {
		//Player::Pwinlosejudge(color_judge_,color_judge1,teki1_level);
	} else if (sphere_body_ == delete_body_
			&& sphere_tekibody2_ == delete_body2_) {
		//Player::Pwinlosejudge(color_judge_,color_judge2,teki2_level);
	} else if (sphere_body_ == delete_body2_
			&& sphere_tekibody2_ == delete_body_) {
		//Player::Pwinlosejudge(color_judge_,color_judge2,teki2_level);
	} else if (sphere_body_ == delete_body_ || sphere_body_ == delete_body2_) {
		if (sphere_body_ == delete_body_)
			delete_body_ = delete_body2_;
		if (delete_body_ != NULL) {
			for (i = world_->getNumCollisionObjects() - 1; i > 141; i--) {
				obj = world_->getCollisionObjectArray()[i];
				body = btRigidBody::upcast(obj);
				if (delete_body_ == body) {
					int level_item = map->DestroyObject(i, level_);
					if (level_item < 0) {
						se_change_color_->Play();
						color_judge_ = ColorChange(level_item);
					} else
						level_ += level_item;
				}
			}
		}

	} else if (sphere_tekibody1_ == delete_body_
			|| sphere_tekibody1_ == delete_body2_) {
		if (sphere_tekibody1_ == delete_body_)
			delete_body_ = delete_body2_;
		if (delete_body_ != NULL) {
			for (i = world_->getNumCollisionObjects() - 1; i > 141; i--) {
				obj = world_->getCollisionObjectArray()[i];
				body = btRigidBody::upcast(obj);
				if (delete_body_ == body) {
					map->DestroyObject(i, level_);
				}
			}
		}
	} else if (sphere_tekibody2_ == delete_body_
			|| sphere_tekibody2_ == delete_body2_) {
		if (sphere_tekibody2_ == delete_body_)
			delete_body_ = delete_body2_;
		if (delete_body_ != NULL) {
			for (i = world_->getNumCollisionObjects() - 1; i > 141; i--) {
				obj = world_->getCollisionObjectArray()[i];
				body = btRigidBody::upcast(obj);
				if (delete_body_ == body) {
					map->DestroyObject(i, level_);
				}
			}
		}
	}

	if (player_radius_ <= (double) level_ / 3.0)
		PlayerSize(player_radius_ += 0.05);

	delete_body_ = NULL;
	delete_body2_ = NULL;

	if (uOutOfRange(sphere_body_->getCenterOfMassPosition(),
			btVector3(-280, -50, -280), btVector3(280, 600, 280)))
		ResMove(sphere_body_);
}

//描画
void Player::Draw() {
	int i, n = 200;
	float x, y, r = 80, r2 = 110, r3 = 76;
	double rate;

	//中心円作成
	u3Dto2D();
	glBegin(GL_POLYGON); // ポリゴンの描画
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	// 円を描画
	for (i = 0; i < n; i++) {
		// 座標を計算
		rate = (double) i / n;
		x = r3 * cos(2.0 * M_PI * rate) + 110;
		y = r3 * sin(2.0 * M_PI * rate) + 110;
		glVertex3f(x, y, 0); // 頂点座標を指定
	}
	glEnd(); // ポリゴンの描画終了
	u2Dto3D();

	//中心円作成
	u3Dto2D();
	glBegin(GL_POLYGON); // ポリゴンの描画
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	// 円を描画
	for (i = 0; i < n; i++) {
		// 座標を計算
		rate = (double) i / n;
		x = r * cos(2.0 * M_PI * rate) + 110;
		y = r * sin(2.0 * M_PI * rate) + 110;
		glVertex3f(x, y, 0); // 頂点座標を指定
	}
	glEnd(); // ポリゴンの描画終了
	u2Dto3D();

	u3Dto2D();
	for (i = 0; i < pcount; i++) {
		if (pcount == 5) {
			glBegin(GL_POLYGON); // ポリゴンの描画
			glColor4f(0.3f, 1.0f, 0.9f, 1.0f);
			// 円を描画
			for (i = 0; i < n; i++) {
				// 座標を計算
				rate = (double) i / n;
				if (2.0 * M_PI * rate >= 0 && 2.0 * M_PI * rate <= M_PI * 5 / 3)
					r2 = 0;
				else
					r2 = 110;

				x = r2 * cos(2.0 * M_PI * rate) + 110;
				y = r2 * sin(2.0 * M_PI * rate) + 110;
				glVertex3f(x, y, 0); // 頂点座標を指定
			}
			glEnd(); // ポリゴンの描画終了
		}

		if (pcount == 5 || pcount == 4) {
			glBegin(GL_POLYGON); // ポリゴンの描画
			glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
			for (i = 0; i < n; i++) {
				// 座標を計算
				rate = (double) i / n;
				if (2.0 * M_PI * rate >= M_PI * 2 / 9
						&& 2.0 * M_PI * rate <= M_PI * 2)
					r2 = 0;
				else
					r2 = 110;

				x = r2 * cos(2.0 * M_PI * rate) + 110;
				y = r2 * sin(2.0 * M_PI * rate) + 110;
				glVertex3f(x, y, 0); // 頂点座標を指定
			}
			glEnd(); // ポリゴンの描画終了
		}
		if (pcount == 5 || pcount == 4 || pcount == 3) {
			glBegin(GL_POLYGON); // ポリゴンの描画
			glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
			for (i = 0; i < n; i++) {
				// 座標を計算
				rate = (double) i / n;
				if ((2.0 * M_PI * rate >= M_PI * 2 * 7 / 36
						&& 2.0 * M_PI * rate <= M_PI * 2)
						|| (2.0 * M_PI * rate >= 0
								&& 2.0 * M_PI * rate <= M_PI * 2 / 9))
					r2 = 0;
				else
					r2 = 110;

				x = r2 * cos(2.0 * M_PI * rate) + 110;
				y = r2 * sin(2.0 * M_PI * rate) + 110;
				glVertex3f(x, y, 0); // 頂点座標を指定
			}
			glEnd(); // ポリゴンの描画終了
		}
		if (pcount == 5 || pcount == 4 || pcount == 3 || pcount == 2) {
			glBegin(GL_POLYGON); // ポリゴンの描画
			glColor4f(1.0f, 0.5f, 0.0f, 1.0f);
			for (i = 0; i < n; i++) {
				// 座標を計算
				rate = (double) i / n;
				if ((2.0 * M_PI * rate >= M_PI * 2 * 47 / 180
						&& 2.0 * M_PI * rate <= M_PI * 2)
						|| (2.0 * M_PI * rate >= 0
								&& 2.0 * M_PI * rate <= M_PI * 2 * 7 / 36))
					r2 = 0;
				else
					r2 = 110;

				x = r2 * cos(2.0 * M_PI * rate) + 110;
				y = r2 * sin(2.0 * M_PI * rate) + 110;
				glVertex3f(x, y, 0); // 頂点座標を指定
			}
			glEnd(); // ポリゴンの描画終了
		}
		if (pcount == 5 || pcount == 4 || pcount == 3 || pcount == 2
				|| pcount == 1) {
			glBegin(GL_POLYGON); // ポリゴンの描画
			glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
			for (i = 0; i < n; i++) {
				// 座標を計算
				rate = (double) i / n;
				if ((2.0 * M_PI * rate >= M_PI * 2 * 57 / 180
						&& 2.0 * M_PI * rate <= M_PI * 2)
						|| (2.0 * M_PI * rate >= 0
								&& 2.0 * M_PI * rate <= M_PI * 2 * 47 / 180))
					r2 = 0;
				else
					r2 = 110;

				x = r2 * cos(2.0 * M_PI * rate) + 110;
				y = r2 * sin(2.0 * M_PI * rate) + 110;
				glVertex3f(x, y, 0); // 頂点座標を指定
			}
			glEnd(); // ポリゴンの描画終了
		}
	}
	u2Dto3D();

	glDisable(GL_LIGHTING);
	RenderScene();
}

void Player::PlayerSize(double size) {
	//形状を設定
	btCollisionShape *new_sphere_shape = new btSphereShape(size);
	delete sphere_body_->getCollisionShape();
	sphere_body_->setCollisionShape(new_sphere_shape);
	player_radius_ = size;
	//すり抜け防止
	sphere_body_->setCcdSweptSphereRadius(player_radius_);
	sphere_body_->setCcdMotionThreshold(player_radius_ * 2.0);
}

void Player::PlayerMove(btVector3 pos) {
	btVector3 t_pos = sphere_body_->getCenterOfMassPosition();
	btVector3 i_pos = sphere_body_->getCenterOfMassPosition();
	t_pos[0] = pos[0] - i_pos[0];
	t_pos[1] = i_pos[1] = 0;
	t_pos[2] = pos[2] - i_pos[2];
	sphere_body_->translate(pos);
}

btVector3 Player::get_center_pos() const {
	return sphere_body_->getCenterOfMassPosition();
}

int Player::get_level() const {
	return level_;
}

double Player::get_camera_distance() const {
	return player_radius_ * 3;
}

int Player::get_color() const {
	return color_judge_;
}

void Player::DeleteBody(btRigidBody** ppBody) {
	btRigidBody* pBody = *ppBody;
	world_->removeRigidBody(pBody);
	if (pBody) {
		delete pBody->getMotionState();
	}
	delete pBody;
	*ppBody = NULL;
}

bool Player::HandleContactProcess(btManifoldPoint& p, void* a, void* b) {
	delete_body_ = static_cast<btRigidBody*>(a);
	delete_body2_ = static_cast<btRigidBody*>(b);
	return true;
}

void Player::Pwinlosejudge(int color1, int color2, int tekilevel) {
	if (color1 == color2) {
		if (tekilevel > level_) {
			se_win_->Play();
			ResMove(sphere_body_);
		} else if (tekilevel < level_) {
			level_ += (tekilevel / 2) + 1;
			se_lose_->Play();
		} else
			se_draw_->Play();
	} else if (color1 == 2 && color2 == 1) {
		se_win_->Play();
		ResMove(sphere_body_);
	} else if (color1 == 3 && color2 == 2) {
		se_win_->Play();
		ResMove(sphere_body_);
	} else if (color1 == 1 && color2 == 3) {
		se_win_->Play();
		ResMove(sphere_body_);
	} else if (color1 == 1 && color2 == 2) {
		se_lose_->Play();
		level_ += (tekilevel / 2) + 1;
	} else if (color1 == 2 && color2 == 3) {
		se_lose_->Play();
		level_ += (tekilevel / 2) + 1;
	} else if (color1 == 3 && color2 == 1) {
		se_lose_->Play();
		level_ += (tekilevel / 2) + 1;
	}
}

int Player::ColorChange(int colorchange) {
	if (colorchange == -1)
		return 1;
	else if (colorchange == -2)
		return 2;
	else if (colorchange == -3)
		return 3;
	else
		return cc_util::GetRandom(1, 3);
}

void Player::ResMove(btRigidBody* sphere_res_body_) {
	player_radius_ = 1;
	level_ = 1;
	color_judge_ = cc_util::GetRandom(1, 3);
	sphere_res_body_->clearForces();
	btVector3 pos = btVector3(0, 120, 0);
	btQuaternion qrot(0, 0, 0, 1);
	btDefaultMotionState* sphere_motion_state = new btDefaultMotionState(
			btTransform(qrot, pos));
	sphere_res_body_->setMotionState(sphere_motion_state);
}

