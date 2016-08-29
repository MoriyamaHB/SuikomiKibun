#ifndef SUIKOMIKIBUN_CAMERA_CAMERA_H_
#define SUIKOMIKIBUN_CAMERA_CAMERA_H_

#include <math.h>
#include <GL/freeglut.h>
#include "../input/input.h"
#include "../util/output_display.h"

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
	double get_speed() const;

	//setter
	void set_speed(double sp);

	//更新
	//TransfarByKey()→TransfarAndRotateByMouse()の順に呼び出さないと横移動の時ずれます
	void TransfarByKey();
	void TransfarAndRotateByMouse();
	void TransfarAndRotateByParam(int dx, int dy);

	//カメラの情報を表示（速度)
	void DisplayInfo() const;

	//gluLookAtを設定する
	void SetGluLookAt() const;

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
	//カメラの角度限度
	const static double kMinWrapAngleH = -M_PI / 2 + 0.001;
	const static double kMaxWrapAngleH = M_PI / 2 - 0.001;
	//カメラの速度関連
	const static double kAcceleration = 0.012; //加速度
	const static double kMaxSpeed = 0.05; //最高速度
	const static double kMinSpeed = 0; //最低速度
	//速度に応じて視点を遠ざける関連
	const static double kMaxWatchDistance = 2;
	const static double kMinWatchDistance = 1;
	//カメラの感度
	const static int kCameraRotatePx = 4000;

	double distance_;	//現在の距離

	//速度をラップする
	void WrapSpeed();
	//スピードに応じてカメラ距離を変更する
	void UpdateWatchDistance();
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
