/*
 * bgm.h
 *
 *  Created on: 2016/07/10
 *      Author: mhrbykm
 */

#ifndef SUIKOMIKIBUN_SOUND_BGM_H_
#define SUIKOMIKIBUN_SOUND_BGM_H_

#include <AL/alut.h>
#include "sound.h"

class Bgm {
public:
	enum BgmStruct {
		kStartBgm = 0, kGameBgm, kResultBgm, BgmStructNum
	};

	Bgm();
	~Bgm();
	//BGMを流す
	void Play(BgmStruct sound);
	void Play(BgmStruct sound, float distance);

	void Update(); //更新(毎フレーム呼び出す)
	void Stop(); //すべてのBGMを止める

	//setter
	void set_distance(BgmStruct sound, float d);

private:
	Sound *pbgm_[BgmStructNum]; //Bgm
	float distance_[BgmStructNum]; //リスナーからの距離
};

#endif /* SOUND_BGM_H_ */
