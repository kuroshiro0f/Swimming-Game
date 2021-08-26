#include "Yamaoka_PlayerActor.h"

const float D_COUNT = 0.36f;      //�@�w�Z�i�ヂ�j�^�[�j
//const float D_COUNT = 0.235f;      //�@�w�Z�i�����j�^�[�j

// �R���X�g���N�^
Yamaoka_PlayerActor::Yamaoka_PlayerActor()
	: mNowPlayerState(STATE_IDLE)
	, mNowKeyState(STATE_KEY_IDLE)
	, mPrevKeyState(STATE_KEY_IDLE)
	, startTime(0)
	, tmpTime(0)
	, countUP(0)
{
	// �t���O�� false ��
	startFlag = false;
	turnFlag = false;

	mPosition = VGet(150, 18, 0);								// �����ʒu�ݒ�
	mRotation = VGet(250.0f, 90.0f * DX_PI_F / 180.0f, 0.0f);	// ��]�p�x
	mDirection = VGet(0, 0, 1);
	mVelosity = VGet(250, 0, 0);

	// ���f���̃��[�h
	modelHandle = MV1LoadModel("data/player/player.pmx");

	animTotal = 0.0f;
	animNowTime = 0.0f;
	animIndex = 0;

	//NowPos = 0;            // ���݂̍��W

	// �S�[���܂ł̋����@( 25m ) 
	//dCount = 25.0f;         // �i�񂾋���
	//maxdCount = 25.0f;      // �S�[��  

	dCount = 50.0f;         // �i�񂾋���
	maxdCount = 50.0f;      // �S�[��  

	// �X�^�~�i�Q�[�W  (MaxSt - MinSt ) //
	st = 1100;      // �X�^�~�i�����l
	MaxSt = 1100;   // �X�^�~�i�ő�l
	MinSt = 700;    // �X�^�~�i�ŏ��l

	count = 30;      // ���̃V�[���ɍs���܂ł̃J�E���g

	countDown = 120; // �J�E���g�_�E��

	skillCount = 0;  // �X�L���̎g�p�J�E���g

}

// �f�X�g���N�^
Yamaoka_PlayerActor::~Yamaoka_PlayerActor()
{
	// ���f���̃A�����[�h
	MV1DeleteModel(modelHandle);
}

// �X�V����
void Yamaoka_PlayerActor::Update(float _deltaTime)
{
	UpdateActor(_deltaTime);
}

