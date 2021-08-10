#include "Nagatomo_PlayerActor.h"

//�R���X�g���N�^
Nagatomo_PlayerActor::Nagatomo_PlayerActor()
	:mNowPlayerState(STATE_IDLE)
	,mNowKeyState(STATE_KEY_IDLE)
	,mPrevKeyState(STATE_KEY_IDLE)
{
	startFlag = true;

	//���f���̃��[�h
	modelHandle = MV1LoadModel("data/model/miku.pmx");

	animTotal = 0.0f;
	animNowTime = 0.0f;
	animIndex = 0;
}

//�f�X�g���N�^
Nagatomo_PlayerActor::~Nagatomo_PlayerActor()
{
	// ���f���̃A�����[�h
	MV1DeleteModel(modelHandle);
}

//�A�N�^�[�̍X�V����
void Nagatomo_PlayerActor::UpdateActor(float _deltaTime)
{
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	if (Key & PAD_INPUT_RIGHT)
	{
		mPrevKeyState = mNowKeyState;
		mNowKeyState = STATE_KEY_RIGHT;

		mPosition = VAdd(mPosition, mVelosity);

	}
	if (Key & PAD_INPUT_LEFT)
	{
		mPrevKeyState = mNowKeyState;
		mNowKeyState = STATE_KEY_LEFT;

		mPosition = VAdd(mPosition, mVelosity);
	}
}

//�X�^�[�g����
void Nagatomo_PlayerActor::StartProcess(float _deltaTime)
{
	//�L�[���͎擾
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	if (mNowPlayerState == STATE_IDLE && timer == 0)
	{
		startFlag = false;
	}

	if (Key & PAD_INPUT_M && startFlag == false)	//�X�y�[�X�L�[
	{
		mNowPlayerState = STATE_SWIM;
	}
}

//�`��
void Nagatomo_PlayerActor::DrawActor()
{
	// 3D���f���̕`��
	MV1DrawModel(modelHandle);

	// 3D���f���̉�]�p�x (�ǂ̕����������Ă��邩)
	MV1SetRotationXYZ(modelHandle, mRotation);
}

//�A�j���[�V����
void Nagatomo_PlayerActor::PlayAnim()
{
	// �j���ł邢��Ƃ�
	if (mNowPlayerState == STATE_SWIM)
	{   // �N���[���A�j���[�V����
		// �Đ����Ԃ�i�߂�
		animNowTime += 0.25f;
		// �Đ����Ԃ��A�j���[�V�����̑��Đ����Ԃ𒴂�����
		if (animNowTime >= animTotal)
		{   // �Đ����Ԃ�0��
			animNowTime = 0;
		}
	}

	// �A�j���[�V�����̍Đ����Ԃ��Z�b�g����
	MV1SetAttachAnimTime(modelHandle, animIndex, animNowTime);
}

//�A�j���[�V�����̃A�^�b�`
void Nagatomo_PlayerActor::attachAnim(int _animPlay)
{
	if (animIndex != -1)
	{   // �A�j���[�V�������폜����
		MV1DetachAnim(modelHandle, animIndex);
	}
	// �A�j���[�V�������A�^�b�`����
	animIndex = MV1AttachAnim(modelHandle, _animPlay);
	// �A�j���[�V�����̑��Đ����Ԃ��擾����
	animTotal = MV1GetAnimTotalTime(modelHandle, animIndex);
}