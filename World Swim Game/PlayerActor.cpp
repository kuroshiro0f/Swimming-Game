#include "PlayerActor.h"

PlayerActor::PlayerActor()
	:mNowPlayerState(STATE_IDLE)
	, mNowKeyState(STATE_KEY_IDLE)
	, mPrevKeyState(STATE_KEY_IDLE)
{
	startFlag = true;

	mPosition = VGet(0, 18, -150);								// �����ʒu�ݒ�
	mRotation = VGet(250.0f, 180.0f * DX_PI_F / 180.0f, 0.0f);	// ��]�p�x
	mDirection = VGet(0, 0, 1);
	//���f���̃��[�h
	modelHandle = MV1LoadModel("data/player/player.pmx");

	animTotal = 0.0f;
	animNowTime = 0.0f;
	animIndex = 0;
}

PlayerActor::~PlayerActor()
{
	// ���f���̃A�����[�h
	MV1DeleteModel(modelHandle);
}

void PlayerActor::Update(float _deltaTime)
{
	UpdateActor(_deltaTime);
}

void PlayerActor::UpdateActor(float _deltaTime)
{
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	//�X�^�[�g����
	if (startFlag)
	{
		StartProcess(_deltaTime);
	}

	//�j������
	if (Key & PAD_INPUT_RIGHT)
	{
		mPrevKeyState = mNowKeyState;					//���̃L�[��Ԃ�O��̃L�[��Ԃ�
		mNowKeyState = STATE_KEY_RIGHT;					//���̃L�[��Ԃ�STATE_KEY_RIGHT��

		if (mNowKeyState != mPrevKeyState)				//���ƑO��̃L�[��Ԃ��Ⴄ�Ƃ�
		{
			mPosition = VAdd(mPosition, mVelosity);
		}

	}
	if (Key & PAD_INPUT_LEFT)
	{
		mPrevKeyState = mNowKeyState;					//���̃L�[��Ԃ�O��̃L�[��Ԃ�
		mNowKeyState = STATE_KEY_LEFT;					//���̃L�[��Ԃ�STATE_KEY_LEFT��

		if (mNowKeyState != mPrevKeyState)				//���ƑO��̃L�[��Ԃ��Ⴄ�Ƃ�
		{
			mPosition = VAdd(mPosition, mVelosity);
		}
	}

	PlayAnim();
	MV1SetPosition(modelHandle, mPosition);
}

void PlayerActor::StartProcess(float _deltaTime)
{
	startFlag = false;

	//�L�[���͎擾
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	if (Key & PAD_INPUT_M)				//�X�y�[�X�L�[
	{
		mNowPlayerState = STATE_SWIM;	//�v���C���[���j����Ԃ�

		attachAnim(0);					//�A�j���[�V����000���A�^�b�`
	}
}

void PlayerActor::DrawActor()
{
	// 3D���f���̕`��
	MV1DrawModel(modelHandle);

	// 3D���f���̉�]�p�x (�ǂ̕����������Ă��邩)
	MV1SetRotationXYZ(modelHandle, mRotation);
}

void PlayerActor::PlayAnim()
{
	// �j���ł邢��Ƃ�
	if (mNowPlayerState == STATE_SWIM)
	{   // �N���[���A�j���[�V����
		// �Đ����Ԃ�i�߂�
		animNowTime += 0.5f;
		// �Đ����Ԃ��A�j���[�V�����̑��Đ����Ԃ𒴂�����
	}
	if (animNowTime >= animTotal)
	{   // �Đ����Ԃ�0��
		animNowTime = 0;
	}

	// �A�j���[�V�����̍Đ����Ԃ��Z�b�g����
	MV1SetAttachAnimTime(modelHandle, animIndex, animNowTime);
}

void PlayerActor::attachAnim(int _animPlay)
{
	if (animIndex != -1)
	{   // �A�j���[�V�������폜����
		MV1DetachAnim(modelHandle, animIndex);
	}
	// �A�j���[�V�������A�^�b�`����
	animIndex = MV1AttachAnim(modelHandle, _animPlay, -1, FALSE);
	// �A�j���[�V�����̑��Đ����Ԃ��擾����
	animTotal = MV1GetAnimTotalTime(modelHandle, animIndex);
}
