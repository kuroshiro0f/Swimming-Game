#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "DxLib.h"
#include "Yamaoka_PlayerActor.h"

class Yamaola_PlayerActor;      //プレイヤークラスへの参照

class Camera
{
public:
	// コンストラクタ
	Camera(const Yamaoka_PlayerActor& playerActor);
	// デストラクタ
	~Camera();

	// 更新
	void Update(const Yamaoka_PlayerActor& playerActor);

	// ポジションの getter/setter
	const VECTOR& GetPos() const
	{
		return mPos;
	}

private:
	VECTOR mPos;    // ポジション
	VECTOR mTempPos;
	VECTOR mPlayerPos;

	const int mCorrection = 12;	//補正値
};

#endif // !_CAMERA_H_