// �A�N�^�[�̍X�V����
void Yamaoka_PlayerActor::UpdateActor(float _deltaTime)
{
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	//// �X�^�[�g����
	////startFlag��true�̂Ƃ�
	//if (startFlag)
	//{
	//	PlayAnim(_deltaTime);								// �A�j���[�V���������擾
	//	MV1SetPosition(modelHandle, mPosition);				// �|�W�V�����X�V
	//	StartProcess(_deltaTime);
	//}

	// �J�E���g�_�E�����I��������J�n
	if (!startFlag && countDown <= 0)
	{
		// �J�n���̎��Ԃ��擾
		startTime = GetNowCount() / 1000;
		// �|�W�V�����X�V
		MV1SetPosition(modelHandle, mPosition);
		// �X�^�[�g����
		StartProcess(_deltaTime);
	}

	//	�X�e�[�g���j���̎�
	if (mNowPlayerState == STATE_SWIM)
	{
		//���ݎ������擾
		tmpTime = GetNowCount() / 1000;
		// �o�ߎ��Ԃ��v�Z 
		countUP = (tmpTime - startTime);

		//�j������
		if (Key & PAD_INPUT_RIGHT && mPosition.x <= 150 && mPosition.x >= -138)
		{
			mPrevKeyState = mNowKeyState;					//���̃L�[��Ԃ�O��̃L�[��Ԃ�
			mNowKeyState = STATE_KEY_RIGHT;					//���̃L�[��Ԃ�STATE_KEY_RIGHT��

			if (mNowKeyState != mPrevKeyState)				//���ƑO��̃L�[��Ԃ��Ⴄ�Ƃ�
			{
				mPosition.x -= mVelosity.x * _deltaTime;	//mPosition��mVelosity�����Z
				st--;										// �X�^�~�i�����炷
				dCount -= D_COUNT;							// �c�苗�������炷
			}
		}

		if (Key & PAD_INPUT_LEFT && mPosition.x <= 150 && mPosition.x >= -138)
		{
			mPrevKeyState = mNowKeyState;					//���̃L�[��Ԃ�O��̃L�[��Ԃ�
			mNowKeyState = STATE_KEY_LEFT;					//���̃L�[��Ԃ�STATE_KEY_LEFT��

			if (mNowKeyState != mPrevKeyState)				//���ƑO��̃L�[��Ԃ��Ⴄ�Ƃ�
			{
				mPosition.x -= mVelosity.x * _deltaTime;	//mPosition��mVelosity�����Z
				st--;										// �X�^�~�i�����炷
				dCount -= D_COUNT;							// �c�苗�������炷
			}

		}
		// �[�܂ōs������player���~�߂�
		if (mPosition.x <= -136)
		{
			mPosition.x = -134;
		}

		// �����߂�����
		if (mPosition.x >= 150)
		{
			mPosition.x = 150;
		}

		// �c�苗��
		if (dCount <= 0.0f)
		{
			// �[�܂ł����狗���J�E���g�� 0 �ɂ���
			dCount = 0;
			// ���̃V�[���܂ł̃J�E���g�����炷
			count--;
		}

		// �J�E���g�_�E���� 0 �ɂȂ��
		if (countDown <= 0)
		{
			// �l�� 0 ��
			countDown = 0;
		}

		// �^�[������
		/*if (mPosition.x <= -132 && mPosition.x >= -140 && Key & PAD_INPUT_M)					//�[�܂ōs����
		{
			mRotation = VGet(250.0f, 270.0f * DX_PI_F / 180.0f, 0.0f);							//�v���C���[�𔽓]
			mVelosity = VGet(-100, 0, 0);														//���x�𔽓]
		}*/

		PlayAnim(_deltaTime);						// �A�j���[�V���������擾
		MV1SetPosition(modelHandle, mPosition);		// �|�W�V�����X�V
	}

	// �����𒴂����� => �^�[����������
	if (dCount <= 25.0f)
	{
		// �^�[���t���O�� true ��
		turnFlag = true;
	}
}

// �X�^�[�g����
void Yamaoka_PlayerActor::StartProcess(float _deltaTime)
{
	// �L�[���͎擾
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	//if (Key & PAD_INPUT_M)				// �X�y�[�X�L�[
	//{
	//	mNowPlayerState = STATE_SWIM;	// �v���C���[���j����Ԃ�

	//	startFlag = false;
	//	attachAnim(0);					// �A�j���[�V����000���A�^�b�`
	//}

	mNowPlayerState = STATE_SWIM;	// �v���C���[���j����Ԃ�

	attachAnim(0);					// �A�j���[�V����000���A�^�b�`

	startFlag = true;               // �X�^�[�g�t���O�� true ��	
}

//�`��
void Yamaoka_PlayerActor::DrawActor()
{
	// 3D���f���̕`��
	MV1DrawModel(modelHandle);

	// 3D���f���̉�]�p�x (�ǂ̕����������Ă��邩)
	MV1SetRotationXYZ(modelHandle, mRotation);

	//DrawFormatString(800, 400, GetColor(255, 0, 0), "%d", mPosition.x);
}

// �A�j���[�V�����̍Đ����Ԃ��擾
void Yamaoka_PlayerActor::PlayAnim(float _deltaTime)
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

// �A�j���[�V���������擾
void Yamaoka_PlayerActor::attachAnim(int _animPlay)
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

// �X�^�~�i�Q�[�W�̕`��
void Yamaoka_PlayerActor::DrawSt(int _st, int _MaxSt, int _MinSt)
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
	DrawFormatString(_MinSt, 1000, GetColor(0, 0, 0), "%d / 400", _MaxSt * _st / _MaxSt - _MinSt);
}

