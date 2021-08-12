#include "Yamaoka_Result.h"
#include "Ueyama_Title.h"

#include "DxLib.h"

const int SCREEN_SIZE_W = 1920;
const int SCREEN_SIZE_H = 1080;

const int addAlphaVal = 5;

Yamaoka_Result::Yamaoka_Result(const int& _time)
	: m_alphaVal(255)
	, m_fadeOutFinishFlag(false)
	, m_time(_time)
{
	// ���L�[���͏d���΍�̃t���O
	// ENTER�L�[��������Ă���ԁA����ENTER�̓��͂𖳌���
	if (CheckHitKey(KEY_INPUT_RETURN))
	{
		m_checkKeyFlag = true;
	}

	//	�t�F�[�h�C������n�߂�
	m_state = RESULT_SCENE_STATE::FADE_IN;
}

Yamaoka_Result::~Yamaoka_Result()
{
	//	�������̉��
	DeleteGraph(m_backGraphHandle);
	DeleteSoundMem(m_bgmSoundHandle);
}

SceneBase* Yamaoka_Result::Update(float _deltaTime)
{
	switch (m_state)
	{
	case Yamaoka_Result::RESULT_SCENE_STATE::FADE_IN:
		break;
	case Yamaoka_Result::RESULT_SCENE_STATE::RESULT:
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
	case Yamaoka_Result::RESULT_SCENE_STATE::FADE_OUT:
		//	�t�F�[�h�A�E�g���I�������^�C�g����
		if (m_fadeOutFinishFlag)
		{
			return new Ueyama_Title();
		}

		break;
	default:
		break;
	}
	return this;
}

void Yamaoka_Result::Draw()
{
	DrawGraph(0, 0, m_backGraphHandle, TRUE);			//	�w�i
	DrawGraph(0, 0, m_logoGraphHandle, TRUE);			//	���S
	DrawGraph(0, 0, m_evaluationGraphHandle, TRUE);		//	�]��
	DrawGraph(0, 0, m_guidanceGraphHandle, TRUE);		//	�ē�
	DrawGraph(0, 0, m_medalGraphHandle, TRUE);			//	���_��

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

	DrawFormatString(300, 500, GetColor(255, 255, 0), "YOUR TIME   %d", m_time);

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

void Yamaoka_Result::Sound()
{
	//	BGM�𗬂�
	PlaySoundMem(m_bgmSoundHandle, DX_PLAYTYPE_BACK, FALSE);
	ChangeVolumeSoundMem(m_volumePal, m_bgmSoundHandle);			//	����
}

void Yamaoka_Result::Load()
{
	//	�O���t�B�b�N�n���h���ɃZ�b�g
	m_backGraphHandle = LoadGraph("data/img/Result/Result_back.png");			//	�w�i
	m_logoGraphHandle = LoadGraph("data/img/Result/Result_logo.png");			//	���S
	m_evaluationGraphHandle = LoadGraph("data/img/Result/Result_best.png");		//�@�]��
	m_guidanceGraphHandle = LoadGraph("data/img/Result/Result_guidance.png");	//	�ē�
	m_medalGraphHandle = LoadGraph("data/img/Result/Result_gold.png");			//	���_��

	//	�T�E���h�n���h���ɃZ�b�g
	m_bgmSoundHandle = LoadSoundMem("data/sound/Result/resultBgmTest.mp3");			//	BGM
}
