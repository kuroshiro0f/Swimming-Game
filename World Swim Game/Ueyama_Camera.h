#ifndef _UEYAMA_CAMERA_H_
#define _UEYAMA_CAMERA_H_

#include "DxLib.h"
#include "Ueyama_PlayerActor.h"

class Ueyama_PlayerActor;      //プレイヤークラスへの参照

class Ueyama_Camera
{
public:
	// コンストラクタ
	Ueyama_Camera(const Ueyama_PlayerActor& playerActor);
	// デストラクタ
	~Ueyama_Camera();

	// 更新
	void Update(const Ueyama_PlayerActor& playerActor);

	// ポジションの getter/setter
	const VECTOR& GetPos() const
	{
		return mPos;
	}

private:
	VECTOR mPos;    // ポジション
	VECTOR mTempPos;
};

#endif // !_UEYAMA_CAMERA_H_