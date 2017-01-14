//グローバルな宣言をここに書く

#ifndef SUIKOMIKIBUN_GV_H_
#define SUIKOMIKIBUN_GV_H_

#include <time.h>
#include <vector>
#include <memory>
#include "util/output_display.h"
#include "util/vector3.h"
#include "util/uGL.h"

extern OutputDisplay output_display0;

//プレイヤーデータ
struct PlayerData {
	Vector3 pos;
	int level;
	int color;
};

enum GameState {
	kConnect, kPlay
};

//ゲームの状態
struct GameData {
	GameState state;
	time_t limited_time;
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
	PlayerData *player_data;
	GameData game_data;
	int client_num;
	//コンストラクタ
	ToClientContainer(int client_num) {
		command = kPlayerDataToClient;
		game_data.state = kConnect;
		this->client_num = client_num;
		player_data = (PlayerData*) malloc(sizeof(PlayerData) * client_num);
	}
	//デストラクタ
	~ToClientContainer() {
		free(player_data);
	}
	//コピーコンストラクタ
	ToClientContainer(const ToClientContainer& o) {
		command = o.command;
		player_data = (PlayerData*) malloc(sizeof(PlayerData) * o.client_num);
		memcpy(player_data, o.player_data, sizeof(PlayerData) * o.client_num);
		game_data = o.game_data;
		client_num = o.client_num;
	}
	//代入演算子オーバーロード
	ToClientContainer &operator=(const ToClientContainer o) {
		if (client_num != o.client_num) {
			uErrorOut(__FILE__, __func__, __LINE__, "コピー先のデータ数が異なります");
			//uExit();
		}
		command = o.command;
		memcpy(player_data, o.player_data, sizeof(PlayerData) * o.client_num);
		game_data = o.game_data;
		client_num = o.client_num;
		return (*this);
	}
};

#endif /* SUIKOMIKIBUN_GV_H_ */
