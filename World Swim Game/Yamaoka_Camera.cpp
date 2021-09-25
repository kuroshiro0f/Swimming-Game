#include "Yamaoka_Camera.h"

// �R���X�g���N�^
Camera::Camera(const Yamaoka_PlayerActor& playerActor)
{
	// ���s 1.0�`10000 �܂ł��J�����̕`��͈͂Ƃ���
	SetCameraNearFar(1.0f, 10000.0f);

	//�J�����̃|�W�V�����ݒ�
	mPos = VGet(0, 40, 50);
	
	//�v���C���[�̃|�W�V�������R�s�[����p�̕ϐ���������
	mTempPos = VGet(0, 0, 0);
}

// �f�X�g���N�^
Camera::~Camera()
{

}

// �X�V
void Camera::Update(const Yamaoka_PlayerActor& playerActor)
{
	//mTempPos = playerActor.GetPos();			//�v���C���[�̃|�W�V������mTempPos�ɃR�s�[
	mPos.x = playerActor.GetPosX();				//mTempPos(�v���C���[)��x���W���J������x���W�ɑ��

	if (playerActor.dCount <= 15)
	{
		mPos.x = playerActor.GetPosX() - 25;				//mTempPos(�v���C���[)��x���W���J������x���W�ɑ��

	}

	SetCameraPositionAndTarget_UpVecY(mPos, playerActor.GetPos());	// �J�����Ɉʒu�𔽉f.

}

void Camera::Draw()
{
	DrawFormatString(20,  0, GetColor(255, 0, 0), "Pos.x   %f", mPos.x);
	DrawFormatString(20, 40, GetColor(255, 0, 0), "Pos.y   %f", mPos.y);
	DrawFormatString(20, 80, GetColor(255, 0, 0), "Pos.z   %f", mPos.z);
}
