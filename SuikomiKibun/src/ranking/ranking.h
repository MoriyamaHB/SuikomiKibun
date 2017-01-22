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
public:
	struct RankItem {
		std::string name;
		int level;
		bool is_me;
	};
private:
	OutputDisplay *disp_;
	std::vector<RankItem> item_;
public:
	Ranking();
	~Ranking();
	void Update(std::string my_name, int my_level, std::string enemy1_name, int enemy1_level,
			std::string enemy2_name, int enemy2_level);
	void Draw() const;

	std::vector<RankItem>& get_item();
};

#endif /* SUIKOMIKIBUN_RANKING_RANKING_H_ */
