#include "Ueyama_PlayerActor.h"

#include <random>

//-----------------------------------------------------------------------------
// �@�ύX�Ȃ�
//-----------------------------------------------------------------------------
const float D_COUNT = 0.355f;

//	�X�^�~�i�Q�[�W�̐F���ς��c��Q�[�W��
const int GREEN = 300;
const int ORANGE = 150;
//const int RED = 3;

//	�X�^�~�i�Q�[�W�̕\���ʒu
const int ST_FIRST_X = 665;
const int ST_FIRST_Y = 85;
const int ST_END_X = 1250;
const int ST_END_Y = 1035;
const int ST_HEIGHT = 62;

//	�X�^�~�i�̌�����
const int ST_SUC_DEC = 60;
const int ST_FAI_DEC = 100;
//const int ST_SUC_DEC = 4;
//const int ST_FAI_DEC = 6;

//�X�s�[�h�֘A
const VECTOR missSpeed = VGet(5, 0, 0);		//���̓~�X�����Ƃ��̃X�s�[�h
const float addSpeed = 3.0f;				//���Z�����X�s�[�h
const float maxSpeed = 15.0f;				//�ő�X�s�[�h

//	�����̕\���ꏊ
const int DISTANCE_X = 1700;
const int DISTANCE_Y = 857;

//���͊֘A
const float limitTime = 1.5f;				//���͐�������
const float maxTime = 0.5f;					//�ő厞��

const int dCountUlt = 15;					//�E���g���g����悤�ɂȂ�c�苗��

//	���X�g�X�p�[�g�ł̖��̈ʒu����
const int FIRST_ARROW_X = -210;
const int SECOND_ARROW_X = -70;
const int THIRD_ARROW_X = 70;
const int FORTH_ARROW_X = 210;

//�R���X�g���N�^
Ueyama_PlayerActor::Ueyama_PlayerActor()
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
	, arrowFailFlag(false)
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

	for (int i = 0; i < 4; i++)
	{
		ultSucFlag[i] = 0;
		arrowAngle[i] = 0;
	}

	mPosition = VGet(150, 18, 0);								// �����ʒu�ݒ�
	mRotation = VGet(250.0f, 90.0f * DX_PI_F / 180.0f, 0.0f);	// ��]�p�x
	mDirection = VGet(0, 0, 1);									//�v���C���[�̕���
	mVelosity = VGet(8, 0, 0);									//���x

	// ���f���̃��[�h
	modelHandle = MV1LoadModel("data/swimmer/player.pmx");

	//	�摜�̃��[�h
	//m_stGraphHandle = LoadGraph("data/Game/Game_st.png");
	m_failGraphHandle = LoadGraph("data/img/Game/Game_fail.png");				//	���s

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

	inputCount = 0;

	// ��~���ԏ�����
	stopTime = 0;

	countSpeed = 0;

	m_font = new Font();
}

