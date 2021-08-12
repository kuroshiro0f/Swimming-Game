#include "Camera.h"

// �R���X�g���N�^
Camera::Camera(const PlayerActor& playerActor)
{
	// ���s 1.0�`10000 �܂ł��J�����̕`��͈͂Ƃ���
	SetCameraNearFar(1.0f, 10000.0f);

	//�J�����̃|�W�V�����ݒ�
	mPos = VGet(50, 60, 0);
	mTempPos = mPos;
}

// �f�X�g���N�^
Camera::~Camera()
{

}

// �X�V
void Camera::Update(const PlayerActor& playerActor)
{
	mPos = VAdd(mTempPos, playerActor.GetPos());

	// �J�����Ɉʒu�𔽉f.
	SetCameraPositionAndTarget_UpVecY(mPos, playerActor.GetPos());

}