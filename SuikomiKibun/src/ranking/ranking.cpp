#include "ranking.h"

Ranking::Ranking() {
	disp = new OutputDisplay("font/kanjuku.otf", 850, 60, 40);
	RankItem r;
	for (int i = 0; i < 3; i++)
		item.push_back(r);
}

Ranking::~Ranking() {
	delete disp;
}

void Ranking::Update(std::string my_name, int my_level, std::string enemy1_name, int enemy1_level,
		std::string enemy2_name, int enemy2_level) {
	//ソート用コンテナに入れる
	item[0].name = my_name;
	item[0].level = my_level;
	item[0].is_me = true;
	item[1].name = enemy1_name;
	item[1].level = enemy1_level;
	item[1].is_me = false;
	item[2].name = enemy2_name;
	item[2].level = enemy2_level;
	item[2].is_me = false;

	//ソート
	std::sort(item.begin(), item.end(), [](const RankItem& x,const RankItem& y) {return x.level > y.level;});

	//描画登録
	disp->Regist("<ランキング>", uColor4fv_yellow);

	float color[4];
	for (auto itr = item.begin(); itr != item.end(); ++itr) {
		if (itr->is_me)
			memcpy(color, uColor4fv_orange, sizeof(float) * 4);
		else
			memcpy(color, uColor4fv_yellow, sizeof(float) * 4);
		disp->Regist(uToStr(itr->level) + "[" + itr->name + "]", color);
	}
}

void Ranking::Draw() const {
	disp->Draw();
}
