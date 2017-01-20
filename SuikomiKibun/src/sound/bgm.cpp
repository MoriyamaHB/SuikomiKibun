/*
 * bgm.cpp
 *
 *  Created on: 2016/07/10
 *      Author: mhrbykm
 */

#include "bgm.h"

//コンストラクタ
Bgm::Bgm() {
	for (int i = 0; i < Bgm::BgmStructNum; i++) {
		distance_[i] = 10.0;
		pbgm_[i] = NULL;
	}
}

//デストラクタ
Bgm::~Bgm() {
	Stop();
}

//Bgmを鳴らす
void Bgm::Play(BgmStruct sound) {
	if (sound >= 0 && sound < BgmStructNum)
		delete pbgm_[sound];
	switch (sound) {
	case kStartBgm:
		pbgm_[sound] = new Sound("sound/start.wav");
		break;
	case kGameBgm:
		pbgm_[sound] = new Sound("sound/game.wav");
		break;
	case kResultBgm:
		pbgm_[sound] = new Sound("sound/result.wav");
		break;
	default:
		uErrorOut(__FILE__, __func__, __LINE__, "そのBGMは存在しません");
		break;
	}
}

void Bgm::Play(BgmStruct sound, float distance) {
	//BGM開始
	Play(sound);
	//距離設定
	set_distance(sound, distance);
}

//Bgmを更新
void Bgm::Update() {
	for (int i = 0; i < Bgm::BgmStructNum; i++) {
		if (pbgm_[i] == NULL)
			continue;
		pbgm_[i]->SetSourceToListenerFront(distance_[i]); //音源をリスナーに同期
		pbgm_[i]->Stream(); //ループさせる
	}
}

//Bgmを止める
void Bgm::Stop() {
	for (int i = 0; i < Bgm::BgmStructNum; i++) {
		delete pbgm_[i];
		pbgm_[i] = NULL;
	}
}

//setter
void Bgm::set_distance(BgmStruct sound, float d) {
	if (sound >= 0 && sound < BgmStructNum)
		distance_[sound] = d;
	else
		uErrorOut(__FILE__, __func__, __LINE__, "そのBGMは存在しません");
}

