#include "Yamaoka_PlayerActor.h"

#include <random>

const float D_COUNT = 0.355f;

//	�X�^�~�i�Q�[�W�̐F���ς��c��Q�[�W��
const int GREEN = 950;
const int ORANGE = 800;
//const int RED = 3;


//�R���X�g���N�^
Yamaoka_PlayerActor::Yamaoka_PlayerActor()
	: mNowPlayerState(STATE_IDLE)
	, mNowKeyState(STATE_KEY_IDLE)
	, mPrevKeyState(STATE_KEY_IDLE)
	, mEvlt(NONE)
	, startTime(0)
	, tmpTime(0)
	, countUP(0)
	, countDownFinishFlag(false)
	, skillFlag(false)
{
	startFlag = false;
	turnFlag = false;
	randomFlag = false;
	inputSpaceFlag = false;
	inputArrowFlag = false;

	mPosition = VGet(150, 18, 0);								// �����ʒu�ݒ�
	mRotation = VGet(250.0f, 90.0f * DX_PI_F / 180.0f, 0.0f);	// ��]�p�x
	mDirection = VGet(0, 0, 1);
	mVelosity = VGet(10, 0, 0);

	//���f���̃��[�h
	modelHandle = MV1LoadModel("data/swimmer/player.pmx");

	animTotal = 0.0f;
	animNowTime = 0.0f;
	animIndex = 0;

	NowPos = 0;            // ���݂̍��W

	// �S�[���܂ł̋����@( 50m )
	dCount = 50.0f;         // �i�񂾋���
	maxdCount = 50.0f;      // �S�[��

	// �X�^�~�i�Q�[�W //
	st = 1250;      // �X�^�~�i�����l
	MaxSt = 1250;   // �X�^�~�i�ő�l
	MinSt = 650;    // �X�^�~�i�ŏ��l

	count = 30;      // ���̃V�[���ɍs���܂ł̃J�E���g

	countDown = 170; // �J�E���g�_�E���i 3�b �j
	inputTime = 0;

	skillCount = 0;    // �X�L���̎g�p�J�E���g
	skillTime = 180;   // �X�L���̌��ʎ���

}

//�f�X�g���N�^
Yamaoka_PlayerActor::~Yamaoka_PlayerActor()
{
	// ���f���̃A�����[�h
	MV1DeleteModel(modelHandle);
}

//�X�V����
void Yamaoka_PlayerActor::Update(float _deltaTime)
{
	UpdateActor(_deltaTime);
}

