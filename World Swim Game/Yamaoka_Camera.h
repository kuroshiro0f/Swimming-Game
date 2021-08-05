#ifndef _YAMAOKA_CAMERA_H_
#define _YAMAOKA_CAMERA_H_

#include "DxLib.h"

class Yamaoka_Actor;

class Yamaoka_Camera
{
public:
	// コンストラクタ
	Yamaoka_Camera(const Yamaoka_Actor& _actor);
	// デストラクタ
	~Yamaoka_Camera();

	// 更新
	void Update(const Yamaoka_Actor& _actor);

	// ポジションの getter/setter
	const VECTOR& GetPos() const
	{
		return m_pos;
	}
private:
	VECTOR m_pos;     // ポジション
};

#endif // !_YAMAOKA_CAMERA_H_

