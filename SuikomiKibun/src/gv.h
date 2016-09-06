//グローバルな宣言をここに書く

#ifndef SUIKOMIKIBUN_GV_H_
#define SUIKOMIKIBUN_GV_H_

#include "util/output_display.h"
#include "util/vector3.h"

extern OutputDisplay output_display0;

typedef struct {
	Vector3 pos;
} ClientData;

typedef struct {
	Vector3 pos;
} ServerData;

#endif /* SUIKOMIKIBUN_GV_H_ */
