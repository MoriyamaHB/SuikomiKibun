// sound.cpp
#include "sound.h"

Sound::Sound(const char *BGMFileName) {

	//デストラクタでbufferを削除するべきに設定
	should_delete_buffer_ = true;

	//有効なバッファとソース番号を取得します
	alGenSources(1, &source_);

	//wavファイルから読み込み
	buffer_ = alutCreateBufferFromFile(BGMFileName); //モノラルじゃないと、3D音響にできない
	if (AL_NONE == buffer_) {
		char string[256];
		sprintf(string, "%sの読み込みに失敗しました", BGMFileName);
		fprintf(stderr, string);
		is_error_ = true;
	} else
		is_error_ = false;
	//ソースとバッファを結び付けます
	alSourcei(source_, AL_BUFFER, buffer_);

	//音源の位置
	alSource3f(source_, AL_POSITION, 0.0, 0.0, 0.0);

}

//多くのSoundクラスを生成したり、処理中に生成する場合に,
//バッファから読みこめば軽くできるかどうかわからないが作っておく
Sound::Sound(ALuint buffer) {

	//デストラクタでbufferを削除しないべきに設定
	should_delete_buffer_ = false;

	//有効なバッファとソース番号を取得します
	alGenSources(1, &source_);

	//bufferから読み込み
	buffer_ = buffer; //既存のbufferから読み込み
	if (AL_NONE == buffer_) {
		char string[256];
		sprintf(string, "bufferの読み込みに失敗しました");
		fprintf(stderr, string);
		is_error_ = true;
	} else
		is_error_ = false;

	//ソースとバッファを結び付けます
	alSourcei(source_, AL_BUFFER, buffer_);

	//音源の位置
	alSource3f(source_, AL_POSITION, 0.0, 0.0, 0.0);

}

//音源座標を設定する
void Sound::SetSource(Vector3 source_position) const {
	if(is_error_)
		return;
	alSource3f(source_, AL_POSITION, source_position.x, source_position.y,
			source_position.z);
}

//音源座標をリスナーと同じ場所に設定
//再生する瞬間だけでなく常に同期するために、毎フレーム呼び出すことを推奨
void Sound::SetSourceToListener() {
	if(is_error_)
		return;
	ALfloat x, y, z;
	alGetListener3f(AL_POSITION, &x, &y, &z);
	alSource3f(source_, AL_POSITION, x, y, z);
}

//音源座標をリスナーの正面に設置する
//音量調節に使う
//再生する瞬間だけでなく常に同期するために、毎フレーム呼び出すことを推奨
void Sound::SetSourceToListenerFront(float distance) {
	if(is_error_)
		return;
	//リスナーの座標を取得
	float vec[6];
	alGetListenerfv(AL_ORIENTATION, vec);
	ALfloat x, y, z;
	alGetListener3f(AL_POSITION, &x, &y, &z);
	Vector3 listener_posi(x, y, z);
	Vector3 listener_direction(vec[0], vec[1], vec[2]);
	//リスナーの正面の座標を計算
	Vector3 source_posi = listener_posi + listener_direction * distance;
	//音源を計算した座標に設置
	alSource3f(source_, AL_POSITION, source_posi.x, source_posi.y,
			source_posi.z);
}

//停止していたら再生(bgmに用いる)
//ループしたい時に毎フレーム呼び出す
void Sound::Stream() const {
	if(is_error_)
		return;
	ALint state;
	if (alGetSourcei(source_, AL_SOURCE_STATE, &state), state != AL_PLAYING) {
		alSourcePlay(source_);
	}
}

//再度最初から再生
void Sound::Play() const {
	if(is_error_)
		return;
	alSourcePlay(source_);
}

Sound::~Sound() {
	if(is_error_)
		return;
	//再生されていたら停止
	ALint state;
	if (alGetSourcei(source_, AL_SOURCE_STATE, &state), state == AL_PLAYING)
		alSourceStop(source_);

	//関連付け解除
	alSourcei(source_, AL_BUFFER, 0);

	//終了
	if (should_delete_buffer_)
		alDeleteBuffers(1, &buffer_);
	alDeleteSources(1, &source_);
}

///////////////////// static ////////////////////////////

//リスナーの座標をセットする
//ソース別ではないのでどこかで一度呼べば良い
void Sound::SetListener(Vector3 listener_position, Vector3 listener_direction,
		Vector3 listener_up_vec) {
	// リスナーの位置(x, y, z)
	alListener3f(AL_POSITION, listener_position.x, listener_position.y,
			listener_position.z);

	//リスナーの向きを計算(視点位置から現在地を引き算)
	listener_direction -= listener_position;

	// リスナーの向き(x, y, z)とUPベクトル(x, y, z)
	float vec[6] = { listener_direction.x, listener_direction.y,
			listener_direction.z, listener_up_vec.x, listener_up_vec.y,
			listener_up_vec.z };
	alListenerfv(AL_ORIENTATION, vec);
}

//リスナー位置設定(カメラクラスを用いる)
//ソース別ではないのでどこかで一度呼べば良い
void Sound::SetListener(const Camera3D3P& camera) {
	SetListener(camera.GetStateCoordinates(), camera.GetStateWatchCoordinates(),
			camera.GetStateUpCoordinates());
}
