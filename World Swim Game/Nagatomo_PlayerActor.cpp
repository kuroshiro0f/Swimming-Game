#include "Nagatomo_PlayerActor.h"

//�R���X�g���N�^
Nagatomo_PlayerActor::Nagatomo_PlayerActor()
	:mNowPlayerState(STATE_IDLE)
	,mNowKeyState(STATE_KEY_IDLE)
	,mPrevKeyState(STATE_KEY_IDLE)
{

	startFlag = true;

	mPosition = VGet(150, 18, 0);								// �����ʒu�ݒ�
	mRotation = VGet(250.0f, 90.0f * DX_PI_F / 180.0f, 0.0f);	// ��]�p�x
	mDirection = VGet(0, 0, 1);
	mVelosity = VGet(250, 0, 0);

	//���f���̃��[�h
	modelHandle = MV1LoadModel("data/player/player.pmx");

	animTotal = 0.0f;
	animNowTime = 0.0f;
	animIndex = 0;

	// �S�[���܂ł̋��� //
	dCount = 1000;         // �i�񂾋���
	maxdCount = 1000;      // �S�[��  
	NowPos = 0;            // ���݂̍��W
	// ������          //

	// �X�^�~�i�Q�[�W //
	st = 1250;      // �X�^�~�i�����l
	MaxSt = 1250;   // �X�^�~�i�ő�l
	MinSt = 650;    // �X�^�~�i�ŏ��l
}

//�f�X�g���N�^
Nagatomo_PlayerActor::~Nagatomo_PlayerActor()
{
	// ���f���̃A�����[�h
	MV1DeleteModel(modelHandle);
}

//�X�V����
void Nagatomo_PlayerActor::Update(float _deltaTime)
{
	UpdateActor(_deltaTime);
}

//�A�N�^�[�̍X�V����
void Nagatomo_PlayerActor::UpdateActor(float _deltaTime)
{
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	
	//�X�^�[�g����
	//startFlag��true�̂Ƃ�
	if (startFlag)
	{
		PlayAnim(_deltaTime);								// �A�j���[�V���������擾
		MV1SetPosition(modelHandle, mPosition);				// �|�W�V�����X�V
		StartProcess(_deltaTime);
	}

	//startFlag��false�̂Ƃ�
	if (!startFlag)
	{
		//�j������
		if (Key & PAD_INPUT_RIGHT && mPosition.x <=150 && mPosition.x >= -138)
		{
			mPrevKeyState = mNowKeyState;					//���̃L�[��Ԃ�O��̃L�[��Ԃ�
			mNowKeyState = STATE_KEY_RIGHT;					//���̃L�[��Ԃ�STATE_KEY_RIGHT��

			if (mNowKeyState != mPrevKeyState)				//���ƑO��̃L�[��Ԃ��Ⴄ�Ƃ�
			{
				mPosition.x -= mVelosity.x * _deltaTime;	//mPosition��mVelosity�����Z
				st--;										 // �X�^�~�i�����炷
				dCount--;									 // �c�苗�������炷
			}
		}

		if (Key & PAD_INPUT_LEFT && mPosition.x <=150 && mPosition.x >= -138)
		{
			mPrevKeyState = mNowKeyState;					//���̃L�[��Ԃ�O��̃L�[��Ԃ�
			mNowKeyState = STATE_KEY_LEFT;					//���̃L�[��Ԃ�STATE_KEY_LEFT��

			if (mNowKeyState != mPrevKeyState)				//���ƑO��̃L�[��Ԃ��Ⴄ�Ƃ�
			{
				mPosition.x -= mVelosity.x * _deltaTime;	//mPosition��mVelosity�����Z
				st--;										// �X�^�~�i�����炷
				dCount--;									// �c�苗�������炷
			}
		}
		//�����߂�����
		if (mPosition.x >= 150)
		{
			mPosition.x = 145;
		}

		//�^�[������
		/*if (mPosition.x <= -132 && mPosition.x >= -140 && Key & PAD_INPUT_M)					//�[�܂ōs����
		{
			mRotation = VGet(250.0f, 270.0f * DX_PI_F / 180.0f, 0.0f);							//�v���C���[�𔽓]
			mVelosity = VGet(-100, 0, 0);														//���x�𔽓]
		}*/

		// �c�苗��
		if (dCount <= 500)
		{
			dCount = 500;
		}

		PlayAnim(_deltaTime);						// �A�j���[�V���������擾
		MV1SetPosition(modelHandle, mPosition);		// �|�W�V�����X�V
	}
}

