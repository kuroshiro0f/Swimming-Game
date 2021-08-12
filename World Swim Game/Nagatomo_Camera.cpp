#include "Nagatomo_Camera.h"

// �R���X�g���N�^
Nagatomo_Camera::Nagatomo_Camera(const Nagatomo_PlayerActor& playerActor)
{
	// ���s 1.0�`10000 �܂ł��J�����̕`��͈͂Ƃ���
	SetCameraNearFar(1.0f, 10000.0f);

	//�J�����̃|�W�V�����ݒ�
	mPos = VGet(50, 60, 0);
	mTempPos = mPos;
}

// �f�X�g���N�^
Nagatomo_Camera::~Nagatomo_Camera()
{

}

// �X�V
void Nagatomo_Camera::Update(const Nagatomo_PlayerActor& playerActor)
{
	mPos = VAdd(mTempPos,playerActor.GetPos());

	// �J�����Ɉʒu�𔽉f.
	SetCameraPositionAndTarget_UpVecY(mPos, playerActor.GetPos());

}