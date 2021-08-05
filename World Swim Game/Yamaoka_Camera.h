#ifndef _YAMAOKA_CAMERA_H_
#define _YAMAOKA_CAMERA_H_

#include "DxLib.h"

class Yamaoka_Actor;

class Yamaoka_Camera
{
public:
	// �R���X�g���N�^
	Yamaoka_Camera(const Yamaoka_Actor& _actor);
	// �f�X�g���N�^
	~Yamaoka_Camera();

	// �X�V
	void Update(const Yamaoka_Actor& _actor);

	// �|�W�V������ getter/setter
	const VECTOR& GetPos() const
	{
		return m_pos;
	}
private:
	VECTOR m_pos;     // �|�W�V����
};

#endif // !_YAMAOKA_CAMERA_H_