// �S�[���܂ł̋���
void Yamaoka_PlayerActor::DrawToGoal(float _playerPos, float _goalPos)
{
	// �f�o�b�O�p
	//DrawFormatString(1300, 500, GetColor(0, 0, 0), "NowPos  %d", count);
	//DrawFormatString(1300, 550, GetColor(0, 0, 0), "  %d", 600 * _playerPos / _goalPos);

	// �c��̋����̕\��
	DrawBox(1590, 895, 1850, 945, GetColor(0, 255, 255), TRUE);
	DrawFormatString(1600, 900, GetColor(0, 0, 0), "�c��  %d m", (int)(_goalPos * _playerPos / _goalPos));

	// �ꉝ��������
	if (_playerPos <= 0)
	{
		_playerPos = 0;    // �l���Œ�
		SetFontSize(100);
		DrawFormatString(900, 450, GetColor(255, 0, 0), "GOAL");
	}

}

// �K�E�Z
void Yamaoka_PlayerActor::Skill(float _playerPos, float _goalPos)
{
	// ���l�\��
	//DrawFormatString(850, 800, GetColor(255, 0, 0), "skillcount   %d", skillCount);

	SetFontSize(40);

	// �K�E�Z�̃A�C�R���i�g�j��\��
	DrawBox(850, 100, 950, 200, GetColor(0, 0, 0), FALSE);

	// turnFlag = true �ŃX�L���J�E���g�� 0 �̂Ƃ�
	if (turnFlag && skillCount == 0)
	{
		DrawFormatString(680, 60, GetColor(255, 0, 0), "�Ђ����킴 ������");
		// �K�E�Z�̃A�C�R���i�g�j��h��Ԃ�
		DrawBox(850, 100, 950, 200, GetColor(255, 0, 0), TRUE);
		DrawFormatString(800, 200, GetColor(0, 255, 255), "PUSH SPACE");
	}
	else
	{
		DrawFormatString(680, 60, GetColor(255, 0, 0), "�Ђ����킴 �����Ȃ�");
	}

	// �v���C���[���^�[��������
	if (turnFlag && (int)(_goalPos * _playerPos / _goalPos) <= 25)
	{
		// Space �������ƕK�E�Z���g��(�X�^�~�i����)
		if (CheckHitKey(KEY_INPUT_SPACE))
		{
			mPrevKeyState = mNowKeyState;		// ���̃L�[��Ԃ�O��̃L�[��Ԃ�
			mNowKeyState = STATE_KEY_SPACE;		// ���̃L�[��Ԃ�STATE_KEY_SPACE��

			// �K�E�Z�̏���
			// ���ƑO��̃L�[��Ԃ��Ⴄ�Ƃ�
			if (mNowKeyState != mPrevKeyState && skillCount <= 0)
			{
				// �X�^�~�i���񕜂���
				st = st + 30;
				// �X�L���J�E���g�� 1 ��
				skillCount = 1;
				// turnFlag �� false ��
				turnFlag = false;
			}
		}
	}

	// A �������ƕK�E�Z���g��(�X�s�[�h�A�b�v �P�`�R�b���炢�̗\��)
	if (CheckHitKey(KEY_INPUT_A))
	{
		mPrevKeyState = mNowKeyState;		// ���̃L�[��Ԃ�O��̃L�[��Ԃ�
		mNowKeyState = STATE_KEY_SPACE;		// ���̃L�[��Ԃ�STATE_KEY_SPACE��

		// �K�E�Z�̏���
		// ���ƑO��̃L�[��Ԃ��Ⴄ�Ƃ�
		if (mNowKeyState != mPrevKeyState)
		{
			// �X�s�[�h�A�b�v
			mVelosity.x = mVelosity.x * 1.005;
			// �X�^�~�i�̏���ʂ��Q�{��
			st -= 2;
		}
	}
	// ���쒆
}
