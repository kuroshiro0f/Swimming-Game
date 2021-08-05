#include "Yamaoka_Actor.h"

int Yamaoka_Actor::modelHandle = NULL;

//�R���X�g���N�^
Yamaoka_Actor::Yamaoka_Actor()
{
	// �RD���f���ǂݍ���
	modelHandle = MV1LoadModel("data/player/player.pmx");

	mSpeed = 0.0f;

	// �S�[���܂ł̋��� //
	dCount = 0;       // �i�񂾋���
	maxdCount = 360;  // �S�[��  
	//                 //

	// �X�^�~�i�Q�[�W //
	endStBar = 1150;  // �X�^�~�i�Q�[�W�̒���(1150 - 780)
	startStBar = 780; // �X�^�~�i�Q�[�W�̎n�_�i���[�j
	maxSt = 500;      // �X�^�~�i�ő�l�̐ݒ�
	nowSt = 0;        // ���݂̃X�^�~�i
	dSt = 0;          // �X�^�~�i�̌����l

	// �A�j���[�V�����������ԏ�����
	animNowTime = 0;
	animTotal = 0;
	animIndex = 0;

	mPosition = VGet(0, 18, -150);     // �����ʒu�ݒ�
	mVelocity = VGet(0, 0, 0);         // �������x
	mRotation = VGet(250.0f, 180.0f * DX_PI_F / 180.0f, 0.0f);   // ��]�p�x

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
	if (CheckHitKey(KEY_INPUT_UP))
	{
		mPosition.z += mSpeed;
		mSpeed += 0.1;
		dSt += 0.25;    // �X�^�~�i�����炷
		dCount += 1;
		// ���x���ő�l�𒴂�����
		if (mSpeed >= 1.5f)
		{
			mSpeed = 1.5f;
		}
		// ���܂ōs����
		if (dCount >= 185)
		{
			mSpeed = 0;   // �X�s�[�h���O��
			dCount -= 1;  // �����̃J�E���g�𑊎E
		}
		// ��������
		mRotation = VGet(250.0f, 180.0f * DX_PI_F / 180.0f, 0.0f);
	}
	if (CheckHitKey(KEY_INPUT_DOWN))
	{
		mPosition.z -= mSpeed;
		mSpeed += 0.1;
		dSt += 0.25;  // �X�^�~�i�����炷
		dCount += 1;
		// ���x���ő�l�𒴂�����
		if (mSpeed >= 1.5f)
		{
			mSpeed = 1.5f;
		}
		// �����ʒu�ɖ߂��Ă�����
		if (dCount >= 360)
		{
			mSpeed = 0;    // �X�s�[�h���O��
			dCount = 360;  // �����̃J�E���g���Œ�
		}
		// ��O������
		mRotation = VGet(250.0f, 0.0f * DX_PI_F / 180.0f, 0.0f);
	}
	// �A�j���[�V���������擾
	PlayAnim();

	// ���݂̃X�^�~�i���v�Z
	nowSt = maxSt - dSt;
	// �X�^�~�i�Q�[�W�̒���     (nowSt / maxSt �Ŕ䗦�v�Z)
	endStBar = nowSt / maxSt * endStBar;
	// �{�^���������Ă���Ԃ����X�^�~�i����
	if (endStBar >= 780)
	{
		dSt = 0;
	}

	// �|�W�V�������X�V
	mPosition = VAdd(mPosition, mVelocity);
	MV1SetPosition(modelHandle, mPosition);
}

//�`��
void Yamaoka_Actor::DrawActor()
{
	// 3D���f���̕`��
	MV1DrawModel(modelHandle);

	// 3D���f���̉�]�p�x (�ǂ̕����������Ă��邩)
	MV1SetRotationXYZ(modelHandle, mRotation);

	// �c��̋����̕\��
	DrawFormatString(1500, 850, GetColor(0, 0, 0), "�c��  %d m", maxdCount - dCount);
	// �X�^�~�i�Q�[�W���O�ȏ�̎�
	if (endStBar >= 780)
	{
		// �X�^�~�i�Q�[�W��`��
		DrawBox(startStBar, 1000, endStBar, 1035, GetColor(0, 255, 0), TRUE);
		DrawFormatString(780, 1000, GetColor(0, 0, 0)
			,"%d / 370", (int)endStBar - (int)startStBar);
	}
	else
	{
		endStBar = 0;
	}
	// �ꉝ��������
	if (dCount >= 360)
	{
		DrawFormatString(900, 700, GetColor(255, 0, 0), "GOAL");
	}

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
