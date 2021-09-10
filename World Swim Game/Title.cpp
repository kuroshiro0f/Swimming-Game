#include "Title.h"
#include "GameScene.h"

#include "DxLib.h"

//	�E�C���h�E�̃T�C�Y
const int SCREEN_SIZE_W = 1920;
const int SCREEN_SIZE_H = 1080;

//	���ʉ����ʒ���
int DECVOLUMEPAL = 40;

//	�ő哧�ߗ�
const int MAX_TRANSP_VAL = 255;
//	���ߗʕω��p�x�N�g��
const int TRANSP_MODERATION = -1;
//	�ŏ��̓��ߗ�
const int FIRST_TRANS_VAL = 100;

//	�t�F�[�h�C���E�t�F�[�h�A�E�g�̑��x
const float ADD_ALPHA_VAL = 6.0f;
const float ADD_ALPHA_VAL_2 = 30.0f;

////	�~����
//const double PI = 3.1415926535897932384626433832795f;

Title::Title()
	: m_state(TITLE_SCENE_STATE::FIRST)
	, m_alphaVal(300)
	, m_fadeOutFinishFlag(false)
	, m_addAlphaVal(ADD_ALPHA_VAL)
	, m_addAlphaVal2(ADD_ALPHA_VAL_2)
	/*, m_bigDropAngle(PI)
	, m_smallDropAngle(PI)*/
{
	if (CheckHitKey(KEY_INPUT_RETURN))
	{
		m_checkKeyFlag = true;
	}

	// ���ߗʕϐ���122�ɐݒ�
	m_transpVal = MAX_TRANSP_VAL;
	m_fadeTransVal = FIRST_TRANS_VAL;
	// �����ߗʕϐ���2�ɐݒ�
	m_permeationAmount = 2;
}

Title::~Title()
{
	StopSoundMem(m_backSoundHandle);

	//	�������̉��
	DeleteGraph(m_backGraphHandle);
	DeleteGraph(m_logoGraphHandle);
	DeleteGraph(m_startGuideGraphHandle);
	DeleteGraph(m_manualGuideGraphHandle);
	DeleteGraph(m_gateGraphHandle);
	DeleteGraph(m_maouGraphHandle);
	DeleteGraph(m_bigDropGraphHandle);
	DeleteGraph(m_smallDropGraphHandle);
	DeleteGraph(m_manualGraphHandle);
	DeleteSoundMem(m_backSoundHandle);
	DeleteSoundMem(m_entClickSoundHandle);
	DeleteSoundMem(m_spaClickSoundHandle);
}

SceneBase* Title::Update(float _deltaTime)
{
	// �X�e�[�g�����g���Ƃɏ�����ύX
	switch (m_state)
	{
	case TITLE_SCENE_STATE::FIRST:
		break;
	case TITLE_SCENE_STATE::SECOND:
		break;
	case TITLE_SCENE_STATE::FADE_IN:
		// ���L�[���͏d���΍�̃t���O
		// ENTER�L�[����w�𗣂�����A����ENTER�̓��͂�L����
		if (!CheckHitKey(KEY_INPUT_RETURN))
		{
			m_checkKeyFlag = false;
		}

		// ENTER�ōŌ�̃X�e�[�g��
		if (CheckHitKey(KEY_INPUT_RETURN) && m_checkKeyFlag == false)
		{
			// ���L�[���͏d���΍�̃t���O
			m_checkKeyFlag = true;

			//	SE�𗬂�
			PlaySoundMem(m_entClickSoundHandle, DX_PLAYTYPE_BACK, TRUE);

			m_state = TITLE_SCENE_STATE::FADE_OUT;
		}
		break;
	case TITLE_SCENE_STATE::TITLE:
		// ���L�[���͏d���΍�̃t���O
		// ENTER�L�[����w�𗣂�����A����ENTER�̓��͂�L����
		if (!CheckHitKey(KEY_INPUT_RETURN))
		{
			m_checkKeyFlag = false;
		}

		// SPACE�Ń}�j���A����
		if (CheckHitKey(KEY_INPUT_SPACE) && m_checkKeyFlag == false)
		{
			// ���L�[���͏d���΍�̃t���O
			m_checkKeyFlag = true;

			//	SE�𗬂�
			PlaySoundMem(m_spaClickSoundHandle, DX_PLAYTYPE_BACK, TRUE);

			m_state = TITLE_SCENE_STATE::MANUAL;
		}

		// ENTER�Ŏ��̃X�e�[�g��
		if (CheckHitKey(KEY_INPUT_RETURN) && m_checkKeyFlag == false)
		{
			// ���L�[���͏d���΍�̃t���O
			m_checkKeyFlag = true;

			//	SE�𗬂�
			PlaySoundMem(m_entClickSoundHandle, DX_PLAYTYPE_BACK, TRUE);

			m_state = TITLE_SCENE_STATE::FADE_OUT;
		}

		break;
	case TITLE_SCENE_STATE::MANUAL:
		if (!CheckHitKey(KEY_INPUT_RETURN))
		{
			m_checkKeyFlag = false;
		}

		if (CheckHitKey(KEY_INPUT_RETURN) && m_checkKeyFlag == false)
		{
			// ���L�[���͏d���΍�̃t���O
			m_checkKeyFlag = true;

			//	SE�𗬂�
			PlaySoundMem(m_spaClickSoundHandle, DX_PLAYTYPE_BACK, TRUE);

			m_state = TITLE_SCENE_STATE::MANUAL2;
		}

		break;
	case TITLE_SCENE_STATE::MANUAL2:
		if (!CheckHitKey(KEY_INPUT_RETURN))
		{
			m_checkKeyFlag = false;
		}

		if (CheckHitKey(KEY_INPUT_RETURN) && m_checkKeyFlag == false)
		{
			// ���L�[���͏d���΍�̃t���O
			m_checkKeyFlag = true;

			//	SE�𗬂�
			PlaySoundMem(m_spaClickSoundHandle, DX_PLAYTYPE_BACK, TRUE);

			m_state = TITLE_SCENE_STATE::TITLE;
		}

		break;
	case TITLE_SCENE_STATE::FADE_OUT:
		if (m_fadeOutFinishFlag)
		{
			//return new Ueyama_GameScene();
			return new GameScene();
		}
		break;
	default:
		break;
	}

	//m_deltaTime = _deltaTime;

	return this;
}

