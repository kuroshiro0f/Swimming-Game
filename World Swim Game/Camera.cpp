#include "Camera.h"

const int CAMERA_ADD_Y = 7;

// �R���X�g���N�^
Camera::Camera(const PlayerActor& playerActor)
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
void Camera::Update(const PlayerActor& playerActor)
{
	mPos.x = playerActor.GetPosX();
	mPlayerPos = playerActor.GetPos();
	mPlayerPos.y = playerActor.GetPos().y + CAMERA_ADD_Y;

	if (!playerActor.turnFlag)
	{
		mPlayerPos.x -= mCorrection;
		mPos.x -= mCorrection;
	}
	if (playerActor.turnFlag)
	{
		mPlayerPos.x += mCorrection;
		mPos.x += mCorrection;
	}

	if (playerActor.dCount <= 15)
	{
		mPos.x = playerActor.GetPosX() - 25 + mCorrection;

	}

	SetCameraPositionAndTarget_UpVecY(mPos, mPlayerPos);	// �J�����Ɉʒu�𔽉f.
}