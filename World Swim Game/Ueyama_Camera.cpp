#include "Ueyama_Camera.h"

//-----------------------------------------------------------------------------
// �@�ύX�Ȃ�
//-----------------------------------------------------------------------------


// �R���X�g���N�^
Ueyama_Camera::Ueyama_Camera(const Ueyama_PlayerActor& playerActor)
{
	// ���s 1.0�`10000 �܂ł��J�����̕`��͈͂Ƃ���
	SetCameraNearFar(1.0f, 10000.0f);

	//�J�����̃|�W�V�����ݒ�
	mPos = VGet(0, 60, 80);
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
	//mTempPos = playerActor.GetPos();								//�v���C���[�̃|�W�V������mTempPos�ɃR�s�[
	mPos.x = playerActor.GetPosX();									//mTempPos(�v���C���[)��x���W���J������x���W�ɑ��

	SetCameraPositionAndTarget_UpVecY(mPos, playerActor.GetPos());	// �J�����Ɉʒu�𔽉f.

}