//�f�X�g���N�^
Ueyama_PlayerActor::~Ueyama_PlayerActor()
{
	// ���f���̃A�����[�h
	MV1DeleteModel(modelHandle);
	DeleteGraph(turnGraphHandle);
	DeleteGraph(m_failGraphHandle);

	delete m_font;
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
		if (!skillFlag && !finishFlag)
		{
			//���ݎ������擾
			tmpTime = GetNowCount() / 1000;
			// �o�ߎ��Ԃ��v�Z 
			countUP = (tmpTime - startTime);
		}

		// 2�b�����ɃX�^�~�i��
		/*if (countUP % 2 == 0 && !skillFlag)
		{
			st += 1;
		}
		if (st >= MaxSt)
		{
			st = MaxSt;
		}*/

		mPrevPosition = mPosition;							//�v���C���[�̃|�W�V������⊮

		//�^�[���O
		if (!turnFlag)
		{
			mPosition.x -= mVelosity.x * _deltaTime;			//�v���C���[�̎����ړ�
		}
		//�^�[����
		else if (!skillFlag)
		{
			mPosition.x += mVelosity.x * _deltaTime;			//�v���C���[�̎����ړ�
		}

		//	�Z���I���܂ňʒu���Œ�
		/*if (skillFlag)
		{
			mPosition.x = mPosition.x;
		}*/

		dCount -= std::sqrt((mPosition.x - mPrevPosition.x) * (mPosition.x - mPrevPosition.x)) * 0.088;

		if (randomFlag == false && skillFlag == false)
		{
			randomKeyNumber = rand() % 3 + 1;				//1�`4�܂ł̐����������_���ɐ���
			inputStartTime = GetNowCount() / 1000;			//�����_���ɖ��𐶐��������Ԃ��擾
			randomFlag = true;
			mCheckKeyFlag = false;
		}

		// ���p���L�[����������
		if (CheckHitKey(KEY_INPUT_C) && skillFlag == false)
		{
			randomKeyNumber = 5;          // randamKeyNumber�� 5 (���p���L�[�̔ԍ�)������
			inputStartTime = GetNowCount() / 1000;			//�����_���ɖ��𐶐��������Ԃ��擾
			randomFlag = true;
		}

		if (dCount <= dCountUlt)
		{
			ultLimitFlag = true;
		}
		//�K�E�Z
		if (/*CheckHitKey(KEY_INPUT_A) &&*/ ultLimitFlag)
		{
			ultFlag = true;
			randomFlag = false;
		}

		/*if (ultFlag)
		{
			if (!m_ultCheckFlag)
			{
				UltNumber(randomFlag);
			}
			UltProcessInput(arrow, 4);
			m_ultCheckFlag = true;
			randomFlag = true;
		}*/

		if (!ultFlag && !skillFlag)
		{
			//�v���C���[�̏���//
			switch (randomKeyNumber)
			{
			case 1:	//���L�[�̂Ƃ�
				inputEndTime = GetNowCount() / 1000;			//���݂̎��Ԃ��擾
				inputTime = (inputEndTime - inputStartTime);

				//���L�[�������ꂽ�Ƃ�
				if (Key & PAD_INPUT_UP && !mCheckKeyFlag)
				{
					if (st > MinSt)
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
				//�� �� �� �L�[�������ꂽ�Ƃ�
				else if (!mCheckKeyFlag && Key & PAD_INPUT_DOWN || !mCheckKeyFlag && Key & PAD_INPUT_RIGHT || !mCheckKeyFlag && Key & PAD_INPUT_LEFT)
				{
					if (st > MinSt)
					{
						mVelosity.x = missSpeed.x * addStaminaSpeed;
						st -= ST_FAI_DEC;
						inputLimitTime = limitTime;							//���͐������Ԃ����Z�b�g
						inputArrowFlag = false;
						mCheckKeyFlag = true;
					}

				}
				//�L�[���͂�����ē��͐������Ԃ��߂�����
				else if (inputArrowFlag && inputTime > inputLimitTime)
				{
					inputTime = 0;
					inputArrowFlag = false;
					randomFlag = false;
					arrowFailFlag = false;
					if (inputLimitTime < maxTime)
					{
						inputLimitTime = maxTime;
					}
					else
					{
						inputLimitTime -= 0.2f;
					}
				}
				//�L�[���͂����ꂸ�ɓ��͐������Ԃ��߂�����
				else if (!inputArrowFlag && inputTime > inputLimitTime)
				{

					mVelosity.x = missSpeed.x * addStaminaSpeed;
					inputTime = 0;
					arrowFailFlag = false;
					inputLimitTime = limitTime;							//���͐������Ԃ����Z�b�g
					randomFlag = false;
				}
				break;
			case 2:		//���L�[�̂Ƃ�
				inputEndTime = GetNowCount() / 1000;
				inputTime = (inputEndTime - inputStartTime);			 //���͎��Ԃ��J�E���g

				if (Key & PAD_INPUT_DOWN && !mCheckKeyFlag)
				{
					if (st > MinSt)
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
				else if (!mCheckKeyFlag && Key & PAD_INPUT_UP || !mCheckKeyFlag && Key & PAD_INPUT_RIGHT || !mCheckKeyFlag && Key & PAD_INPUT_LEFT)
				{
					if (st > MinSt)
					{
						mVelosity.x = missSpeed.x * addStaminaSpeed;
						st -= ST_FAI_DEC;
						inputLimitTime = limitTime;							//���͐������Ԃ����Z�b�g
						inputArrowFlag = false;
						mCheckKeyFlag = true;
					}
				}
				//�L�[���͂�����ē��͐������Ԃ��߂�����
				else if (inputArrowFlag && inputTime > inputLimitTime)
				{
					inputTime = 0;
					inputArrowFlag = false;
					randomFlag = false;
					arrowFailFlag = false;
					if (inputLimitTime < maxTime)
					{
						inputLimitTime = maxTime;
					}
					else
					{
						inputLimitTime -= 0.2f;
					}
				}
				//�L�[���͂����ꂸ�ɓ��͐������Ԃ��߂�����
				else if (!inputArrowFlag && inputTime > inputLimitTime)
				{

					mVelosity.x = missSpeed.x * addStaminaSpeed;
					inputTime = 0;
					inputLimitTime = limitTime;							//���͐������Ԃ����Z�b�g
					arrowFailFlag = false;
					randomFlag = false;
				}
				break;
			case 3:		//���L�[�̂Ƃ�
				inputEndTime = GetNowCount() / 1000;
				inputTime = (inputEndTime - inputStartTime);			//���͎��Ԃ��J�E���g

				if (Key & PAD_INPUT_RIGHT && !mCheckKeyFlag)
				{
					if (st > MinSt)
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
				else if (!mCheckKeyFlag && Key & PAD_INPUT_UP || !mCheckKeyFlag && Key & PAD_INPUT_DOWN || !mCheckKeyFlag && Key & PAD_INPUT_LEFT)
				{
					if (st > MinSt)
					{
						mVelosity.x = missSpeed.x * addStaminaSpeed;
						st -= ST_FAI_DEC;
						inputLimitTime = limitTime;							//���͐������Ԃ����Z�b�g
						inputArrowFlag = false;
						mCheckKeyFlag = true;
					}
				}
				//�L�[���͂�����ē��͐������Ԃ��߂�����
				else if (inputArrowFlag && inputTime > inputLimitTime)
				{
					inputTime = 0;
					inputArrowFlag = false;
					randomFlag = false;
					arrowFailFlag = false;
					if (inputLimitTime < maxTime)
					{
						inputLimitTime = maxTime;
					}
					else
					{
						inputLimitTime -= 0.2f;
					}
				}
				//�L�[���͂����ꂸ�ɓ��͐������Ԃ��߂�����
				else if (!inputArrowFlag && inputTime > inputLimitTime)
				{
					mVelosity.x = missSpeed.x * addStaminaSpeed;
					inputTime = 0;
					inputLimitTime = limitTime;				//���͐������Ԃ����Z�b�g
					arrowFailFlag = false;
					randomFlag = false;
				}
				break;
			case 4:		//���L�[�̂Ƃ�
				inputEndTime = GetNowCount() / 1000;
				inputTime = (inputEndTime - inputStartTime);  //���͎��Ԃ��J�E���g

				if (Key & PAD_INPUT_LEFT && !mCheckKeyFlag)
				{
					if (st > MinSt)
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
				else if (!mCheckKeyFlag && Key & PAD_INPUT_UP || !mCheckKeyFlag && Key & PAD_INPUT_DOWN || !mCheckKeyFlag && Key & PAD_INPUT_RIGHT)
				{
					if (st > MinSt)
					{
						mVelosity.x = missSpeed.x * addStaminaSpeed;
						st -= ST_FAI_DEC;
						inputLimitTime = limitTime;							//���͐������Ԃ����Z�b�g
						inputArrowFlag = false;
						mCheckKeyFlag = true;
					}
				}
				//�L�[���͂�����ē��͐������Ԃ��߂�����
				else if (inputArrowFlag && inputTime > inputLimitTime)
				{
					inputTime = 0;
					inputArrowFlag = false;
					randomFlag = false;
					arrowFailFlag = false;
					if (inputLimitTime < maxTime)
					{
						inputLimitTime = maxTime;
					}
					else
					{
						inputLimitTime -= 0.2f;
					}
				}
				//�L�[���͂����ꂸ�ɓ��͐������Ԃ��߂�����
				else if (!inputArrowFlag && inputTime > inputLimitTime)
				{

					mVelosity.x = missSpeed.x * addStaminaSpeed;
					inputTime = 0;
					inputLimitTime = limitTime;							//���͐������Ԃ����Z�b�g
					arrowFailFlag = false;
					randomFlag = false;
				}
				break;
			default:
				break;
			}
		}


		// ���p������
		if (randomKeyNumber == STATE_KEY_C)
		{
			inputEndTime = GetNowCount() / 1000;			//���݂̎��Ԃ��擾
			inputTime = (inputEndTime - inputStartTime);	//���ݎ��Ԃƃ����_���ɖ��𐶐��������Ԃ̍�

			// C�L�[�������ꂽ�Ƃ�
			if (Key & PAD_INPUT_C)
			{
				st += 5;			// �X�^�~�i�𑝂₷
				inputTime = 0;		// ���͉\���Ԃ�������
				// �X�^�~�i���ő�l�𒴂�����
				if (st >= MaxSt)
				{
					st = MaxSt;
				}

				mVelosity.x = missSpeed.x * addStaminaSpeed + 1;
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

		//�c��X�^�~�i��50%�ȏ�
		if (st >= halfSt)
		{
			addStaminaSpeed = 1.0f;
		}
		//�c��X�^�~�i��50%�ȉ�
		if (st <= halfSt && st >= quarterSt)
		{
			addStaminaSpeed = 0.8f;
		}
		//�c��X�^�~�i��25%�ȉ�
		if (st <= quarterSt)
		{
			addStaminaSpeed = 0.6f;
		}

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

		//�X�y�[�X�������ꂽ�Ƃ�
		if (Key & PAD_INPUT_M && turnFlag == false)
		{
			inputSpaceFlag = true;
			inputLimitTime = limitTime;			//���͐������Ԃ����Z�b�g
			mPosX = mPosition.x;				//�����ꂽ���̃v���C���[�̍��W��⊮

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
		else if (turnFlag == false && mPosition.x <= -140)
		{
			mEvlt = BAD;						//BAD�]����
			turnGraphHandle = LoadGraph("data/img/Game/Turn1.png");
			turnFlag = true;
			inputLimitTime = limitTime;			//���͐������Ԃ����Z�b�g
		}


		//�v�[���̒[�܂ŗ�����
		if (mPosition.x <= -136)
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

		LastSpurt();	// ���X�g�X�p�[�g

		PlayAnim(_deltaTime);						// �A�j���[�V���������擾
		MV1SetPosition(modelHandle, mPosition);		// �|�W�V�����X�V
	}
}

//�X�^�[�g����
void Ueyama_PlayerActor::StartProcess(float _deltaTime)
{
	//�L�[���͎擾
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	mNowPlayerState = STATE_SWIM;	// �v���C���[���j����Ԃ�

	attachAnim(0);					// �A�j���[�V����000���A�^�b�`

	startFlag = true;               // �X�^�[�g�t���O�� true ��	
}

//�`��
void Ueyama_PlayerActor::DrawActor()
{
	// 1.3f �` 1.5f
	SetDraw3DScale(1.4f);
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
//void Ueyama_PlayerActor::DrawSt(int _st, int _MaxSt, int _MinSt)
//{
//	int color;							//	�X�^�~�i�Q�[�W�̒��g�̐F
//	// �F�ݒ�
//	if (_st >= GREEN)
//	{
//		color = GetColor(0, 255, 0);		//	���g(��)
//	}
//	if (_st < GREEN && _st >= ORANGE)
//	{
//		color = GetColor(255, 177, 27);	//	���g�i�I�����W�j
//	}
//	if (_st < ORANGE)
//	{
//		color = GetColor(255, 0, 0);		//	���g�i�ԁj
//	}
//	int frame_color = GetColor(0, 0, 0);	//	�g (��)
//
//	// �X�^�~�i�Q�[�W�� 0 �ɂȂ�����
//	if (_st <= _MinSt)
//	{
//		// �X�^�~�i���ŏ��l��
//		_st = _MinSt;
//	}
//
//	//// �Q�[�W�̘g�\��
//	//DrawBox(ST_FIRST_X, ST_FIRST_Y, ST_END_X, ST_END_Y, frame_color, FALSE);
//	////// �Q�[�W�̒��g�\��
//	//DrawBox(ST_FIRST_X, ST_FIRST_Y, _st + ST_FIRST_X, ST_END_Y, color, TRUE);
//	DrawRectGraph(ST_FIRST_X, ST_FIRST_Y, ST_FIRST_X, ST_FIRST_Y, _st, ST_HEIGHT, m_stGraphHandle, TRUE, FALSE);
//	//// ���l�\�� 
//	//DrawFormatString(ST_FIRST_X, ST_FIRST_Y, GetColor(0, 0, 0), "%d / 600", _st);
//}

// �S�[���܂ł̋���
void Ueyama_PlayerActor::DrawToGoal(float _playerPos, float _goalPos)
{
	// �ꉝ��������
	if (_playerPos <= 0)
	{
		_playerPos = 0;    // �l���Œ�
		SetFontSize(100);
		DrawFormatString(900, 450, GetColor(255, 0, 0), "GOAL");
	}
	// �c��̋����̕\��
	DrawFormatStringToHandle(DISTANCE_X, DISTANCE_Y, GetColor(255, 255, 255), m_font->gameSceneScoreHandle, " %d", (int)(_goalPos * _playerPos / _goalPos));
}

//	�K�E�Z
void Ueyama_PlayerActor::LastSpurt()
{
	// �c��15m�ȉ��ɂȂ�����
	if (dCount <= dCountUlt)
	{
		//DrawFormatString(850, 700, GetColor(255, 0, 0), "Last Spurt");

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

		if (stopTime % 200 == 0)
		{
			skillCount = 1;
		}
		// �R�b�Ԓ�~
		if (/*stopTime % 240 == 0*/stopTime % 500 == 0 && skillCount == 1)
		{
			skillCount = 2;
		}
		// ��~���Ԃ��I�������
		if (skillCount == 2 && !finishFlag)
		{
			skillFlag = false;
			stopTime = 0;
			mVelosity = VGet(inputCount * 20, 0, 0);

			// �ĊJ���̃Y�������������i�����j
			countUP -= 2;
		}
		// �S�[��������
		if (GetPosX() >= 130 && GetTurnFlag() == true)
		{
			finishFlag = true;
			skillCount = 1;
		}

	}

	// ���l�\��
	//DrawFormatString(850, 700, GetColor(255, 0, 0), "skillTime   %d", skillTime);
	//DrawFormatString(0, 30, GetColor(255, 0, 255), "skillCount   %d", skillCount);
	//DrawFormatString(0, 100, GetColor(255, 0, 255), "stopTime    %d", stopTime);

}

void Ueyama_PlayerActor::UltNumber(bool _randomFlag)
{
	if (!_randomFlag)
	{
		for (int i = 0; i < 4; i++)
		{
			randomKeyNumber = rand() % 3 + 1;				//1�`4�܂ł̐����������_���ɐ���
			arrow[i] = randomKeyNumber;
		}
	}
}

void Ueyama_PlayerActor::UltProcessInput(int _arrow[], int _size)
{
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	count = 0;

	if (skillCount == 1)
	{
		for (int i = 0; i < _size; i++)
		{
			if (ultSucFlag[i] == 0)
			{
				switch (_arrow[i])
				{
				case STATE_KEY_UP:
					if (CheckHitKey(KEY_INPUT_UP) && mCheckKeyFlag)
					{
						inputCount++;
						m_ultFinishFlag++;
						ultSucFlag[i] = 1;
						mCheckKeyFlag = false;
					}
					else if (CheckHitKey(KEY_INPUT_DOWN) && mCheckKeyFlag || CheckHitKey(KEY_INPUT_RIGHT) && mCheckKeyFlag
						|| CheckHitKey(KEY_INPUT_LEFT) && mCheckKeyFlag)
					{
						m_ultCheckFlag++;
						ultSucFlag[i] = 2;
						mCheckKeyFlag = false;
					}
					break;
				case STATE_KEY_DOWN:
					if (CheckHitKey(KEY_INPUT_DOWN) && mCheckKeyFlag)
					{
						inputCount++;
						m_ultFinishFlag++;
						ultSucFlag[i] = 1;
						mCheckKeyFlag = false;
					}
					else if (CheckHitKey(KEY_INPUT_UP) && mCheckKeyFlag || CheckHitKey(KEY_INPUT_RIGHT) && mCheckKeyFlag
						|| CheckHitKey(KEY_INPUT_LEFT) && mCheckKeyFlag)
					{
						m_ultCheckFlag++;
						ultSucFlag[i] = 2;
						mCheckKeyFlag = false;
					}
					break;
				case STATE_KEY_RIGHT:
					if (CheckHitKey(KEY_INPUT_RIGHT) && mCheckKeyFlag)
					{
						inputCount++;
						m_ultFinishFlag++;
						ultSucFlag[i] = 1;
						mCheckKeyFlag = false;
					}
					else if (CheckHitKey(KEY_INPUT_UP) && mCheckKeyFlag || CheckHitKey(KEY_INPUT_DOWN) && mCheckKeyFlag
						|| CheckHitKey(KEY_INPUT_LEFT) && mCheckKeyFlag)
					{
						m_ultCheckFlag++;
						ultSucFlag[i] = 2;
						mCheckKeyFlag = false;
					}
					break;
				case STATE_KEY_LEFT:
					if (CheckHitKey(KEY_INPUT_LEFT) && mCheckKeyFlag)
					{
						inputCount++;
						m_ultFinishFlag++;
						ultSucFlag[i] = 1;
						mCheckKeyFlag = false;
					}
					else if (CheckHitKey(KEY_INPUT_UP) && mCheckKeyFlag || CheckHitKey(KEY_INPUT_DOWN) && mCheckKeyFlag
						|| CheckHitKey(KEY_INPUT_RIGHT) && mCheckKeyFlag)
					{
						m_ultCheckFlag++;
						ultSucFlag[i] = 2;
						mCheckKeyFlag = false;
					}
					break;
				}
			}
			

			if (!CheckHitKey(KEY_INPUT_UP) && !CheckHitKey(KEY_INPUT_DOWN) && !CheckHitKey(KEY_INPUT_RIGHT) && !CheckHitKey(KEY_INPUT_LEFT))
			{
				mCheckKeyFlag = true;
			}
			//mVelosity = VGet(inputCount * 2, 0, 0);
		}
	}
}

void Ueyama_PlayerActor::UltArrowMotion(int _ult1, int _ult2, int _ult3, int _ult4)
{
	if (_ult1 == 1)
	{
		arrowAngle[0]++;
	}
	else if (_ult1 == 2)
	{
		DrawGraph(FIRST_ARROW_X, 0, m_failGraphHandle, TRUE);
	}
	if (_ult2 == 1)
	{
		arrowAngle[1]++;
	}
	else if (_ult2 == 2)
	{
		DrawGraph(SECOND_ARROW_X, 0, m_failGraphHandle, TRUE);
	}
	if (_ult3 == 1)
	{
		arrowAngle[2]++;
	}
	else if (_ult3 == 2)
	{
		DrawGraph(THIRD_ARROW_X, 0, m_failGraphHandle, TRUE);
	}
	if (_ult4 == 1)
	{
		arrowAngle[3]++;
	}
	else if (_ult4 == 2)
	{
		DrawGraph(FORTH_ARROW_X, 0, m_failGraphHandle, TRUE);
	}
}
