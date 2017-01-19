/*
 * scene_mgr.h
 *
 *  Created on: 2016/08/29
 *      Author: mhrbykm
 */

#ifndef SUIKOMIKIBUN_SCENE_SCENEMGR_H_
#define SUIKOMIKIBUN_SCENE_SCENEMGR_H_

//各シーンを示す列挙型
typedef enum {
	kSceneStart, //スタート
	kSceneInputIniInfo, //ゲーム初期情報入力
	kSceneGame, //ゲーム
	kSceneBtDemo, //bulletテスト
	kSceneServerOnly, //サーバーだけ
	//無し
	kSceneNone
} Scene;

#include "../gv.h"

//シーン変更時に渡すデータ
typedef struct {
	InputIniInfoData input_ini_info_data;
	bool is_param;	//パラメータを入力したか
} SceneParam;

//シーンを変更するためのインターフェイスクラス
class ISceneChanger {
public:
	virtual ~ISceneChanger() {
	}
	virtual void ChangeScene(Scene NextScene) = 0; //指定シーンに変更する
	virtual void ChangeScene(Scene NextScene, SceneParam param) = 0; //指定シーンに変更する
};

//シーンの基底クラス。
//新しいシーンを作るときはこのクラスを継承してください
class BaseScene {
protected:
	ISceneChanger* scene_changer_;    //クラス所有元にシーン切り替えを伝えるインターフェイス
public:
	BaseScene(ISceneChanger* changer) {
		scene_changer_ = changer;
	}
	virtual ~BaseScene() {
	}
	virtual void Update()=0;
	virtual void Draw() const=0;
};

#include "start.h"
#include "input_ini_info.h"
#include "game.h"
#include "bt_demo.h"
#include "server_only.h"
#include "../util/uGL.h"

//シーン遷移管理クラス
class SceneMgr: public ISceneChanger {
private:
	BaseScene* scene_;    //シーン管理変数
	Scene next_scene_;    //次のシーン管理変数
	SceneParam scene_param_;    //次のシーンに渡すパラメータ

	void MakeNewScene();    //シーンを作成する

public:
	SceneMgr(Scene Scene);
	SceneMgr(Scene Scene, SceneParam param);
	~SceneMgr();
	void Update();    	//更新
	void Draw() const;  //描画

	// 引数 nextScene にシーンを変更する
	void ChangeScene(Scene NextScene);  //パラーメータ不要時
	void ChangeScene(Scene NextScene, SceneParam param);
};

#endif /* SUIKOMIKIBUN_SCENEMGR_H_ */
