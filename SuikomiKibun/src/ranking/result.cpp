#include "result.h"

Result::Result() {
	disp_ = new OutputDisplay("font/kanjuku.otf", 300, 150, 90);
}

Result::~Result() {

}

void Result::SetData(const std::vector<Ranking::RankItem>& item) {
	item_.clear();
	std::copy(item.begin(), item.end(), std::back_inserter(item_));
}

void Result::Draw() {
	//描画登録
	disp_->Regist("<結果>", uColor4fv_yellow);

	float color[4];
	int i = 0;
	int prev_level = -99;
	for (auto itr = item_.begin(); itr != item_.end(); ++itr) {
		//順位をすすめる
		if (prev_level != itr->level)
			i++;
		//前のプレイヤーのレベルを記録
		prev_level = itr->level;
		//色
		if (itr->is_me)
			memcpy(color, uColor4fv_orange, sizeof(float) * 4);
		else
			memcpy(color, uColor4fv_yellow, sizeof(float) * 4);
		//描画登録
		disp_->Regist(uToStr(i) + "位: " + itr->name + "(" + uToStr(itr->level) + ")", color);
	}

	//描画
	disp_->Draw();
}
