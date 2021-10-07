#include "Nagatomo_Camera.h"

// �R���X�g���N�^
Nagatomo_Camera::Nagatomo_Camera(const Nagatomo_PlayerActor& playerActor)
{
	// ���s 1.0�`10000 �܂ł��J�����̕`��͈͂Ƃ���
	SetCameraNearFar(1.0f, 10000.0f);

	//�J�����̃|�W�V�����ݒ�
	mPos = VGet(0, 40, 50);
	//�v���C���[�̃|�W�V�������R�s�[����p�̕ϐ���������
	mTempPos = VGet(0, 0, 0);
}

// �f�X�g���N�^
Nagatomo_Camera::~Nagatomo_Camera()
{

}

// �X�V
void Nagatomo_Camera::Update(const Nagatomo_PlayerActor& playerActor)
{
	mPos.x = playerActor.GetPosX();	
	mPlayerPos = playerActor.GetPos();

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