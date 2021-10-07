#include "Ueyama_Camera.h"

//-----------------------------------------------------------------------------
// �@�ύX�Ȃ�
//-----------------------------------------------------------------------------

const int CAMERA_ADD_Y = 7;

// �R���X�g���N�^
Ueyama_Camera::Ueyama_Camera(const Ueyama_PlayerActor& playerActor)
{
	// ���s 1.0�`10000 �܂ł��J�����̕`��͈͂Ƃ���
	SetCameraNearFar(1.0f, 10000.0f);

	//�J�����̃|�W�V�����ݒ�
	mPos = VGet(0, 40, 50);
	//�v���C���[�̃|�W�V�������R�s�[����p�̕ϐ���������
	mTempPos = VGet(0, 0, 0);
}

// �f�X�g���N�^
Ueyama_Camera::~Ueyama_Camera()
{

}

// �X�V
void Ueyama_Camera::Update(const Ueyama_PlayerActor& playerActor)
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

	/*if (CheckHitKey(KEY_INPUT_1))
	{
		mPos.x--;
	}
	if (CheckHitKey(KEY_INPUT_2))
	{
		mPos.x++;
	}
	if (CheckHitKey(KEY_INPUT_3))
	{
		mPos.y--;
	}
	if (CheckHitKey(KEY_INPUT_4))
	{
		mPos.y++;
	}
	if (CheckHitKey(KEY_INPUT_5))
	{
		mPos.z--;
	}
	if (CheckHitKey(KEY_INPUT_6))
	{
		mPos.z++;
	}*/

	SetCameraPositionAndTarget_UpVecY(mPos, mPlayerPos);	// �J�����Ɉʒu�𔽉f.
}