void Title::Draw()
{
	// ���ߗʍX�V
	UpdateTransparent();

	if (m_state != TITLE_SCENE_STATE::FIRST && m_state != TITLE_SCENE_STATE::SECOND)
	{
		// �`��
		DrawGraph(0, 0, m_backGraphHandle, TRUE);			//	�w�i
		if (m_state != TITLE_SCENE_STATE::MANUAL && m_state != TITLE_SCENE_STATE::MANUAL2)
		{
			DrawGraph(0, 0, m_logoGraphHandle, TRUE);			//	���S

			// ���߂��ĕ`��
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_transpVal);
			DrawGraph(0, 0, m_startGuideGraphHandle, TRUE);		//	�X�^�[�g�̈ē�
			// ���߂����ɖ߂�
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			// ���߂��ĕ`��
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_transpVal);
			DrawGraph(0, 0, m_manualGuideGraphHandle, TRUE);	//	�}�j���A���ւ̈ē�
			// ���߂����ɖ߂�
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}

	if (m_state == TITLE_SCENE_STATE::MANUAL)
	{
		// ���߂��ĕ`��
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 190);
		DrawBox(0, 0, SCREEN_SIZE_W, SCREEN_SIZE_H, GetColor(0, 0, 0), TRUE);
		// ���߂����ɖ߂�
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		//	�`��
		DrawGraph(0, 0, m_manualGraphHandle, TRUE);
	}
	if (m_state == TITLE_SCENE_STATE::MANUAL2)
	{
		// ���߂��ĕ`��
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 190);
		DrawBox(0, 0, SCREEN_SIZE_W, SCREEN_SIZE_H, GetColor(0, 0, 0), TRUE);
		// ���߂����ɖ߂�
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		//	�`��
		DrawGraph(0, 0, m_manual2GraphHandle, TRUE);
	}

	if (m_state == TITLE_SCENE_STATE::FIRST)
	{
		// �A���t�@�l�̌��Z
		m_alphaVal -= m_addAlphaVal;
		if (m_alphaVal <= 255)
		{
			//	�`��
			DrawGraph(0, 0, m_gateGraphHandle, TRUE);			//	GATE

			// �A���t�@�u�����h�L����(�����ŃA���t�@�l���Z�b�g)
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alphaVal);

			// ��ʑS�̂ɔC�ӂ̃J���[�̎l�p�`��`��
			DrawBox(0, 0, SCREEN_SIZE_W, SCREEN_SIZE_H, GetColor(0, 0, 0), TRUE);

			// �A���t�@�u�����h������
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			// �A���t�@�l��0�ɂȂ�����t�F�[�h�C���I��
			if (m_alphaVal <= 0)
			{
				m_state = TITLE_SCENE_STATE::SECOND;
				m_alphaVal = 255;
			}
		}
	}

	if (m_state == TITLE_SCENE_STATE::SECOND)
	{
		//	�`��
		DrawGraph(0, 0, m_maouGraphHandle, TRUE);			//	������

		// �A���t�@�l�̌��Z
		m_alphaVal -= m_addAlphaVal;

		// �A���t�@�u�����h�L����(�����ŃA���t�@�l���Z�b�g)
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alphaVal);

		// ��ʑS�̂ɔC�ӂ̃J���[�̎l�p�`��`��
		DrawBox(0, 0, SCREEN_SIZE_W, SCREEN_SIZE_H, GetColor(0, 0, 0), TRUE);

		// �A���t�@�u�����h������
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 60);

		// �A���t�@�l��0�ɂȂ�����t�F�[�h�C���I��
		if (m_alphaVal <= 0)
		{
			m_state = TITLE_SCENE_STATE::FADE_IN;
			m_alphaVal = 255;
		}
	}

	//	�t�F�[�h�C������
	if (m_state == TITLE_SCENE_STATE::FADE_IN)
	{
		// �A���t�@�l�̌��Z
		m_alphaVal -= m_addAlphaVal;

		// �A���t�@�u�����h�L����(�����ŃA���t�@�l���Z�b�g)
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alphaVal);

		// ��ʑS�̂ɔC�ӂ̃J���[�̎l�p�`��`��
		DrawBox(0, 0, SCREEN_SIZE_W, SCREEN_SIZE_H, GetColor(0, 0, 0), TRUE);

		// �A���t�@�u�����h������
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// �A���t�@�l���ő�(255)�ɂȂ�����t�F�[�h�A�E�g�I��
		if (m_alphaVal <= 0)
		{
			m_alphaVal = 0;
			m_state = TITLE_SCENE_STATE::TITLE;
		}
	}

	// �t�F�[�h�A�E�g����
	if (m_state == TITLE_SCENE_STATE::FADE_OUT)
	{
		// �A���t�@�l�̉��Z
		m_alphaVal += m_addAlphaVal2;
		// �A���t�@�u�����h�L����(�����ŃA���t�@�l���Z�b�g)
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alphaVal);

		// ��ʑS�̂ɔC�ӂ̃J���[�̎l�p�`��`��
		DrawBox(0, 0, SCREEN_SIZE_W, SCREEN_SIZE_H, GetColor(0, 0, 0), TRUE);

		// �A���t�@�u�����h������
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// �A���t�@�l���ő�(255)�ɂȂ�����t�F�[�h�A�E�g�I��
		if (m_alphaVal >= 255)
		{
			m_alphaVal = 255;
			m_fadeOutFinishFlag = true;
		}
	}
}

