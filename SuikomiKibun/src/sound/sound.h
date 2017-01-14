#ifndef SUIKOMIKIBUN_SOUND_SOUND_H_
#define SUIKOMIKIBUN_SOUND_SOUND_H_

#include <AL/alut.h>
#include <stdio.h>

#include "../camera/camera.h"
#include "../util/vector3.h"

//sound.h
class Sound {
private:
	//バッファとソース
	ALuint buffer_, source_;
	//フラグ
	bool should_delete_buffer_;
	//エラーフラグ
	bool is_error_;

public:
	//コンストラクタ
	Sound(const char *BGMFileName);
	Sound(ALuint buffer);
	static void SetListener(Vector3 listener_position,
			Vector3 listener_direction, Vector3 listener_up_vec); //リスナー位置設定
	static void SetListener(const Camera3D3P& camera); //リスナー位置設定(カメラクラスを用いる)
	void SetSource(Vector3 source_position) const; //音源位置設定
	void SetSourceToListener(); //音源位置をリスナーと同じ位置に設定
	void SetSourceToListenerFront(float distance); //音源位置をリスナーの正面位置に設定
	void Stream() const;	//毎フレーム呼ぶと音源がループする
	void Play() const;	//呼び出すと再度最初から再生する
	~Sound();	//デストラクタ
};

#endif
