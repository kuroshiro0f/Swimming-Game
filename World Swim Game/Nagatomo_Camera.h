#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "DxLib.h"
#include "Nagatomo_PlayerActor.h"

class Nagatomo_PlayerActor;      //プレイヤークラスへの参照

class Nagatomo_Camera
{
public:
	// コンストラクタ
	Nagatomo_Camera(const Nagatomo_PlayerActor& player);
	// デストラクタ
	~Nagatomo_Camera();

	// 更新
	void Update(const Nagatomo_PlayerActor& player);

	// ポジションの getter/setter
	const VECTOR& GetPos() const
	{
		return pos;
	}

private:
	VECTOR pos;    // ポジション
};

#endif // !_CAMERA_H_

