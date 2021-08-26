#ifndef _UEYAMA_CAMERA_H_
#define _UEYAMA_CAMERA_H_

#include "DxLib.h"
#include "Ueyama_PlayerActor.h"

class Ueyama_PlayerActor;      //�v���C���[�N���X�ւ̎Q��

class Ueyama_Camera
{
public:
	// �R���X�g���N�^
	Ueyama_Camera(const Ueyama_PlayerActor& playerActor);
	// �f�X�g���N�^
	~Ueyama_Camera();

	// �X�V
	void Update(const Ueyama_PlayerActor& playerActor);

	// �|�W�V������ getter/setter
	const VECTOR& GetPos() const
	{
		return mPos;
	}

private:
	VECTOR mPos;    // �|�W�V����
	VECTOR mTempPos;
};

#endif // !_UEYAMA_CAMERA_H_