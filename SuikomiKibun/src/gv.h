//グローバルな宣言をここに書く

#ifndef SUIKOMIKIBUN_GV_H_
#define SUIKOMIKIBUN_GV_H_

#include <time.h>
#include "util/output_display.h"
#include "util/vector3.h"

extern OutputDisplay output_display0;

//ゲーム初期情報
struct InputIniInfoData {
	char s_or_c;
	char server_ip[48];
	int port;
	char client_name[48];
};

//プレイヤーデータ
struct PlayerData {
	//定数
	static const int kNameLength = 24;

	Vector3 pos;
	int level;
	int color;
	char name[kNameLength];
};

enum GameState {
	kConnect, kPlay
};

//勝敗構造体
struct WinLoseData {
	int win_cnt;
	int win_level;
	int lose_cnt;
	int draw_cnt;
};

//ゲームの状態
struct GameData {
	GameState state;
	time_t limited_time;
	WinLoseData win_lose;
};

//送信するデータの種類を示す(toサーバー)
enum CommandToServer {
	kPlayerDataToServer
};

//送信するデータの種類を示す(toクライアント)
enum CommandToClient {
	kPlayerDataToClient
};

//クライアントからサーバーに送信するデータ
struct ToServerContainer {
	CommandToServer command; //データの種類
	PlayerData player_data; //プレイヤーデータ
};

//サーバーからクライアントに送信するデータ
struct ToClientContainer {
public:
	CommandToClient command; //データの種類
	PlayerData player_data[2];
	GameData game_data;
	//コンストラクタ
	ToClientContainer() {
		command = kPlayerDataToClient;
		game_data.state = kConnect;
	}
	//コピーコンストラクタ
	ToClientContainer(const ToClientContainer& o) {
		command = o.command;
		player_data[0] = o.player_data[0];
		player_data[1] = o.player_data[1];
		game_data = o.game_data;
	}
	//代入演算子オーバーロード
	ToClientContainer &operator=(const ToClientContainer o) {
		command = o.command;
		player_data[0] = o.player_data[0];
		player_data[1] = o.player_data[1];
		game_data = o.game_data;
		return (*this);
	}
};

#endif /* SUIKOMIKIBUN_GV_H_ */
