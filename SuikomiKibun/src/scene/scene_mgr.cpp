#include "scene_mgr.h"

SceneMgr::SceneMgr(Scene Scene) :
		scene_(NULL), next_scene_(Scene) {
	memset(&scene_param_, 0, sizeof(scene_param_));
	MakeNewScene();
}

SceneMgr::SceneMgr(Scene Scene, SceneParam param) :
		scene_(NULL), next_scene_(Scene), scene_param_(param) {
	scene_param_.is_param = true;
	MakeNewScene();
}

SceneMgr::~SceneMgr() {
	delete scene_;
}

//更新
void SceneMgr::Update() {
	if (next_scene_ != kSceneNone) { //次のシーンがセットされていたら
		MakeNewScene();
	}
	scene_->Update();	//シーンの更新
}

//描画
void SceneMgr::Draw() const {
	scene_->Draw();		//シーンの描画
}

//シーン作成
void SceneMgr::MakeNewScene() {
	delete scene_;
	switch (next_scene_) {		//シーンによって処理を分岐
	case kSceneStart:
		scene_ = (BaseScene*) new StartScene(this, scene_param_);
		break;
	case kSceneInputIniInfo:
		scene_ = (BaseScene*) new InputIniInfo(this, scene_param_);
		break;
	case kSceneGame:
		scene_ = (BaseScene*) new GameScene(this, scene_param_);
		break;
	case kSceneServerOnly:
		scene_ = (BaseScene*) new ServerOnlyScene(this, scene_param_);
		break;
	default:
		uErrorOut(__FILE__, __func__, __LINE__, "不明なシーンです、スタート画面に移行します");
		scene_ = (BaseScene*) new StartScene(this, scene_param_);
		break;
	}
	next_scene_ = kSceneNone;   //次のシーン情報をクリア
}

// 引数 nextScene にシーンを変更する(パラーメータ不要時)
void SceneMgr::ChangeScene(Scene NextScene) {
	next_scene_ = NextScene;    //次のシーンをセットする
	memset(&scene_param_, 0, sizeof(scene_param_));    //パラーメータを初期化
}

// 引数 nextScene にシーンを変更する
void SceneMgr::ChangeScene(Scene NextScene, SceneParam param) {
	next_scene_ = NextScene;    //次のシーンをセットする
	scene_param_ = param;		//パラメータを記録
	scene_param_.is_param = true;
}
