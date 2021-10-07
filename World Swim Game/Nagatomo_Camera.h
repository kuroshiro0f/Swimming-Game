#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "DxLib.h"
#include "Nagatomo_PlayerActor.h"

class Nagatomo_PlayerActor;      //プレイヤークラスへの参照

class Nagatomo_Camera
{
public:
	// コンストラクタ
	Nagatomo_Camera(const Nagatomo_PlayerActor& playerActor);
	// デストラクタ
	~Nagatomo_Camera();

	// 更新
	void Update(const Nagatomo_PlayerActor& playerActor);

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