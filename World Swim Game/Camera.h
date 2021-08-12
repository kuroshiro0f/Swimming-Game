#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "DxLib.h"
#include "PlayerActor.h"

class PlayerActor;      //プレイヤークラスへの参照

class Camera
{
public:
	// コンストラクタ
	Camera(const PlayerActor& playerActor);
	// デストラクタ
	~Camera();

	// 更新
	void Update(const PlayerActor& playerActor);

	// ポジションの getter/setter
	const VECTOR& GetPos() const
	{
		return mPos;
	}

private:
	VECTOR mPos;    // ポジション
	VECTOR mTempPos;
};

#endif // !_CAMERA_H_