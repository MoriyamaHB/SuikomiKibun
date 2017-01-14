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
	void Play(BgmStruct sound); //BGMを流す
	void Update(); //更新(毎フレーム呼び出す)
	void Stop(); //すべてのBGMを止める
	~Bgm();
private:
	Sound *pbgm_[BgmStructNum]; //Bgm
};

#endif /* SOUND_BGM_H_ */
