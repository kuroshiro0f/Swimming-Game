#include "Ueyama_PlayerActor.h"

const float D_COUNT = 0.355f;

//	�X�^�~�i�Q�[�W�̐F���ς��c��Q�[�W��
const int GREEN = 950;
const int ORANGE = 800;
//const int RED = 3;

//�R���X�g���N�^
Ueyama_PlayerActor::Ueyama_PlayerActor()
	:mNowPlayerState(STATE_IDLE)
	, mNowKeyState(STATE_KEY_IDLE)
	, mPrevKeyState(STATE_KEY_IDLE)
	, startTime(0)
	, tmpTime(0)
	, countUP(0)
{
	startFlag = false;

	mPosition = VGet(150, 18, 0);								// �����ʒu�ݒ�
	mRotation = VGet(250.0f, 90.0f * DX_PI_F / 180.0f, 0.0f);	// ��]�p�x
	mDirection = VGet(0, 0, 1);
	mVelosity = VGet(250, 0, 0);

	//���f���̃��[�h
	modelHandle = MV1LoadModel("data/player/player.pmx");

	animTotal = 0.0f;
	animNowTime = 0.0f;
	animIndex = 0;

	NowPos = 0;            // ���݂̍��W
	// ������          //

	// �S�[���܂ł̋����@( 25m ) 
	dCount = 25.0f;         // �i�񂾋���
	maxdCount = 25.0f;      // �S�[��  

	// �X�^�~�i�Q�[�W //
	st = 1250;      // �X�^�~�i�����l
	MaxSt = 1250;   // �X�^�~�i�ő�l
	MinSt = 650;    // �X�^�~�i�ŏ��l

	count = 30;      // ���̃V�[���ɍs���܂ł̃J�E���g

	countDown = 120; // �J�E���g�_�E���i 3�b �j
}

//�f�X�g���N�^
Ueyama_PlayerActor::~Ueyama_PlayerActor()
{
	// ���f���̃A�����[�h
	MV1DeleteModel(modelHandle);
}

//�X�V����
void Ueyama_PlayerActor::Update(float _deltaTime)
{
	UpdateActor(_deltaTime);
}

//�A�N�^�[�̍X�V����
void Ueyama_PlayerActor::UpdateActor(float _deltaTime)
{
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	////�X�^�[�g����
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
		MV1SetPosition(modelHandle, mPosition);				// �|�W�V�����X�V
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
				st -= 15;										// �X�^�~�i�����炷
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
		//�����߂�����
		if (mPosition.x >= 150)
		{
			mPosition.x = 145;
		}

		// �c�苗��
		if (dCount <= 0.9)
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

		//�^�[������
		/*if (mPosition.x <= -132 && mPosition.x >= -140 && Key & PAD_INPUT_M)					//�[�܂ōs����
		{
			mRotation = VGet(250.0f, 270.0f * DX_PI_F / 180.0f, 0.0f);							//�v���C���[�𔽓]
			mVelosity = VGet(-100, 0, 0);														//���x�𔽓]
		}*/

		PlayAnim(_deltaTime);						// �A�j���[�V���������擾
		MV1SetPosition(modelHandle, mPosition);		// �|�W�V�����X�V
	}
}

//�X�^�[�g����
void Ueyama_PlayerActor::StartProcess(float _deltaTime)
{
	//�L�[���͎擾
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	//if (Key & PAD_INPUT_M)				//�X�y�[�X�L�[
	//{
	//	mNowPlayerState = STATE_SWIM;	//�v���C���[���j����Ԃ�

	//	startFlag = false;
	//	attachAnim(0);					//�A�j���[�V����000���A�^�b�`
	//}

	mNowPlayerState = STATE_SWIM;	// �v���C���[���j����Ԃ�

	attachAnim(0);					// �A�j���[�V����000���A�^�b�`

	startFlag = true;               // �X�^�[�g�t���O�� true ��	
}

//�`��
void Ueyama_PlayerActor::DrawActor()
{
	// 3D���f���̕`��
	MV1DrawModel(modelHandle);

	// 3D���f���̉�]�p�x (�ǂ̕����������Ă��邩)
	MV1SetRotationXYZ(modelHandle, mRotation);
}

//�A�j���[�V����
void Ueyama_PlayerActor::PlayAnim(float _deltaTime)
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

void Ueyama_PlayerActor::attachAnim(int _animPlay)
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
void Ueyama_PlayerActor::DrawSt(int _st, int _MaxSt, int _MinSt)
{
	int color;							//	�X�^�~�i�Q�[�W�̒��g�̐F
	// �F�ݒ�
	if (_st >= GREEN)
	{
		color = GetColor(0, 255, 0);		//	���g(��)
	}
	if (_st < GREEN && _st >= ORANGE)
	{
		color = GetColor(255, 177, 27);	//	���g�i�I�����W�j
	}
	if (_st < ORANGE)
	{
		color = GetColor(255, 0, 0);		//	���g�i�ԁj
	}
	int frame_color = GetColor(0, 0, 0);	//	�g (��)

	// �X�^�~�i�Q�[�W�� 0 �ɂȂ�����
	if (_st <= _MinSt)
	{
		// �X�^�~�i���ŏ��l��
		_st = _MinSt;
	}

	// �Q�[�W�̘g�\��
	DrawBox(_MinSt, 1000, _MaxSt, 1035, frame_color, FALSE);
	// �Q�[�W�̒��g�\��
	DrawBox(_MinSt, 1000,  _st , 1035, color, TRUE);
	// ���l�\�� 
	DrawFormatString(_MinSt, 1000, GetColor(0, 0, 0), "%d / 600",_st - _MinSt);
}

// �S�[���܂ł̋���
void Ueyama_PlayerActor::DrawToGoal(float _playerPos, float _goalPos)
{
	// �f�o�b�O�p
	//DrawFormatString(1300, 500, GetColor(0, 0, 0), "NowPos  %d", count);
	//DrawFormatString(1300, 550, GetColor(0, 0, 0), "  %d", 600 * _playerPos / _goalPos);

	// �c��̋����̕\��
	DrawBox(1590, 895, 1850, 945, GetColor(255, 255, 0), TRUE);
	DrawFormatString(1600, 900, GetColor(0, 0, 0), "�c��  %d m", (int)(_goalPos * _playerPos / _goalPos));

	// �ꉝ��������
	if (_playerPos <= 0)
	{
		_playerPos = 0;    // �l���Œ�
		SetFontSize(100);
		DrawFormatString(900, 450, GetColor(255, 0, 0), "GOAL");
	}
}