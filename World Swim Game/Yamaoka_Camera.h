#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "DxLib.h"
#include "Yamaoka_PlayerActor.h"

class Yamaola_PlayerActor;      //�v���C���[�N���X�ւ̎Q��

class Camera
{
public:
	// �R���X�g���N�^
	Camera(const Yamaoka_PlayerActor& playerActor);
	// �f�X�g���N�^
	~Camera();

	// �X�V
	void Update(const Yamaoka_PlayerActor& playerActor);

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