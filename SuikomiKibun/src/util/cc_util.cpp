#include "cc_util.h"
namespace {
bool has_inited_srand = false;
}

namespace cc_util {

//乱数を取得する
int GetRandom(int min, int max) {
	if (!has_inited_srand) {
		srand((unsigned int) time(NULL));
		has_inited_srand = true;
		rand(); //何故か最初だけ65固定になっていたのでここで一度呼び出しておく
	}
	return min + (int) (rand() * (max - min + 1.0) / (1.0 + RAND_MAX));
}
}