//�A�N�^�[�̍X�V����
void Yamaoka_PlayerActor::UpdateActor(float _deltaTime)
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

		mPrevPosition = mPosition;							// �v���C���[�̃|�W�V������⊮
		mPosition.x -= mVelosity.x * _deltaTime;			// �v���C���[�̎����ړ�
		//dCount += (mPosition.x - mPrevPosition.x);		// �c�苗�������炷
		dCount -= std::sqrt((mPosition.x - mPrevPosition.x) * (mPosition.x - mPrevPosition.x)) * 0.088;


		if (randomFlag == false)
		{
			randomKeyNumber = rand() % 3 + 1;				//1�`5�܂ł̐����������_���ɐ���
			inputStartTime = GetNowCount() / 1000;			//�����_���ɖ��𐶐��������Ԃ��擾
			randomFlag = true;
		}

		// ���p���L�[����������
		if (CheckHitKey(KEY_INPUT_C))
		{
			randomKeyNumber = 5;          // randamKeyNumber�� 5 (���p���L�[�̔ԍ�)������
			inputStartTime = GetNowCount() / 1000;			//�����_���ɖ��𐶐��������Ԃ��擾
			randomFlag = true;
		}

		//�v���C���[�̏���//
		//�����_���ɐ����������� STATE_KEY_UP(1) �Ɠ����Ƃ�
		if (randomKeyNumber == STATE_KEY_UP)
		{
			inputEndTime = GetNowCount() / 1000;			//���݂̎��Ԃ��擾
			inputTime = (inputEndTime - inputStartTime);	//���ݎ��Ԃƃ����_���ɖ��𐶐��������Ԃ̍�

			//���L�[�������ꂽ�Ƃ�
			if (Key & PAD_INPUT_UP)
			{
				if (st > MinSt)
				{
					//�^�[��������false�̂Ƃ�
					if (turnFlag == false)
					{
						mVelosity = VGet(15, 0, 0);				//mVelosity�� 15 �ɃZ�b�g
					}
					//�^�[��������true�̂Ƃ�
					else
					{
						mVelosity = VGet(-15, 0, 0);			//mVelosity�� -15 �ɃZ�b�g
					}
					st -= 5;										// �X�^�~�i�����炷
					inputTime = 0;								//���͉\���Ԃ�������
				}
			}
			//�� �� �� �L�[�������ꂽ��
			else if (Key & PAD_INPUT_DOWN || Key & PAD_INPUT_RIGHT || Key & PAD_INPUT_LEFT)
			{
				if (st > MinSt)
				{
					//�^�[��������false�̂Ƃ�
					if (turnFlag == false)
					{
						mVelosity = VGet(5, 0, 0);				//mVelosity�� 5 �ɃZ�b�g
					}
					//�^�[��������true�̂Ƃ�
					else
					{
						mVelosity = VGet(-5, 0, 0);				//mVelosity�� -5 �ɃZ�b�g
					}
					st -= 10;
				}

			}
			//���ݎ��Ԃƃ����_���ɖ��𐶐��������Ԃ̍���1�b��������
			else if (inputTime > 1)
			{
				if (turnFlag == false)
				{
					mVelosity = VGet(5, 0, 0);
				}
				else
				{
					mVelosity = VGet(-5, 0, 0);
				}
				randomFlag = false;
				inputTime = 0;
			}
		}
		//�����_���ɐ����������� STATE_KEY_DOWN(2) �Ɠ����Ƃ�
		if (randomKeyNumber == STATE_KEY_DOWN)
		{
			inputEndTime = GetNowCount() / 1000;
			inputTime = (inputEndTime - inputStartTime);  //���͉\���Ԃ��J�E���g
			
			if (Key & PAD_INPUT_DOWN)
			{
				if (st > MinSt)
				{
					//�^�[��������false�̂Ƃ�
					if (turnFlag == false)
					{
						mVelosity = VGet(15, 0, 0);				//mVelosity�� 15 �ɃZ�b�g
					}
					//�^�[��������true�̂Ƃ�
					else
					{
						mVelosity = VGet(-15, 0, 0);			//mVelosity�� -15 �ɃZ�b�g
					}
					st -= 5;										// �X�^�~�i�����炷
					inputTime = 0;								//���͉\���Ԃ�������
				}

			}
			else if (Key & PAD_INPUT_UP || Key & PAD_INPUT_RIGHT || Key & PAD_INPUT_LEFT)	//�� �L�[�������ꂽ�����͉\���Ԃ�3�b�ɂȂ�����
			{
				if (st > MinSt)
				{
					//�^�[��������false�̂Ƃ�
					if (turnFlag == false)
					{
						mVelosity = VGet(5, 0, 0);				//mVelosity�� 5 �ɃZ�b�g
					}
					//�^�[��������true�̂Ƃ�
					else
					{
						mVelosity = VGet(-5, 0, 0);				//mVelosity�� -5 �ɃZ�b�g
					}
					st -= 10;
				}

			}
			else if (inputTime > 1)
			{
				if (turnFlag == false)
				{
					mVelosity = VGet(5, 0, 0);
				}
				else
				{
					mVelosity = VGet(-5, 0, 0);
				}
				randomFlag = false;
				inputTime = 0;
	
			}
		}
		//�����_���ɐ����������� STATE_KEY_RIGHT(3) �Ɠ����Ƃ�
		if (randomKeyNumber == STATE_KEY_RIGHT)
		{
			inputEndTime = GetNowCount() / 1000;
			inputTime = (inputEndTime - inputStartTime);  //���͉\���Ԃ��J�E���g

			if (Key & PAD_INPUT_RIGHT)
			{
				if (st > MinSt)
				{
					//�^�[��������false�̂Ƃ�
					if (turnFlag == false)
					{
						mVelosity = VGet(15, 0, 0);				//mVelosity�� 15 �ɃZ�b�g
					}
					//�^�[��������true�̂Ƃ�
					else
					{
						mVelosity = VGet(-15, 0, 0);			//mVelosity�� -15 �ɃZ�b�g
					}
					st -= 5;										// �X�^�~�i�����炷
					inputTime = 0;								//���͉\���Ԃ�������
				}
	
			}
			else if (Key & PAD_INPUT_UP || Key & PAD_INPUT_DOWN || Key & PAD_INPUT_LEFT)	//�� �L�[�������ꂽ�����͉\���Ԃ�3�b�ɂȂ�����
			{
				if (st > MinSt)
				{
					//�^�[��������false�̂Ƃ�
					if (turnFlag == false)
					{
						mVelosity = VGet(5, 0, 0);				//mVelosity�� 5 �ɃZ�b�g
					}
					//�^�[��������true�̂Ƃ�
					else
					{
						mVelosity = VGet(-5, 0, 0);				//mVelosity�� -5 �ɃZ�b�g
					}
					st -= 10;
				}
	
			}
			else if (inputTime > 1)
			{
				if (turnFlag == false)
				{
					mVelosity = VGet(5, 0, 0);
				}
				else
				{
					mVelosity = VGet(-5, 0, 0);
				}
				randomFlag = false;
				inputTime = 0;
	
			}
		}
		//�����_���ɐ����������� STATE_KEY_LEFT(4) �Ɠ����Ƃ�
		if (randomKeyNumber == STATE_KEY_LEFT)
		{
			inputEndTime = GetNowCount() / 1000;
			inputTime = (inputEndTime - inputStartTime);  //���͉\���Ԃ��J�E���g

			if (Key & PAD_INPUT_LEFT)
			{
				if (st > MinSt)
				{
					//�^�[��������false�̂Ƃ�
					if (turnFlag == false)
					{
						mVelosity = VGet(15, 0, 0);				//mVelosity�� 15 �ɃZ�b�g
					}
					//�^�[��������true�̂Ƃ�
					else
					{
						mVelosity = VGet(-15, 0, 0);			//mVelosity�� -15 �ɃZ�b�g
					}
					st -= 5;										// �X�^�~�i�����炷
					inputTime = 0;								//���͉\���Ԃ�������
				}
	
			}
			else if (Key & PAD_INPUT_UP || Key & PAD_INPUT_DOWN || Key & PAD_INPUT_RIGHT)	//�� �L�[�������ꂽ�����͉\���Ԃ�3�b�ɂȂ�����
			{
				if (st > MinSt)
				{
					//�^�[��������false�̂Ƃ�
					if (turnFlag == false)
					{
						mVelosity = VGet(5, 0, 0);				//mVelosity�� 5 �ɃZ�b�g
					}
					//�^�[��������true�̂Ƃ�
					else
					{
						mVelosity = VGet(-5, 0, 0);				//mVelosity�� -5 �ɃZ�b�g
					}
					st -= 10;
				}
	
			}
			else if (inputTime > 1)
			{
				if (st > MinSt)
				{
					if (turnFlag == false)
					{
						mVelosity = VGet(5, 0, 0);
					}
					else
					{
						mVelosity = VGet(-5, 0, 0);
					}
					randomFlag = false;
					inputTime = 0;
				}
	
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
				st += 5;			// �X�^�~�i�����炷
				inputTime = 0;		// ���͉\���Ԃ�������
				// �X�^�~�i���ő�l�𒴂�����
				if (st >= MaxSt)
				{
					st = MaxSt;
				}

				// ����������
				if (!turnFlag)
				{
					mVelosity = VGet(2.0, 0, 0);
				}
				else
				{
					mVelosity = VGet(-2.0, 0, 0);
				}
			}
			// ���ݎ��Ԃƃ����_���ɖ��𐶐��������Ԃ̍���1�b��������
			else if (inputTime > 1)
			{
				randomFlag = false;
				inputTime = 0;			// ���͉\���Ԃ�������
			}
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
			if (st <= MinSt && turnFlag == false)
			{
				// ���x��ʏ�̔������x��
				mVelosity = VGet(2.0, 0, 0);
			}
			// �^�[��������
			else if (st <= MinSt && turnFlag == true)
			{
				// ���x��ʏ�̔������x��
				mVelosity = VGet(-2.0, 0, 0);
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
		else if (turnFlag == false && mPosition.x <= -136)		//������Ȃ��܂ܒ[�܂ŗ����Ƃ�
		{
			mEvlt = BAD;						//BAD�]����
		}


		//�v�[���̒[�܂ŗ�����
		if (mPosition.x <= -136)
		{
			turnFlag = true;
			mRotation = VGet(250.0f, 270.0f * DX_PI_F / 180.0f, 0.0f);     //�v���C���[�̌����𔽓]

			switch (mEvlt)
			{
			case BAD:
				mVelosity = VGet(-10, 0, 0);
				st -= 15;

			case NORMAL:
				mVelosity = VGet(-12, 0, 0);
				st -= 10;

			case GOOD:
				mVelosity = VGet(-15, 0, 0);
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
void Yamaoka_PlayerActor::StartProcess(float _deltaTime)
{
	//�L�[���͎擾
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	mNowPlayerState = STATE_SWIM;	// �v���C���[���j����Ԃ�

	attachAnim(0);					// �A�j���[�V����000���A�^�b�`

	startFlag = true;               // �X�^�[�g�t���O�� true ��	
}

// �`��
void Yamaoka_PlayerActor::DrawActor()
{
	// 1.3f �` 1.5f
	SetDraw3DScale(1.4f);
	// 3D���f���̕`��
	MV1DrawModel(modelHandle);

	// 3D���f���̉�]�p�x (�ǂ̕����������Ă��邩)
	MV1SetRotationXYZ(modelHandle, mRotation);
}

//�A�j���[�V����
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
	DrawBox(_MinSt, 1000, _st, 1035, color, TRUE);
	// ���l�\��
	DrawFormatString(_MinSt, 1000, GetColor(0, 0, 0), "%d / 600", _st - _MinSt);
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
	}
}

// �K�E�Z
void Yamaoka_PlayerActor::Skill(float _playerPos, float _goalPos)
{
	SetFontSize(40);

	// �K�E�Z�̃A�C�R���i�g�j��\��
	DrawBox(850, 100, 950, 200, GetColor(0, 0, 0), FALSE);
	DrawFormatString(850,  60, GetColor(255, 0, 0), "�����Z");
	DrawFormatString(850, 200, GetColor(255, 0, 0), "PUSH A");


	// A �������Ɖ����Z���g��
	if (CheckHitKey(KEY_INPUT_A) && st > MinSt)
	{
		skillFlag = true;

		// �K�E�Z�̃A�C�R���i�g�j��h��Ԃ�
		DrawBox(850, 100, 950, 200, GetColor(0, 255, 0), TRUE);

		mPrevKeyState = mNowKeyState;	// ���̃L�[��Ԃ�O��̃L�[��Ԃ�
		mNowKeyState = STATE_KEY_A;		// ���̃L�[��Ԃ�STATE_KEY_A��


		// �X�^�~�i�������ȏ�̂Ƃ�
		if (st - MinSt > MaxSt - MinSt / 2)
		{
			skillTime = 180;
		}
		// �X�^�~�i�������ȉ��̂Ƃ�
		else
		{
			skillTime = 60;
		}


		// �����Z�̏���
		// ���ƑO��̃L�[��Ԃ��Ⴄ�Ƃ�
		if (mNowKeyState != mPrevKeyState)
		{
			// �X�s�[�h�A�b�v
			if (turnFlag)
			{
				// �X�^�~�i���ŏ��l�ɂ���
				st = MinSt;
				// �X�L�����ʎ��Ԃ� 0 �ȏ�̎�
				if (skillTime >= 0)
				{
					mVelosity = VGet(-30, 0, 0);
					skillTime--;
				}
			}
			else
			{
				// �X�^�~�i���ŏ��l�ɂ���
				st = MinSt;
				// �X�L�����ʎ��Ԃ� 0 �ȏ�̎�
				if (skillTime >= 0)
				{
					mVelosity = VGet(30, 0, 0);
					skillTime--;
				}
			}
		}

		// �X�L�����ʎ��Ԃ� 0 �ȉ��ɂȂ��
		if (skillTime <= 0)
		{
			skillFlag = false;
			skillTime = 180;
		}

	}
	else
	{
		// ���݂̃L�[��Ԃ�ύX����
		mNowKeyState = STATE_KEY_S;
	}

	// ���l�\��
	//DrawFormatString(850, 700, GetColor(255, 0, 0), "skillTime   %d", skillTime);

}