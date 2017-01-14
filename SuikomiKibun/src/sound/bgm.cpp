/*
 * bgm.cpp
 *
 *  Created on: 2016/07/10
 *      Author: mhrbykm
 */

#include "bgm.h"
#include <stdio.h>

//Bgmを鳴らす
void Bgm::Play(Bgm::BgmStruct sound) {
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
		fprintf(stderr, "そのBGMは存在しません");
		break;
	}
}

//Bgmを更新
void Bgm::Update() {
	for (int i = 0; i < Bgm::BgmStructNum; i++) {
		if (pbgm_[i] == NULL)
			continue;
		pbgm_[i]->SetSourceToListenerFront(10); //音源をリスナーに同期
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

Bgm::~Bgm() {
	Stop();
}
