/*
 * result.h
 *
 *  Created on: 2017/01/22
 *      Author: mhrbykm
 */

#ifndef SUIKOMIKIBUN_RANKING_RESULT_H_
#define SUIKOMIKIBUN_RANKING_RESULT_H_

#include <vector>
#include <algorithm>
#include <iterator>
#include <limits>

#include "ranking.h"
#include "../util/output_display.h"
#include "../util/uGL.h"

class Result {
private:
	std::vector<Ranking::RankItem> item_;
	OutputDisplay *disp_;
public:
	Result();
	~Result();
	void SetData(const std::vector<Ranking::RankItem>& item);
	void Draw();
};

#endif /* RANKING_RESULT_H_ */
