#ifndef SUIKOMIKIBUN_OUTPUTDISPLAY_H_
#define SUIKOMIKIBUN_OUTPUTDISPLAY_H_

#include "../util/uGL.h"

namespace output_display {
typedef struct {
	char str[128];
	float color[4];
	int life; //描画回数
	int flag;
} OutputItem;

void Init();
void Regist(const char str[], const float color[], int life = 1);
void Draw();
}

#endif
