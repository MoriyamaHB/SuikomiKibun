#include "ranking.h"

Ranking::Ranking() {
	disp_ = new OutputDisplay("font/kanjuku.otf", 850, 60, 40);
	RankItem r;
	for (int i = 0; i < 3; i++)
		item_.push_back(r);
}

Ranking::~Ranking() {
	delete disp_;
}

void Ranking::Update(std::string my_name, int my_level, std::string enemy1_name, int enemy1_level,
		std::string enemy2_name, int enemy2_level) {
	//ソート用コンテナに入れる
	item_[0].name = my_name;
	item_[0].level = my_level;
	item_[0].is_me = true;
	item_[1].name = enemy1_name;
	item_[1].level = enemy1_level;
	item_[1].is_me = false;
	item_[2].name = enemy2_name;
	item_[2].level = enemy2_level;
	item_[2].is_me = false;

	//ソート
	std::sort(item_.begin(), item_.end(), [](const RankItem& x,const RankItem& y) {return x.level > y.level;});

	//描画登録
	disp_->Regist("<ランキング>", uColor4fv_yellow);

	float color[4];
	for (auto itr = item_.begin(); itr != item_.end(); ++itr) {
		if (itr->is_me)
			memcpy(color, uColor4fv_orange, sizeof(float) * 4);
		else
			memcpy(color, uColor4fv_yellow, sizeof(float) * 4);
		disp_->Regist(uToStr(itr->level) + "[" + itr->name + "]", color);
	}
}

void Ranking::Draw() const {
	disp_->Draw();
}