void Title::Sound()
{
	//	BGM�𗬂�
	PlaySoundMem(m_backSoundHandle, DX_PLAYTYPE_BACK, FALSE);
}

void Title::Load()
{
	//	�O���t�B�b�N�n���h���ɃZ�b�g
	m_backGraphHandle = LoadGraph("data/img/Title/Title_back2.png");			//	�w�i
	m_logoGraphHandle = LoadGraph("data/img/Title/Title_logo.png");				//	���S
	m_startGuideGraphHandle = LoadGraph("data/img/Title/Title_start.png");		//	�X�^�[�g�̈ē�
	m_manualGuideGraphHandle = LoadGraph("data/img/Title/Title_manual.png");	//	�}�j���A���ւ̈ē�
	m_gateGraphHandle = LoadGraph("data/img/Title/GATE.png");					//	GATE
	m_maouGraphHandle = LoadGraph("data/img/Title/maou.png");					//	������
	//m_bigDropGraphHandle = LoadGraph("data/img/Title/drop1.png");				//	�傫�Ȑ��H
	//m_smallDropGraphHandle = LoadGraph("data/img/Title/drop2.png");			//	�����Ȑ��H
	m_manualGraphHandle = LoadGraph("data/img/Manual/Manual.png");				//	�}�j���A��
	m_manual2GraphHandle = LoadGraph("data/img/Manual/Manual2.png");			//	�}�j���A��2

	//	�T�E���h�n���h���ɃZ�b�g
	m_backSoundHandle = LoadSoundMem("data/sound/Title/Title4.ogg");			//	BGM
	m_entClickSoundHandle = LoadSoundMem("data/sound/Title/SE2.mp3");		//	ENTER�L�[���������Ƃ�
	m_spaClickSoundHandle = LoadSoundMem("data/sound/Title/SE1.mp3");		//	SPACE�L�[���������Ƃ�

	//	���ʒ���
	ChangeVolumeSoundMem(m_volumePal, m_backSoundHandle);
	ChangeVolumeSoundMem(m_volumePal + DECVOLUMEPAL, m_spaClickSoundHandle);
	ChangeVolumeSoundMem(m_volumePal + DECVOLUMEPAL, m_entClickSoundHandle);
}

/// �_�ŃG�t�F�N�g�ɗp���铧�ߗʂ̍X�V����
void Title::UpdateTransparent()
{
	// ���ߗʂ�122���傫���Ȃ�����
	if (m_transpVal >= MAX_TRANSP_VAL)
	{
		// ���ߗʂ�255�ɐݒ�
		m_transpVal = MAX_TRANSP_VAL - 1;
		// �����ߗʂ�-1�ɂ���
		m_permeationAmount *= TRANSP_MODERATION;
	}
	// ���ߗʂ�0��菬�������
	else if (MAX_TRANSP_VAL / 2 >= m_transpVal)
	{
		// ���ߗʂ�ݒ�
		m_transpVal = MAX_TRANSP_VAL / 2 + 1;
		// �����ߗʂ�1�ɂ���
		m_permeationAmount *= TRANSP_MODERATION;
	}
	// �����ߗʂ𓧉ߗʂɉ��Z����
	m_transpVal += m_permeationAmount;
}