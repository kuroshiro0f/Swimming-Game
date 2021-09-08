#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "DxLib.h"
#include "Nagatomo_PlayerActor.h"

class Nagatomo_PlayerActor;      //プレイヤークラスへの参照

class Camera
{
public:
	// コンストラクタ
	Camera(const Nagatomo_PlayerActor& playerActor);
	// デストラクタ
	~Camera();

	// 更新
	void Update(const Nagatomo_PlayerActor& playerActor);

	// ポジションの getter/setter
	const VECTOR& GetPos() const
	{
		return mPos;
	}

private:
	VECTOR mPos;    // ポジション
};

#endif // !_CAMERA_H_