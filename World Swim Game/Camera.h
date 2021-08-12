#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "DxLib.h"
#include "PlayerActor.h"

class PlayerActor;      //�v���C���[�N���X�ւ̎Q��

class Camera
{
public:
	// �R���X�g���N�^
	Camera(const PlayerActor& playerActor);
	// �f�X�g���N�^
	~Camera();

	// �X�V
	void Update(const PlayerActor& playerActor);

	// �|�W�V������ getter/setter
	const VECTOR& GetPos() const
	{
		return mPos;
	}

private:
	VECTOR mPos;    // �|�W�V����
	VECTOR mTempPos;
};

#endif // !_CAMERA_H_