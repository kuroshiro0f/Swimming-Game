#include "Nagatomo_Camera.h"

// �R���X�g���N�^
Camera::Camera(const Nagatomo_PlayerActor& playerActor)
{
	// ���s 1.0�`10000 �܂ł��J�����̕`��͈͂Ƃ���
	SetCameraNearFar(1.0f, 10000.0f);

	//�J�����̏����|�W�V�����ݒ�
	mPos = VGet(0, 60, 80);
}

// �f�X�g���N�^
Camera::~Camera()
{

}

// �X�V
void Camera::Update(const Nagatomo_PlayerActor& playerActor)
{							
	//�v���C���[�̃|�W�V������mTempPos�ɃR�s�[
	mPos.x = playerActor.GetPosX();									//mTempPos(�v���C���[)��x���W���J������x���W�ɑ��

	/*VECTOR aimPos = VGet(playerActor.GetPos().x, 60, 80);		//b
	VECTOR posToAim = VSub(aimPos, mPos);	//(b - a)
	VECTOR scaledPosToAim = VScale(posToAim, 0.5f);	//((b-a)*t)
	mPos = VAdd(mPos, scaledPosToAim);*/

	SetCameraPositionAndTarget_UpVecY(mPos, playerActor.GetPos());	// �J�����Ɉʒu�𔽉f.

}
