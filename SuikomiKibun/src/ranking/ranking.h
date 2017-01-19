/*
 * ranking.h
 *
 *  Created on: 2017/01/19
 *      Author: mhrbykm
 */

#ifndef SUIKOMIKIBUN_RANKING_RANKING_H_
#define SUIKOMIKIBUN_RANKING_RANKING_H_

#include <string>
#include <vector>
#include <algorithm>
#include <memory>

#include "../util/output_display.h"
#include "../util/uGL.h"

class Ranking {
private:
	struct RankItem {
		std::string name;
		int level;
		bool is_me;
	};

	OutputDisplay *disp;
	std::vector<RankItem> item;
public:
	Ranking();
	~Ranking();
	void Update(std::string my_name, int my_level, std::string enemy1_name, int enemy1_level,
			std::string enemy2_name, int enemy2_level);
	void Draw() const;
};

#endif /* SUIKOMIKIBUN_RANKING_RANKING_H_ */
