#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "DxLib.h"
#include "Nagatomo_PlayerActor.h"

class Nagatomo_PlayerActor;      //�v���C���[�N���X�ւ̎Q��

class Nagatomo_Camera
{
public:
	// �R���X�g���N�^
	Nagatomo_Camera(const Nagatomo_PlayerActor& player);
	// �f�X�g���N�^
	~Nagatomo_Camera();

	// �X�V
	void Update(const Nagatomo_PlayerActor& player);

	// �|�W�V������ getter/setter
	const VECTOR& GetPos() const
	{
		return pos;
	}

private:
	VECTOR pos;    // �|�W�V����
};

#endif // !_CAMERA_H_

