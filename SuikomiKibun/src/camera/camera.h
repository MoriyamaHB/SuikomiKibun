#ifndef SUIKOMIKIBUN_CAMERA_CAMERA_H_
#define SUIKOMIKIBUN_CAMERA_CAMERA_H_

#include <math.h>
#include <GL/freeglut.h>
#include "../input/input.h"

class Vector3;

//3人称 & 自由飛行のカメラ移動計算を行います
class Camera3D3P {
public:
	Camera3D3P();

	//初期化
	void InitCoordinates();

	//getter
	Vector3 GetStateWatchCoordinates() const;
	Vector3 GetStateCoordinates() const;
	Vector3 GetStateUpCoordinates() const;
	void GetAngle(double *angle_w, double *angle_h) const;
	double get_angle_w() const;
	double get_speed() const;
	//setter
	void set_distance(double distance);

	//更新
	//更新でなるべく最初の方に読んだほうがいいみたい(ライトより後だとライトがおかしくなる)(glulookat)
	void Update(); 					//マウスによる視点移動
	void Update(int dx, int dy); 	//パラメータによるカメラ座標移動
	void Update(float gx, float gy, float gz); 	//パラメータによる視点座標の移動

protected:
	float x_;
	float y_;
	float z_;
	float gx_;
	float gy_;
	float gz_;
	float ux_;
	float uy_;
	float uz_;
	double angle_w_;
	double angle_h_;
	double speed_;
	double distance_;

	//カメラの角度限度
	constexpr static double kMinWrapAngleH = -M_PI / 2 + 0.001;
	constexpr static double kMaxWrapAngleH = M_PI / 2 - 0.001;
	//カメラの感度
	const static int kCameraRotatePx = 4000;

	//更新(内部呼び出し)
	//TransfarByKey()→TransfarAndRotateByMouse()の順に呼び出さないと横移動の時ずれます
	void TransfarByKey();
	void TransfarAndRotateByMouse();
	void TransfarAndRotateByParam(int dx, int dy);
	void SetGluLookAt() const;
};

//1人称 & 自由飛行のカメラ移動計算を行います
//3人称視点の動きと同期できていないため使用しないほうが良い
class Camera3D1P: public Camera3D3P {
public:
	//カメラ移動
	void TransfarAndRotateByMouse();
	void TransfarByKey();
};

#endif
