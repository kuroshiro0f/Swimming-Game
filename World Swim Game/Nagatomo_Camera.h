#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "DxLib.h"
#include "Nagatomo_PlayerActor.h"

class Nagatomo_PlayerActor;      //�v���C���[�N���X�ւ̎Q��

class Nagatomo_Camera
{
public:
	// �R���X�g���N�^
	Nagatomo_Camera(const Nagatomo_PlayerActor& playerActor);
	// �f�X�g���N�^
	~Nagatomo_Camera();

	// �X�V
	void Update(const Nagatomo_PlayerActor& playerActor);

	// �|�W�V������ getter/setter
	const VECTOR& GetPos() const
	{
		return mPos;
	}

private:
	VECTOR mPos;    // �|�W�V����
	VECTOR mTempPos;
	VECTOR mPlayerPos;

	const int mCorrection = 12;	//�␳�l
};

#endif // !_CAMERA_H_