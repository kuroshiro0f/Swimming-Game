#include "Yamaoka_Actor.h"

int Yamaoka_Actor::modelHandle = NULL;

//�R���X�g���N�^
Yamaoka_Actor::Yamaoka_Actor()
{
	// �RD���f���ǂݍ���
	modelHandle = MV1LoadModel("data/player/player.pmx");

	mSpeed = 0.0f;

	// �S�[���܂ł̋��� //
	dCount = 1000;         // �i�񂾋���
	maxdCount = 1000;      // �S�[��  
	NowPos = 0;            // ���݂̍��W
	// ������          //

	// �X�^�~�i�Q�[�W //
	st = 1200;      // �X�^�~�i�����l
	MaxSt = 1200;   // �X�^�~�i�ő�l
	MinSt = 700;    // �X�^�~�i�ŏ��l

	// �A�j���[�V�����������ԏ�����
	animNowTime = 0;
	animTotal = 0;
	animIndex = 0;

	mPosition = VGet(-150, 18, 0);     // �����ʒu�ݒ�
	mVelocity = VGet(0, 0, 0);         // �������x
	mRotation = VGet(250.0f, -90.0f * DX_PI_F / 180.0f, 0.0f);   // ��]�p�x
}

//�f�X�g���N�^
Yamaoka_Actor::~Yamaoka_Actor()
{
	MV1DeleteModel(modelHandle);
}

// Update�֐��@Game����R�[�������
void Yamaoka_Actor::Update(float _deltaTime)
{
	UpdateActor(_deltaTime);
}

//�A�N�^�[�̍X�V
void Yamaoka_Actor::UpdateActor(float _deltaTime)
{
	// �L�[���͎擾
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// ��������.
	VECTOR accelVec = VGet(0, 0, 0);

	// �x���V�e�B�����v�Z
	mVelocity = VAdd(mVelocity, accelVec);

	attachAnim(0);
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		mPosition.x += mSpeed;
		mSpeed += 0.05;

		dCount -= 1;
		st--;   // �X�^�~�i�����炷
		// ���x���ő�l�𒴂�����
		if (mSpeed >= 1.1f)
		{
			mSpeed = 1.1f;
		}
		// ���܂ōs����
		//if (dCount <= 741)
		if(NowPos >= 271)
		{
			mSpeed = 0;
			dCount += 1;
			st++;
		}
		// �X�^�~�i��0�ɂȂ��
		if (st <= MinSt)
		{
			mSpeed = 0;
		}

		mPosition = VAdd(mPosition, mVelocity);

		// ��������
		mRotation = VGet(250.0f, -90.0f * DX_PI_F / 180.0f, 0.0f);
	}
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		mPosition.x -= mSpeed;
		mSpeed += 0.05;

		dCount -= 1;
		st--;   // �X�^�~�i�����炷
		// ���x���ő�l�𒴂�����
		if (mSpeed >= 1.1f)
		{
			mSpeed = 1.1f;
		}	
		// �X�^�~�i��0�ɂȂ��
		if (st <= MinSt)
		{
			mSpeed = 0;
		}
		// �X�^�[�g�ʒu�ɖ߂�����
		if (NowPos <= 20)
		{
			mSpeed = 0;
		}

		mPosition = VAdd(mPosition, mVelocity);

		// ��O������
		mRotation = VGet(250.0f, 90.0f * DX_PI_F / 180.0f, 0.0f);
	}
	// �A�j���[�V���������擾
	PlayAnim();

	// �c�苗��
	if (dCount <= 500)
	{
		dCount = 500;
	}

	// �|�W�V�������X�V
	mPosition = VAdd(mPosition, mVelocity);
	NowPos = (int)mPosition.x + 150;
	MV1SetPosition(modelHandle, mPosition);
}

//�`��
void Yamaoka_Actor::DrawActor()
{
	// 3D���f���̕`��
	MV1DrawModel(modelHandle);

	// 3D���f���̉�]�p�x (�ǂ̕����������Ă��邩)
	MV1SetRotationXYZ(modelHandle, mRotation);
	
	// �X�^�~�i�Q�[�W�̕`��
	DrawSt(st, MaxSt, MinSt);

	// �S�[���܂ł̋���
	DrawToGoal(dCount, maxdCount);

	// �f�o�b�O�p
	//DrawFormatString(1100, 850, GetColor(0, 0, 0), "dCount  %d", dCount);
}

//�A�j���[�V�����Đ��֐�
void Yamaoka_Actor::PlayAnim()
{
	// �Đ����Ԃ�i�߂�
	animNowTime += 0.5f;
	// �Đ����Ԃ��A�j���[�V�����̑��Đ����Ԃ𒴂�����
	if (animNowTime >= animTotal)
	{
		// �Đ����Ԃ��O��
		animNowTime = 0;
	}

	// �A�j���[�V�����̍Đ����Ԃ��Z�b�g����
	MV1SetAttachAnimTime(modelHandle, animIndex, animNowTime);
}

//�Đ�����A�j���[�V�����̔ԍ����擾����֐�
void Yamaoka_Actor::attachAnim(int _animPlay)
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

// �X�^�~�i�Q�[�W�̕`��
void Yamaoka_Actor::DrawSt(int _st, int _MaxSt, int _MinSt)
{
	// �F�ݒ�
	int color = GetColor(0, 255, 0);  // ���g(��)
	int color2 = GetColor(0, 0, 0);   // �g (��)

	// �X�^�~�i�Q�[�W�� 0 �ɂȂ�����
	if (st <= MinSt)
	{
		// �X�^�~�i���ŏ��l��
		st = MinSt;
	}

	// �Q�[�W�̘g�\��
	DrawBox(MinSt, 1000, 1200, 1035, color2, FALSE);
	// �Q�[�W�̒��g�\��
	DrawBox(MinSt, 1000, 1200 * _st / _MaxSt, 1035, color, TRUE);
	// ���l�\�� 
	DrawFormatString(MinSt, 1000, GetColor(0, 0, 0), "%d / 500", 1200 * _st / _MaxSt - MinSt);
}

// �S�[���܂ł̋���
void Yamaoka_Actor::DrawToGoal(int _playerPos, int _goalPos)
{
	// �f�o�b�O�p
	//DrawFormatString(1300, 500, GetColor(0, 0, 0), "NowPos  %d", NowPos);
	//DrawFormatString(1300, 550, GetColor(0, 0, 0), "  %d", 600 * _playerPos / _goalPos);

	// �c��̋����̕\��
	DrawBox(1590, 895, 1850, 945, GetColor(0, 255, 255), TRUE);
	DrawFormatString(1600, 900, GetColor(0, 0, 0), "�c��  %d m", 1000 * _playerPos/ _goalPos - 500);

	// �ꉝ��������
	if (_playerPos <= 500)
	{
		_playerPos = 500;    // �l���Œ�
		DrawFormatString(900, 950, GetColor(255, 0, 0), "GOAL");
	}

}
