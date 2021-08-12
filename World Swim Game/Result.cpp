#include "Result.h"
#include "Title.h"

#include "DxLib.h"

//	�X�N���[���̃T�C�Y
const int SCREEN_SIZE_W = 1920;
const int SCREEN_SIZE_H = 1080;

//	�^�C���\�����锒���l�p�̏ꏊ�̍��W
const int SQUARE_START_X = 30;
const int SQUARE_START_Y = 370;
const int SQUARE_END_X = 1100;
const int SQUARE_END_Y = 1030;

// �^�C����\������ꏊ
const int TIME_X = (SQUARE_END_X - SQUARE_START_X) / 2 - 40;
const int TIME_Y = (SQUARE_END_Y - SQUARE_START_Y) - 10;

//	�t�F�[�h�C���E�t�F�[�h�A�E�g�̑��x
const int addAlphaVal = 5;


Result::Result(const int _time)
	: m_alphaVal(255)
	, m_fadeOutFinishFlag(false)
{
	// ���L�[���͏d���΍�̃t���O
	// ENTER�L�[��������Ă���ԁA����ENTER�̓��͂𖳌���
	if (CheckHitKey(KEY_INPUT_RETURN))
	{
		m_checkKeyFlag = TRUE;
	}

	//	�t�F�[�h�C������n�߂�
	m_state = RESULT_SCENE_STATE::FADE_IN;
}

Result::~Result()
{
	//	�������̉��
	DeleteGraph(m_backGraphHandle);
	DeleteSoundMem(m_bgmSoundHandle);
}

SceneBase* Result::Update(float _deltaTime)
{
	switch (m_state)
	{
	case Result::RESULT_SCENE_STATE::FADE_IN:
		break;
	case Result::RESULT_SCENE_STATE::RESULT:
		// ���L�[���͏d���΍�̃t���O
		// ENTER�L�[����w�𗣂�����A����ENTER�̓��͂�L����
		if (!CheckHitKey(KEY_INPUT_RETURN))
		{
			m_checkKeyFlag = false;
		}

		// ENTER�Ŏ��̃X�e�[�g��
		if (CheckHitKey(KEY_INPUT_RETURN) && m_checkKeyFlag == false)
		{
			// ���L�[���͏d���΍�̃t���O
			m_checkKeyFlag = true;

			//	�X�e�[�g���t�F�[�h�A�E�g��
			m_state = RESULT_SCENE_STATE::FADE_OUT;
		}

		break;
	case Result::RESULT_SCENE_STATE::FADE_OUT:
		//	�t�F�[�h�A�E�g���I�������^�C�g����
		if (m_fadeOutFinishFlag)
		{
			return new Title();
		}

		break;
	default:
		break;
	}
	return this;
}

void Result::Draw()
{
	//	�`��
	DrawGraph(0, 0, m_backGraphHandle, TRUE);			//	�w�i
	DrawGraph(0, 0, m_logoGraphHandle, TRUE);			//	���S
	DrawGraph(0, 0, m_evaluationGraphHandle, TRUE);		//	�]��
	DrawGraph(0, 0, m_guidanceGraphHandle, TRUE);		//	�ē�
	DrawGraph(0, 0, m_medalGraphHandle, TRUE);			//	���_��

	// ���߂��ĕ`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 190);
	DrawBox(SQUARE_START_X, SQUARE_START_Y, SQUARE_END_X, SQUARE_END_Y, GetColor(255, 255, 255), TRUE);
	// ���߂����ɖ߂�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//	�t�F�[�h�C������
	if (m_state == RESULT_SCENE_STATE::FADE_IN)
	{
		// �A���t�@�l�̌��Z
		m_alphaVal -= addAlphaVal;

		// �A���t�@�u�����h�L����(�����ŃA���t�@�l���Z�b�g)
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alphaVal);

		// ��ʑS�̂ɔC�ӂ̃J���[�̎l�p�`��`��
		DrawBox(0, 0, SCREEN_SIZE_W, SCREEN_SIZE_H, GetColor(0, 0, 0), TRUE);

		// �A���t�@�u�����h������
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// �A���t�@�l���ő�(255)�ɂȂ�����t�F�[�h�A�E�g�I��
		if (m_alphaVal <= 0)
		{
			m_state = RESULT_SCENE_STATE::RESULT;
		}
	}

	// �t�F�[�h�A�E�g����
	if (m_state == RESULT_SCENE_STATE::FADE_OUT)
	{
		// �A���t�@�l�̉��Z
		m_alphaVal += addAlphaVal;
		// �A���t�@�u�����h�L����(�����ŃA���t�@�l���Z�b�g)
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alphaVal);

		// ��ʑS�̂ɔC�ӂ̃J���[�̎l�p�`��`��
		DrawBox(0, 0, SCREEN_SIZE_W, SCREEN_SIZE_H, GetColor(0, 0, 0), TRUE);

		// �A���t�@�u�����h������
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// �A���t�@�l���ő�(255)�ɂȂ�����t�F�[�h�A�E�g�I��
		if (m_alphaVal >= 255)
		{
			m_fadeOutFinishFlag = true;
		}
	}
}

void Result::Sound()
{
	PlaySoundMem(m_bgmSoundHandle, DX_PLAYTYPE_BACK, FALSE);
	ChangeVolumeSoundMem(m_volumePal, m_bgmSoundHandle);
}

void Result::Load()
{
	//	�O���t�B�b�N�n���h���ɃZ�b�g
	m_backGraphHandle = LoadGraph("data/img/Result/Result_back.png");			//	�w�i
	m_logoGraphHandle = LoadGraph("data/img/Result/Result_logo.png");			//	���S
	m_evaluationGraphHandle = LoadGraph("data/img/Result/Result_best.png");		//�@�]��
	m_guidanceGraphHandle = LoadGraph("data/img/Result/Result_guidance.png");	//	�ē�
	m_medalGraphHandle = LoadGraph("data/img/Result/Result_gold.png");			//	���_��

	//	�T�E���h�n���h���ɃZ�b�g
	m_bgmSoundHandle = LoadSoundMem("data/sound/Result/Result.ogg");			//	BGM
}