//�X�^�[�g����
void Nagatomo_PlayerActor::StartProcess(float _deltaTime)
{

	//�L�[���͎擾
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	if (Key & PAD_INPUT_M)				//�X�y�[�X�L�[
	{
		mNowPlayerState = STATE_SWIM;	//�v���C���[���j����Ԃ�

		startFlag = false;
		attachAnim(0);					//�A�j���[�V����000���A�^�b�`
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
void Nagatomo_PlayerActor::PlayAnim(float _deltaTime)
{
	// �j���ł邢��Ƃ�
	if (mNowPlayerState == STATE_SWIM)
	{   // �N���[���A�j���[�V����
		// �Đ����Ԃ�i�߂�
		animNowTime += 15.0f * _deltaTime;
		// �Đ����Ԃ��A�j���[�V�����̑��Đ����Ԃ𒴂�����
		if (animNowTime >= animTotal)
		{   // �Đ����Ԃ�0��
			animNowTime = 0;
		}
	}
		
	// �A�j���[�V�����̍Đ����Ԃ��Z�b�g����
	MV1SetAttachAnimTime(modelHandle, animIndex, animNowTime);
}

//�Đ�����A�j���[�V�������A�^�b�`����֐�
void Nagatomo_PlayerActor::attachAnim(int _animPlay)
{
	if (animIndex != -1)
	{   // �A�j���[�V�������폜����
		MV1DetachAnim(modelHandle, animIndex);
	}
	// �A�j���[�V�������A�^�b�`����
	animIndex = MV1AttachAnim(modelHandle, _animPlay,-1,FALSE);
	// �A�j���[�V�����̑��Đ����Ԃ��擾����
	animTotal = MV1GetAnimTotalTime(modelHandle, animIndex);
}

// �X�^�~�i�Q�[�W�̕`��
void Nagatomo_PlayerActor::DrawSt(int _st, int _MaxSt, int _MinSt)
{
	// �F�ݒ�
	int color = GetColor(0, 255, 0);  // ���g(��)
	int color2 = GetColor(0, 0, 0);   // �g (��)

	// �X�^�~�i�Q�[�W�� 0 �ɂȂ�����
	if (_st <= _MinSt)
	{
		// �X�^�~�i���ŏ��l��
		_st = _MinSt;
	}

	// �Q�[�W�̘g�\��
	DrawBox(_MinSt, 1000, _MaxSt, 1035, color2, FALSE);
	// �Q�[�W�̒��g�\��
	DrawBox(_MinSt, 1000, _MaxSt * _st / _MaxSt, 1035, color, TRUE);
	// ���l�\�� 
	DrawFormatString(_MinSt, 1000, GetColor(0, 0, 0), "%d / 600", _MaxSt * _st / _MaxSt - _MinSt);
}

// �S�[���܂ł̋���
void Nagatomo_PlayerActor::DrawToGoal(int _playerPos, int _goalPos)
{
	// �f�o�b�O�p
	//DrawFormatString(1300, 500, GetColor(0, 0, 0), "NowPos  %d", NowPos);
	//DrawFormatString(1300, 550, GetColor(0, 0, 0), "  %d", 600 * _playerPos / _goalPos);

	// �c��̋����̕\��
	DrawBox(1590, 895, 1850, 945, GetColor(0, 255, 255), TRUE);
	DrawFormatString(1600, 900, GetColor(0, 0, 0), "�c��  %d m", 1000 * _playerPos / _goalPos - 500);

	// �ꉝ��������
	if (_playerPos <= 500)
	{
		_playerPos = 500;    // �l���Œ�
		DrawFormatString(900, 950, GetColor(255, 0, 0), "GOAL");
	}

}