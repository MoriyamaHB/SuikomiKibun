//グローバルな宣言をここに書く

#ifndef SUIKOMIKIBUN_GV_H_
#define SUIKOMIKIBUN_GV_H_

#include "util/output_display.h"
#include "util/vector3.h"

extern OutputDisplay output_display0;

//プレイヤーデータ
typedef struct {
	Vector3 pos;
	double radius;
	int color;
} PlayerData;

//送信するデータの種類を示す(toサーバー)
enum CommandToServer {
	kPlayerDataToServer
};

//送信するデータの種類を示す(toクライアント)
enum CommandToClient {
	kPlayerDataToClient
};

//クライアントからサーバーに送信するデータ
typedef struct {
	CommandToServer command; //データの種類
	PlayerData player_data; //プレイヤーデータ
} ToServerContainer;

//サーバーからクライアントに送信するデータ
typedef struct {
private:

public:
	CommandToClient command; //データの種類
	PlayerData player_data[2];
} ToClientContainer;

#endif /* SUIKOMIKIBUN_GV_H_ */
