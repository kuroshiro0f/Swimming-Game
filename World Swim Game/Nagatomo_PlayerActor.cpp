#include "Nagatomo_PlayerActor.h"

#include <random>

//�R���X�g���N�^
Nagatomo_PlayerActor::Nagatomo_PlayerActor()
	:mNowPlayerState(STATE_IDLE)
	, mNowKeyState(STATE_KEY_IDLE)
	, mPrevKeyState(STATE_KEY_IDLE)
	, mEvlt(NONE)
	, startTime(0)
	, tmpTime(0)
	, countUP(0)
	, countDownFinishFlag(false)
	, skillFlag(false)
	, turnGraphFlag(false)
	, finishFlag(false)
	, m_ultCheckFlag(false)
	, m_ultFinishFlag(0)
{
	startFlag = false;
	turnFlag = false;
	randomFlag = false;
	inputSpaceFlag = false;
	inputArrowFlag = false;
	mCheckKeyFlag = false;
	ultLimitFlag = false;
	ultFlag = false;

	mPosition = VGet(150, 18, 0);								// �����ʒu�ݒ�
	mRotation = VGet(250.0f, 90.0f * DX_PI_F / 180.0f, 0.0f);	// ��]�p�x
	mDirection = VGet(0, 0, 1);									//�v���C���[�̕���
	mVelosity = VGet(8, 0, 0);									//���x

	// ���f���̃��[�h
	modelHandle = MV1LoadModel("data/swimmer/player.pmx");

	animTotal = 0.0f;
	animNowTime = 0.0f;
	animIndex = 0;

	NowPos = 0;            // ���݂̍��W

	// �S�[���܂ł̋����@( 50m ) 
	dCount = 50.0f;         // �i�񂾋���
	maxdCount = 50.0f;      // �S�[��  


	//�X�^�~�i�֘A
	st = 600;			// �X�^�~�i�����l
	MaxSt = 600;			// �X�^�~�i�ő�l
	MinSt = 0;				// �X�^�~�i�ŏ��l
	halfSt = 600 / 2;		//�X�^�~�i�i50%�j
	quarterSt = 600 / 4;		//�X�^�~�i�i25%�j

	count = 30;      // ���̃V�[���ɍs���܂ł̃J�E���g

	countDown = 170; // �J�E���g�_�E���i 3�b �j
	inputTime = 0;

	skillCount = 0;  // �X�L���̎g�p�J�E���g
	skillTime = 180;   // �X�L���̌��ʎ���

	//���͊֘A
	inputTime = 0.0f;
	inputLimitTime = limitTime;

	//�X�s�[�h�֘A
	addStaminaSpeed = 0.0f;

	// ��~���ԏ�����
	inputCount = 0;
}

