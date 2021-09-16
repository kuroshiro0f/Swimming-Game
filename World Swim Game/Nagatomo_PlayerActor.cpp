#include "Nagatomo_PlayerActor.h"

#include <random>

const float D_COUNT = 0.355f;

//�X�s�[�h�֘A
const VECTOR missSpeed = VGet(5, 0, 0);		//���̓~�X�����Ƃ��̃X�s�[�h
const float addSpeed = 3.0f;				//���Z�����X�s�[�h
//���͊֘A
const float limitTime = 1.5f;				//���͐�������

//�R���X�g���N�^
Nagatomo_PlayerActor::Nagatomo_PlayerActor()
	:mNowPlayerState(STATE_IDLE)
	,mNowKeyState(STATE_KEY_IDLE)
	,mPrevKeyState(STATE_KEY_IDLE)
	,mEvlt(NONE)
	, startTime(0)
	, tmpTime(0)
	, countUP(0)
{

	startFlag  = false;
	turnFlag   = false;
	randomFlag = false;
	inputSpaceFlag  = false;
	inputArrowFlag  = false;
	mCheckKeyFlag   = false;

	mPosition = VGet(150, 18, 0);								// �����ʒu�ݒ�
	mRotation = VGet(250.0f, 90.0f * DX_PI_F / 180.0f, 0.0f);	// ��]�p�x
	mDirection = VGet(0, 0, 1);									//�v���C���[�̕���
	mVelosity = VGet(8, 0, 0);									//���x

	//���f���̃��[�h
	modelHandle = MV1LoadModel("data/player/player.pmx");

	animTotal = 0.0f;
	animNowTime = 0.0f;
	animIndex = 0;

	NowPos = 0;            // ���݂̍��W

	// �S�[���܂ł̋����@( 50m ) 
	dCount    = 50.0f;         // �i�񂾋���
	maxdCount = 50.0f;      // �S�[��  

	//�X�^�~�i�֘A
	st        = 600;			// �X�^�~�i�����l
	MaxSt     = 600;			// �X�^�~�i�ő�l
	MinSt     = 300;			// �X�^�~�i�ŏ��l
	halfSt    = 1250 / 2;		//�X�^�~�i�i50%�j
	quarterSt = 1250 / 4;		//�X�^�~�i�i25%�j

	//�J�E���g�_�E��
	count     = 30;				// ���̃V�[���ɍs���܂ł̃J�E���g
	countDown = 120;			// �J�E���g�_�E���i 3�b �j

	//���͊֘A
	inputTime = 0.0f;	
	inputLimitTime = limitTime;

	//�X�s�[�h�֘A
	addStaminaSpeed = 0.0f;
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

		mPrevPosition = mPosition;							//�v���C���[�̃|�W�V������⊮

		//�^�[���O
		if (!turnFlag)
		{
			mPosition.x -= mVelosity.x * _deltaTime;			//�v���C���[�̎����ړ�
		}
		//�^�[����
		else
		{
			mPosition.x += mVelosity.x * _deltaTime;			//�v���C���[�̎����ړ�
		}

		dCount -= std::sqrt((mPosition.x - mPrevPosition.x) * (mPosition.x - mPrevPosition.x)) * 0.088;		// �c�苗�������炷

		if (randomFlag == false)
		{
			randomKeyNumber = rand() % 4 + 1;				//1�`4�܂ł̐����������_���ɐ���
			inputStartTime = GetNowCount() / 1000;			//�����_���ɖ��𐶐��������Ԃ��擾
			randomFlag = true;
			mCheckKeyFlag = false;
		}

		//�v���C���[�̏���//
		switch (randomKeyNumber)
		{
		case 1:	//���L�[�̂Ƃ�
			inputEndTime = GetNowCount() / 1000;			//���݂̎��Ԃ��擾
			inputTime = (inputEndTime - inputStartTime);

			//���L�[�������ꂽ�Ƃ�
			if (Key & PAD_INPUT_UP && !mCheckKeyFlag)
			{
				mVelosity.x += addSpeed * addStaminaSpeed;
				st -= 20;
				inputTime = 0;
				inputArrowFlag = true;
				mCheckKeyFlag = true;
			}
			//�� �� �� �L�[�������ꂽ�Ƃ�
			else if (Key & PAD_INPUT_DOWN || Key & PAD_INPUT_RIGHT || Key & PAD_INPUT_LEFT )
			{
				
				mVelosity.x = missSpeed.x * addStaminaSpeed;
				st -= 40;
				inputLimitTime = limitTime;							//���͐������Ԃ����Z�b�g
				inputArrowFlag = false;
				
			}
			//�L�[���͂�����ē��͐������Ԃ��߂�����
			else if (inputArrowFlag && inputTime > inputLimitTime)
			{
				inputTime = 0;
				inputLimitTime -= 0.2f;	
				randomFlag = false;
			}
			//�L�[���͂����ꂸ�ɓ��͐������Ԃ��߂�����
			else if (!inputArrowFlag && inputTime > inputLimitTime)
			{

				mVelosity.x = missSpeed.x * addStaminaSpeed;
				inputTime = 0;
				inputLimitTime = limitTime;							//���͐������Ԃ����Z�b�g
				randomFlag = false;
			}
			break;
		case 2:		//���L�[�̂Ƃ�
			inputEndTime = GetNowCount() / 1000;
			inputTime = (inputEndTime - inputStartTime);			 //���͎��Ԃ��J�E���g

			if (Key & PAD_INPUT_DOWN && !mCheckKeyFlag)
			{
				mVelosity.x += addSpeed * addStaminaSpeed;
				st -= 20;
				inputTime = 0;
				inputArrowFlag = true;
				mCheckKeyFlag = true;
			}
			else if (Key & PAD_INPUT_UP || Key & PAD_INPUT_RIGHT || Key & PAD_INPUT_LEFT)
			{
				mVelosity.x = missSpeed.x * addStaminaSpeed;
				st -= 40;
				inputLimitTime = limitTime;							//���͐������Ԃ����Z�b�g
				inputArrowFlag = false;
			}
			//�L�[���͂�����ē��͐������Ԃ��߂�����
			else if (inputArrowFlag && inputTime > inputLimitTime)
			{
				inputTime = 0;
				inputLimitTime -= 0.2f;
				randomFlag = false;
			}
			//�L�[���͂����ꂸ�ɓ��͐������Ԃ��߂�����
			else if (!inputArrowFlag && inputTime > inputLimitTime)
			{

				mVelosity.x = missSpeed.x * addStaminaSpeed;
				inputTime = 0;
				inputLimitTime = limitTime;							//���͐������Ԃ����Z�b�g
				randomFlag = false;
			}
			break;
		case 3:		//���L�[�̂Ƃ�
			inputEndTime = GetNowCount() / 1000;
			inputTime = (inputEndTime - inputStartTime);			//���͎��Ԃ��J�E���g

			if (Key & PAD_INPUT_RIGHT && !mCheckKeyFlag)
			{
				mVelosity.x += addSpeed * addStaminaSpeed;
				st -= 20;
				inputTime = 0;
				inputArrowFlag = true;
				mCheckKeyFlag = true;
			}
			else if (Key & PAD_INPUT_UP || Key & PAD_INPUT_DOWN || Key & PAD_INPUT_LEFT)
			{
				mVelosity.x = missSpeed.x * addStaminaSpeed;
				st -= 40;
				inputLimitTime = limitTime;							//���͐������Ԃ����Z�b�g
				inputArrowFlag = false;
			}
			//�L�[���͂�����ē��͐������Ԃ��߂�����
			else if (inputArrowFlag && inputTime > inputLimitTime)
			{
				inputTime = 0;
				inputLimitTime -= 0.2f;	
				randomFlag = false;
			}
			//�L�[���͂����ꂸ�ɓ��͐������Ԃ��߂�����
			else if (!inputArrowFlag && inputTime > inputLimitTime)
			{

				mVelosity.x = missSpeed.x * addStaminaSpeed;
				inputTime = 0;
				inputLimitTime = limitTime;				//���͐������Ԃ����Z�b�g
				randomFlag = false;
			}
			break;
		case 4:		//���L�[�̂Ƃ�
			inputEndTime = GetNowCount() / 1000;
			inputTime = (inputEndTime - inputStartTime);  //���͎��Ԃ��J�E���g

			if (Key & PAD_INPUT_LEFT && !mCheckKeyFlag)
			{
				mVelosity.x += addSpeed * addStaminaSpeed;
				st -= 20;
				inputTime = 0;
				inputArrowFlag = true;
				mCheckKeyFlag = true;
			}
			else if (Key & PAD_INPUT_UP || Key & PAD_INPUT_DOWN || Key & PAD_INPUT_RIGHT)
			{
				mVelosity.x = missSpeed.x * addStaminaSpeed;
				st -= 40;
				inputLimitTime = limitTime;							//���͐������Ԃ����Z�b�g
				inputArrowFlag = false;
			}
			//�L�[���͂�����ē��͐������Ԃ��߂�����
			else if (inputArrowFlag && inputTime > inputLimitTime)
			{
				inputTime = 0;
				inputLimitTime -= 0.2f;
				randomFlag = false;
			}
			//�L�[���͂����ꂸ�ɓ��͐������Ԃ��߂�����
			else if (!inputArrowFlag && inputTime > inputLimitTime)
			{

				mVelosity.x = missSpeed.x * addStaminaSpeed;
				inputTime = 0;
				inputLimitTime = limitTime;							//���͐������Ԃ����Z�b�g
				randomFlag = false;
			}
			break;

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

		//�����߂�����
		if (mPosition.x >= 150)
		{
			mPosition.x = 145;
		}
		if (mPosition.x <= -138)
		{
			mPosition.x = -138;
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

			}
			if (-120 >= mPosX && mPosX > -130)
			{
				mEvlt = NORMAL;					//���͂���������������NORMALE�]��

			}
			if (-130 >= mPosX && mPosX > -140)
			{
				mEvlt = GOOD;					//���͂����傤�ǂ�������GOOD�]��

			}
		}
		//������Ȃ��܂ܒ[�܂ŗ����Ƃ�
		else if (turnFlag == false && mPosition.x <= -136)
		{
			mEvlt = BAD;						//BAD�]����
			turnFlag = true;
			inputLimitTime = limitTime;			//���͐������Ԃ����Z�b�g
		}

		//�v�[���̒[�܂ŗ�����
		if (mPosition.x <= -136)
		{
			turnFlag = true;
			mRotation = VGet(250.0f, 270.0f * DX_PI_F / 180.0f, 0.0f);							//�v���C���[�̌����𔽓]

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
	// 3D���f���̕`��
	MV1DrawModel(modelHandle);

	// 3D���f���̉�]�p�x (�ǂ̕����������Ă��邩)
	MV1SetRotationXYZ(modelHandle, mRotation);

	//�f�o�b�O�p//
	if (randomFlag)
	{
		DrawFormatString(0, 0, GetColor(0, 0, 0), "%d", randomKeyNumber);
	}
	DrawFormatString(0, 50, GetColor(0, 0, 0), "���͉\����:%f", inputTime);
	DrawFormatString(0, 100, GetColor(0, 0, 0), "���͐�������:%f", inputLimitTime);
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
	DrawFormatString(_MinSt, 1000, GetColor(0, 0, 0), "%d / 600", st);

	//--------------//
	//  �f�o�b�O�p  //
	//-------------//

	//if (mEvlt == BAD)
	//{
	//	//�f�o�b�O
	//	DrawFormatString(1600, 900, GetColor(0, 0, 0), "BAD:%f", mPosX);
	//}
	//if (mEvlt == NORMAL)
	//{
	//	//�f�o�b�O
	//	DrawFormatString(1600, 900, GetColor(0, 0, 0), "NORMAL:%f", mPosX);
	//}
	//if (mEvlt == GOOD)
	//{
	//	//�f�o�b�O
	//	DrawFormatString(1600, 900, GetColor(0, 0, 0), "GOOD:%f", mPosX);
	//}
}

// �S�[���܂ł̋���
void Nagatomo_PlayerActor::DrawToGoal(int _dCount)
{
	//-------------//
	// �f�o�b�O�p //
	//------------//

	//DrawFormatString(1300, 500, GetColor(0, 0, 0), "NowPos  %d", count);
	//DrawFormatString(1300, 550, GetColor(0, 0, 0), "  %d", 600 * _playerPos / _goalPos);

	// �c��̋����̕\��
	DrawBox(1590, 895, 1850, 945, GetColor(0, 255, 255), TRUE);
	DrawFormatString(1600, 900, GetColor(0, 0, 0), "�c��  %d m", (_dCount));

	// �ꉝ��������
	if (_dCount <= 0)
	{
		_dCount = 0;    // �l���Œ�
		SetFontSize(100);
		//DrawFormatString(900, 450, GetColor(255, 0, 0), "GOAL");
	}
}
