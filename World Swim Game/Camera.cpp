#include "Camera.h"

// �R���X�g���N�^
Camera::Camera(const PlayerActor& playerActor)
{
	// ���s 1.0�`10000 �܂ł��J�����̕`��͈͂Ƃ���
	SetCameraNearFar(1.0f, 10000.0f);

	//�J�����̃|�W�V�����ݒ�
	mPos = VGet(0, 60, 80);
	//�v���C���[�̃|�W�V�������R�s�[����p�̕ϐ���������
	mTempPos = VGet(0, 0, 0);
}

// �f�X�g���N�^
Camera::~Camera()
{

}

// �X�V
void Camera::Update(const PlayerActor& playerActor)
{
	//mTempPos = playerActor.GetPos();								//�v���C���[�̃|�W�V������mTempPos�ɃR�s�[
	mPos.x = playerActor.GetPosX();									//mTempPos(�v���C���[)��x���W���J������x���W�ɑ��

	SetCameraPositionAndTarget_UpVecY(mPos, playerActor.GetPos());	// �J�����Ɉʒu�𔽉f.

}