//�f�X�g���N�^
Nagatomo_PlayerActor::~Nagatomo_PlayerActor()
{
	// ���f���̃A�����[�h
	MV1DeleteModel(modelHandle);
	DeleteGraph(turnGraphHandle);
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

	// �J�E���g�_�E�����I��������J�n
	if (!startFlag && countDown <= 0)
	{
		// �J�n���̎��Ԃ��擾
		startTime = GetNowCount() / 1000;
		MV1SetPosition(modelHandle, mPosition);				// �|�W�V�����X�V
		StartProcess(_deltaTime);
		countDownFinishFlag = true;
	}

	//	�X�e�[�g���j���̎�
	if (mNowPlayerState == STATE_SWIM)
	{
		//���ݎ������擾
		tmpTime = GetNowCount() / 1000;
		// �o�ߎ��Ԃ��v�Z 
		countUP = (tmpTime - startTime);

		mPrevPosition = mPosition;							//�v���C���[�̃|�W�V������⊮

		AutoMove(turnFlag, _deltaTime);

		//	�Z���I���܂ňʒu���Œ�
		if (skillFlag)
		{
			mPosition.x = mPosition.x;
		}

		dCount -= std::sqrt((mPosition.x - mPrevPosition.x) * (mPosition.x - mPrevPosition.x)) * 0.088;		//�c��̋���

		GenRandomKey(randomFlag, ultFlag);

		// ���p���L�[����������
		if (CheckHitKey(KEY_INPUT_C))
		{
			randomKeyNumber = 5;          // randamKeyNumber�� 5 (���p���L�[�̔ԍ�)������
			inputStartTime = GetNowCount() / 1000;			//�����_���ɖ��𐶐��������Ԃ��擾
			randomFlag = true;
		}

		if (dCount <= dCountUlt)
		{
			ultLimitFlag = true;
		}

		ProcessInput(ultFlag, skillFlag);

		// ���p������
		if (randomKeyNumber == STATE_KEY_C)
		{
			inputEndTime = GetNowCount() / 1000;			//���݂̎��Ԃ��擾
			inputTime = (inputEndTime - inputStartTime);	//���ݎ��Ԃƃ����_���ɖ��𐶐��������Ԃ̍�

			// C�L�[�������ꂽ�Ƃ�
			if (Key & PAD_INPUT_C)
			{
				st += 5;			// �X�^�~�i�����炷
				inputTime = 0;		// ���͉\���Ԃ�������
				// �X�^�~�i���ő�l�𒴂�����
				if (st >= MaxSt)
				{
					st = MaxSt;
				}

				mVelosity = VGet(2.0, 0, 0);
			}
			// ���ݎ��Ԃƃ����_���ɖ��𐶐��������Ԃ̍���1�b��������
			else if (inputTime > 1)
			{
				randomFlag = false;
				inputTime = 0;			// ���͉\���Ԃ�������
				inputLimitTime = limitTime;							//���͐������Ԃ����Z�b�g
				inputArrowFlag = false;
			}
		}

		StaminaCoefficient(st, halfSt, quarterSt);

		// �X�^�~�i���؂ꂽ��
		if (!skillFlag)
		{
			if (st <= MinSt)
			{
				// �X�^�~�i���ŏ��l�ŌŒ�
				st = MinSt;
			}
			// �^�[������O
			if (st <= MinSt)
			{
				// ���x��ʏ�̔������x��
				mVelosity = VGet(2.0, 0, 0);
			}
		}

		//�����߂�����
		if (mPosition.x >= 150)
		{
			mPosition.x = 145;
		}
		if (mPosition.x <= -138)
		{
			mPosition.x = 138;
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

		TurnProcess(turnFlag, mPosition);

		LastSpurt();	// ���X�g�X�p�[�g

		PlayAnim(_deltaTime);						// �A�j���[�V���������擾
		MV1SetPosition(modelHandle, mPosition);		// �|�W�V�����X�V
	}
}

//�X�^�[�g����
void Nagatomo_PlayerActor::StartProcess(float _deltaTime)
{
	//�L�[���͎擾
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	mNowPlayerState = STATE_SWIM;	// �v���C���[���j����Ԃ�

	attachAnim(0);					// �A�j���[�V����000���A�^�b�`

	startFlag = true;               // �X�^�[�g�t���O�� true ��	
}

//�`��
void Nagatomo_PlayerActor::DrawActor()
{
	// 1.3f �` 1.5f
	SetDraw3DScale(1.4f);
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

void Nagatomo_PlayerActor::attachAnim(int _animPlay)
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
void Nagatomo_PlayerActor::DrawSt(int _st, int _MaxSt, int _MinSt)
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
	DrawBox(ST_FIRST_X, ST_FIRST_Y, ST_END_X, ST_END_Y, frame_color, FALSE);
	// �Q�[�W�̒��g�\��
	DrawBox(ST_FIRST_X, ST_FIRST_Y, _st + ST_FIRST_X, ST_END_Y, color, TRUE);
	// ���l�\�� 
	DrawFormatString(ST_FIRST_X, ST_FIRST_Y, GetColor(0, 0, 0), "%d / 600", _st);
}

// �S�[���܂ł̋���
void Nagatomo_PlayerActor::DrawToGoal(float _playerPos, float _goalPos)
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

void Nagatomo_PlayerActor::AutoMove(bool _turnFlag,float _deltaTime)
{
	//�^�[���O
	if (!_turnFlag)
	{
		mPosition.x -= mVelosity.x * _deltaTime;			//�v���C���[�̎����ړ�
	}
	//�^�[����
	else
	{
		mPosition.x += mVelosity.x * _deltaTime;			//�v���C���[�̎����ړ�
	}
}

void Nagatomo_PlayerActor::GenRandomKey(bool _randomFlag, bool _ultFlag)
{
	if (_randomFlag == false && !_ultFlag)
	{
		randomKeyNumber = rand() % 3 + 1;				//1�`4�܂ł̐����������_���ɐ���
		inputStartTime = GetNowCount() / 1000;			//�����_���ɖ��𐶐��������Ԃ��擾
		randomFlag = true;
		mCheckKeyFlag = false;
	}
}

void Nagatomo_PlayerActor::ProcessInput(bool _ultFlag, bool _skillFlag)
{
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	if (!_ultFlag && !_skillFlag)
	{
		switch (randomKeyNumber)
		{
		case 1:	//���L�[�̂Ƃ�
			inputEndTime = GetNowCount() / 1000;			//���݂̎��Ԃ��擾
			inputTime = (inputEndTime - inputStartTime);	//���͎��Ԃ��J�E���g

			//���L�[�������ꂽ�Ƃ�
			if (Key & PAD_INPUT_UP && !mCheckKeyFlag)
			{
				SuccessInput(st, MinSt);
			}
			//�� �� �� �L�[�������ꂽ�Ƃ�
			else if (!mCheckKeyFlag && Key & PAD_INPUT_DOWN || !mCheckKeyFlag && Key & PAD_INPUT_RIGHT || !mCheckKeyFlag && Key & PAD_INPUT_LEFT)
			{
				FailureInput(st, MinSt);
			}
			InputOverTime(inputArrowFlag, inputTime, inputLimitTime);
			break;

		case 2:		//���L�[�̂Ƃ�
			inputEndTime = GetNowCount() / 1000;					 //���݂̎��Ԃ��擾
			inputTime = (inputEndTime - inputStartTime);			 //���͎��Ԃ��J�E���g

			if (Key & PAD_INPUT_DOWN && !mCheckKeyFlag)
			{
				SuccessInput(st, MinSt);
			}
			else if (!mCheckKeyFlag && Key & PAD_INPUT_UP || !mCheckKeyFlag && Key & PAD_INPUT_RIGHT || !mCheckKeyFlag && Key & PAD_INPUT_LEFT)
			{
				FailureInput(st, MinSt);
			}
			InputOverTime(inputArrowFlag, inputTime, inputLimitTime);
			break;

		case 3:		//���L�[�̂Ƃ�
			inputEndTime = GetNowCount() / 1000;					//���݂̎��Ԃ��擾
			inputTime = (inputEndTime - inputStartTime);			//���͎��Ԃ��J�E���g

			if (Key & PAD_INPUT_RIGHT && !mCheckKeyFlag)
			{
				SuccessInput(st, MinSt);
			}
			else if (!mCheckKeyFlag && Key & PAD_INPUT_UP || !mCheckKeyFlag && Key & PAD_INPUT_DOWN || !mCheckKeyFlag && Key & PAD_INPUT_LEFT)
			{
				FailureInput(st, MinSt);
			}
			InputOverTime(inputArrowFlag, inputTime, inputLimitTime);
			break;

		case 4:		//���L�[�̂Ƃ�
			inputEndTime = GetNowCount() / 1000;					//���݂̎��Ԃ��擾
			inputTime = (inputEndTime - inputStartTime);			//���͎��Ԃ��J�E���g

			if (Key & PAD_INPUT_LEFT && !mCheckKeyFlag)
			{
				SuccessInput(st, MinSt);
			}
			else if (!mCheckKeyFlag && Key & PAD_INPUT_UP || !mCheckKeyFlag && Key & PAD_INPUT_DOWN || !mCheckKeyFlag && Key & PAD_INPUT_RIGHT)
			{
				FailureInput(st, MinSt);
			}
			InputOverTime(inputArrowFlag,inputTime,inputLimitTime);

			break;
		default:
			break;
		}
	}
}

void Nagatomo_PlayerActor::SuccessInput(int _st, int _MinSt)
{
	if (_st > _MinSt)
	{
		if (mVelosity.x > maxSpeed)
		{
			mVelosity.x = maxSpeed;
		}
		else
		{
			mVelosity.x += addSpeed * addStaminaSpeed;
		}
		st -= ST_SUC_DEC;
		inputTime = 0;
		inputArrowFlag = true;
		mCheckKeyFlag = true;
	}
}

void Nagatomo_PlayerActor::FailureInput(int _st, int _MinSt)
{
	if (st > MinSt)
	{
		mVelosity.x = missSpeed.x * addStaminaSpeed;
		st -= ST_FAI_DEC;
		inputLimitTime = limitTime;
		inputArrowFlag = false;
		mCheckKeyFlag = true;
	}
}

void Nagatomo_PlayerActor::InputOverTime(bool _inputArrowFlag, float _inputTime, float _inputLimitTime)
{
	//�L�[���͂�����ē��͐������Ԃ��߂�����
	if (_inputArrowFlag && _inputTime > _inputLimitTime)
	{
		_inputTime = 0;
		_inputArrowFlag = false;
		randomFlag = false;
		if (_inputLimitTime < maxTime)
		{
			_inputLimitTime = maxTime;
		}
		else
		{
			_inputLimitTime -= 0.2f;
		}
	}
	//�L�[���͂����ꂸ�ɓ��͐������Ԃ��߂�����
	if (!_inputArrowFlag && _inputTime > _inputLimitTime)
	{

		mVelosity.x = missSpeed.x * addStaminaSpeed;
		_inputTime = 0;
		_inputLimitTime = limitTime;
		randomFlag = false;
	}
}

void Nagatomo_PlayerActor::TurnProcess(bool _turnFlag, VECTOR _mPosition)
{
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	//�X�y�[�X�������ꂽ�Ƃ�
	if (Key & PAD_INPUT_M && _turnFlag == false)
	{
		inputSpaceFlag = true;
		inputLimitTime = limitTime;			//���͐������Ԃ����Z�b�g
		mPosX = _mPosition.x;				//�����ꂽ���̃v���C���[�̍��W��⊮

		if (-90 >= mPosX && mPosX > -120)
		{
			mEvlt = BAD;					//���͂�����������BAD�]��
			turnGraphHandle = LoadGraph("data/img/Game/Turn1.png");

		}
		if (-120 >= mPosX && mPosX > -130)
		{
			mEvlt = NORMAL;					//���͂���������������NORMALE�]��
			turnGraphHandle = LoadGraph("data/img/Game/Turn2.png");

		}
		if (-130 >= mPosX && mPosX > -140)
		{
			mEvlt = GOOD;					//���͂����傤�ǂ�������GOOD�]��
			turnGraphHandle = LoadGraph("data/img/Game/Turn3.png");

		}
	}
	//������Ȃ��܂ܒ[�܂ŗ����Ƃ�
	else if (_turnFlag == false && _mPosition.x <= -140)
	{
		mEvlt = BAD;						//BAD�]����
		turnGraphHandle = LoadGraph("data/img/Game/Turn1.png");
		turnFlag = true;
		inputLimitTime = limitTime;			//���͐������Ԃ����Z�b�g
	}


	//�v�[���̒[�܂ŗ�����
	if (_mPosition.x <= -136)
	{
		turnFlag = true;
		mRotation = VGet(250.0f, 270.0f * DX_PI_F / 180.0f, 0.0f);							//�v���C���[�̌����𔽓]
		turnGraphFlag = true;
		switch (mEvlt)
		{
		case BAD:
			mVelosity = VGet(10, 0, 0);
			st -= 15;

		case NORMAL:
			mVelosity = VGet(12, 0, 0);
			st -= 10;

		case GOOD:
			mVelosity = VGet(15, 0, 0);
			st -= 5;

		default:
			break;
		}
	}
}

// �K�E�Z
void Nagatomo_PlayerActor::LastSpurt()
{
	// �c��15m�ȉ��ɂȂ�����
	if (dCount <= dCountUlt)
	{
		DrawFormatString(850, 700, GetColor(255, 0, 0), "Last Spurt");

		if (!m_ultCheckFlag)
		{
			UltNumber(randomFlag);
		}
		UltProcessInput(arrow, 4);
		m_ultCheckFlag = true;
		ultFlag = true;
		randomFlag = false;
		skillFlag = true;
		//mVelosity.x = 60.0f;

		stopTime++;

		// ���̓��͏I���܂Œ�~
		if (m_ultFinishFlag == 4)
		{
			skillCount = 1;
		}
		// ��~���Ԃ��I�������
		if (skillCount == 1 && !finishFlag)
		{
			skillFlag = false;
			stopTime = 0;

			// �ĊJ���̃Y�������������i�����j
			countUP -= 3;
		}
		// �S�[��������
		if (GetPosX() >= 130 && GetTurnFlag() == true)
		{
			finishFlag = true;
		}

	}

	// ���l�\��
	//DrawFormatString(850, 700, GetColor(255, 0, 0), "skillTime   %d", skillTime);
	//DrawFormatString(0, 30, GetColor(255, 0, 255), "mVelosity.x        %f", mVelosity.x);
	//DrawFormatString(0, 100, GetColor(255, 0, 255), "stopTime    %d", stopTime);
}

void Nagatomo_PlayerActor::UltNumber(bool _randomFlag)
{
	if (_randomFlag)
	{
		for (int i = 0; i < 4; i++)
		{
			randomKeyNumber = rand() % 3 + 1;				//1�`4�܂ł̐����������_���ɐ���
			arrow[i] = randomKeyNumber;
		}
	}
}

void Nagatomo_PlayerActor::UltProcessInput(int _arrow[], int _size)
{
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	count = 0;

	for (int i = 0; i < _size; i++)
	{
		switch (_arrow[i])
		{
		case STATE_KEY_UP:
			if (CheckHitKey(KEY_INPUT_UP) && mCheckKeyFlag)
			{
				inputCount += 1;
				mCheckKeyFlag = false;
			}
			else
			{
				break;
			}
			break;
		case STATE_KEY_DOWN:
			if (CheckHitKey(KEY_INPUT_DOWN) && mCheckKeyFlag)
			{
				inputCount += 1;
				mCheckKeyFlag = false;
			}
			else
			{
				break;
			}
			break;
		case STATE_KEY_RIGHT:
			if (CheckHitKey(KEY_INPUT_RIGHT) && mCheckKeyFlag)
			{
				inputCount += 1;
				mCheckKeyFlag = false;
			}
			else
			{
				break;
			}
			break;
		case STATE_KEY_LEFT:
			if (CheckHitKey(KEY_INPUT_LEFT) && mCheckKeyFlag)
			{
				inputCount += 1;
				mCheckKeyFlag = false;
			}
			else
			{
				break;
			}
			break;
		}
		mVelosity = VGet(inputCount * 2, 0, 0);
		mCheckKeyFlag = true;
	}
}

void Nagatomo_PlayerActor::StaminaCoefficient(int _st, int _halfSt, int _quarterSt)
{
	//�c��X�^�~�i��50%�ȏ�
	if (_st >= _halfSt)
	{
		addStaminaSpeed = 1.0f;
	}
	//�c��X�^�~�i��50%�ȉ�
	if (_st <= _halfSt && st >= _quarterSt)
	{
		addStaminaSpeed = 0.8f;
	}
	//�c��X�^�~�i��25%�ȉ�
	if (_st <= _quarterSt)
	{
		addStaminaSpeed = 0.6f;
	}
}
