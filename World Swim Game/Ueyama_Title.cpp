#include "Ueyama_Title.h"
#include "Ueyama_GameScene.h"
#include "GameScene.h"

#include "DxLib.h"

//-----------------------------------------------------------------------------
//	�E�ύX�_
//-----------------------------------------------------------------------------

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

//	�~�߂鎞��
const int STOP_TIME = 50;

//	�t�F�[�h�C���E�t�F�[�h�A�E�g�̑��x
const int ADD_ALPHA_VAL = 5;
const int ADD_ALPHA_VAL_2 = 10;

//	�C���J�̈ړ�
const int DOLPHIN_MIN_Y = -30;

//	����Y���W
const int ARROW_UP_Y = 0;
const int ARROW_DOWN_Y = 130;

Ueyama_Title::Ueyama_Title(bool _roundFlag)
	: m_alphaVal(300)
	, m_fadeOutFinishFlag(false)
	, m_titleFlag(false)
	, m_addAlphaVal(ADD_ALPHA_VAL)
	, m_addAlphaVal2(ADD_ALPHA_VAL_2)
	, m_transpVal2(0)
	, m_displayCount(0)
	, m_transpDownFlag(false)
	, m_cursolNum(0)
	, m_arrowY(ARROW_UP_Y)
	, m_blueY(0)
	, m_yellowY(DOLPHIN_MIN_Y)
	, m_blueUpFlag(true)
	, m_yellowUpFlag(false)
{
	if (CheckHitKey(KEY_INPUT_RETURN))
	{
		m_checkKeyFlag = true;
	}

	if (_roundFlag)
	{
		m_state = TITLE_SCENE_STATE::FADE_IN;
	}
	else if (!_roundFlag)
	{
		m_state = TITLE_SCENE_STATE::FIRST;
	}

	// ���ߗʕϐ���122�ɐݒ�
	m_transpVal = MAX_TRANSP_VAL;
	m_fadeTransVal = FIRST_TRANS_VAL;
	// �����ߗʕϐ���2�ɐݒ�
	m_permeationAmount = 2;
}

Ueyama_Title::~Ueyama_Title()
{
	StopSoundMem(m_backSoundHandle);

	//	�������̉��
	DeleteGraph(m_backGraphHandle);
	DeleteGraph(m_logoGraphHandle);
	DeleteGraph(m_startGuideGraphHandle);
	DeleteGraph(m_manualGuideGraphHandle);
	DeleteGraph(m_arrowGraphHandle);
	DeleteGraph(m_blueDolphinGraphHandle);
	DeleteGraph(m_yellowDolphinGraphHandle);
	DeleteGraph(m_gateGraphHandle);
	DeleteGraph(m_maouGraphHandle);
	DeleteGraph(m_manualGraphHandle);
	DeleteGraph(m_manual2GraphHandle);
	DeleteGraph(m_manual3GraphHandle);
	DeleteSoundMem(m_backSoundHandle);
	DeleteSoundMem(m_entClickSoundHandle);
	DeleteSoundMem(m_spaClickSoundHandle);

	delete m_volume;
}

SceneBase* Ueyama_Title::Update(float _deltaTime)
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
		if (!CheckHitKey(KEY_INPUT_RETURN) && !CheckHitKey(KEY_INPUT_UP) && !CheckHitKey(KEY_INPUT_DOWN))
		{
			m_checkKeyFlag = false;
		}

		//	���[�h�I��
		if (CheckHitKey(KEY_INPUT_UP) && !m_checkKeyFlag || CheckHitKey(KEY_INPUT_DOWN) && !m_checkKeyFlag)
		{
			//	SE�𗬂�
			PlaySoundMem(m_spaClickSoundHandle, DX_PLAYTYPE_BACK, TRUE);

			// ���L�[���͏d���΍�̃t���O
			m_checkKeyFlag = true;

			if (m_cursolNum == 0)
			{
				m_cursolNum = 1;
			}
			else if (m_cursolNum == 1)
			{
				m_cursolNum = 0;
			}
		}

		// ENTER�őI�����ꂽ�X�e�[�g��
		if (CheckHitKey(KEY_INPUT_RETURN) && m_checkKeyFlag == false)
		{
			// ���L�[���͏d���΍�̃t���O
			m_checkKeyFlag = true;

			//	SE�𗬂�
			PlaySoundMem(m_entClickSoundHandle, DX_PLAYTYPE_BACK, TRUE);

			if (m_cursolNum == 0)
			{
				m_state = TITLE_SCENE_STATE::FADE_OUT;
			}
			else if (m_cursolNum == 1)
			{
				m_state = TITLE_SCENE_STATE::MANUAL;
			}
		}
		break;
	case TITLE_SCENE_STATE::TITLE:
		// ���L�[���͏d���΍�̃t���O
		// ENTER�L�[����w�𗣂�����A����ENTER�̓��͂�L����
		if (!CheckHitKey(KEY_INPUT_RETURN) && !CheckHitKey(KEY_INPUT_UP) && !CheckHitKey(KEY_INPUT_DOWN))
		{
			m_checkKeyFlag = false;
		}

		//	���[�h�I��
		if (CheckHitKey(KEY_INPUT_UP) && !m_checkKeyFlag || CheckHitKey(KEY_INPUT_DOWN) && !m_checkKeyFlag)
		{
			//	SE�𗬂�
			PlaySoundMem(m_spaClickSoundHandle, DX_PLAYTYPE_BACK, TRUE);

			// ���L�[���͏d���΍�̃t���O
			m_checkKeyFlag = true;

			if (m_cursolNum == 0)
			{
				m_cursolNum = 1;
			}
			else if (m_cursolNum == 1)
			{
				m_cursolNum = 0;
			}
		}

		// ENTER�őI�����ꂽ�X�e�[�g��
		if (CheckHitKey(KEY_INPUT_RETURN) && m_checkKeyFlag == false)
		{
			// ���L�[���͏d���΍�̃t���O
			m_checkKeyFlag = true;

			//	SE�𗬂�
			PlaySoundMem(m_entClickSoundHandle, DX_PLAYTYPE_BACK, TRUE);

			if (m_cursolNum == 0)
			{
				m_state = TITLE_SCENE_STATE::FADE_OUT;
			}
			else if (m_cursolNum == 1)
			{
				m_state = TITLE_SCENE_STATE::MANUAL;
			}
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

			m_state = TITLE_SCENE_STATE::MANUAL3;
		}

		break;
	case TITLE_SCENE_STATE::MANUAL3:
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

			m_cursolNum = 0;

			m_state = TITLE_SCENE_STATE::TITLE;
		}

		break;
	case TITLE_SCENE_STATE::FADE_OUT:
		if (m_fadeOutFinishFlag)
		{
			return new Ueyama_GameScene();
			//return new GameScene();
		}
		break;
	default:
		break;
	}

	/*m_deltaTime = _deltaTime;*/

	return this;
}

void Ueyama_Title::Draw()
{

	// ���ߗʍX�V
	UpdateTransparent();

	if (m_state != TITLE_SCENE_STATE::FIRST && m_state != TITLE_SCENE_STATE::SECOND)
	{
		// �`��
		DrawGraph(0, 0, m_backGraphHandle, TRUE);			//	�w�i
		if (m_state != TITLE_SCENE_STATE::MANUAL && m_state != TITLE_SCENE_STATE::MANUAL2 && m_state != TITLE_SCENE_STATE::MANUAL3)
		{
			DrawGraph(0, 0, m_logoGraphHandle, TRUE);			//	���S

			/*m_bigDropAngle += DROP_SPEED;
			m_smallDropAngle -= DROP_SPEED;

			DrawRotaGraph(DROP1_X, DROP1_Y, 1.0, m_bigDropAngle, m_bigDropGraphHandle, TRUE, FALSE);
			DrawRotaGraph(DROP2_X, DROP2_Y, 1.0, m_smallDropAngle, m_smallDropGraphHandle, TRUE, FALSE);*/

			DrawGraph(0, m_arrowY, m_arrowGraphHandle, TRUE);

			if (m_cursolNum == 0)
			{
				m_arrowY = ARROW_UP_Y;
			}
			else if (m_cursolNum == 1)
			{
				m_arrowY = ARROW_DOWN_Y;
			}

			if (m_cursolNum == 0)
			{
				// ���߂��ĕ`��
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_transpVal);
			}
			DrawGraph(0, 0, m_startGuideGraphHandle, TRUE);		//	�X�^�[�g�̈ē�
			if (m_cursolNum == 0)
			{
				// ���߂����ɖ߂�
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}

			if (m_cursolNum == 1)
			{
				// ���߂��ĕ`��
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_transpVal);
			}
			DrawGraph(0, 0, m_manualGuideGraphHandle, TRUE);	//	�}�j���A���ւ̈ē�
			if (m_cursolNum == 1)
			{
				// ���߂����ɖ߂�
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}

			DrawGraph(0, m_blueY, m_blueDolphinGraphHandle, TRUE);
			DrawGraph(0, m_yellowY, m_yellowDolphinGraphHandle, TRUE);

			DlphinMotion();
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
	if (m_state == TITLE_SCENE_STATE::MANUAL3)
	{
		// ���߂��ĕ`��
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 190);
		DrawBox(0, 0, SCREEN_SIZE_W, SCREEN_SIZE_H, GetColor(0, 0, 0), TRUE);
		// ���߂����ɖ߂�
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		//	�`��
		DrawGraph(0, 0, m_manual3GraphHandle, TRUE);
	}

	if (m_state == TITLE_SCENE_STATE::FIRST || m_state == TITLE_SCENE_STATE::SECOND)
	{
		if (!m_titleFlag)
		{
			// �A���t�@�l�̌��Z
			m_alphaVal -= m_addAlphaVal;
		}
		else if (m_titleFlag)
		{
			m_alphaVal += m_addAlphaVal;
		}

		// ��ʑS�̂ɔC�ӂ̃J���[�̎l�p�`��`��
		DrawBox(0, 0, SCREEN_SIZE_W, SCREEN_SIZE_H, GetColor(255, 255, 255), TRUE);

		// �A���t�@�u�����h�L����(�����ŃA���t�@�l���Z�b�g)
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alphaVal);

		// ��ʑS�̂ɔC�ӂ̃J���[�̎l�p�`��`��
		DrawBox(0, 0, SCREEN_SIZE_W, SCREEN_SIZE_H, GetColor(0, 0, 0), TRUE);

		// �A���t�@�u�����h������
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// �A���t�@�l��0�ɂȂ�����t�F�[�h�C���I��
		if (m_alphaVal <= 0 && !m_titleFlag)
		{
			m_alphaVal = 0;
		}
		if (m_alphaVal >= 255 && m_titleFlag)
		{
			m_alphaVal = 255;
			m_state = TITLE_SCENE_STATE::FADE_IN;
		}
	}

	if (m_state == TITLE_SCENE_STATE::FIRST)
	{
		if (m_alphaVal <= 0)
		{
			// ���߂��ĕ`��
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_transpVal2);
			//	�`��
			DrawGraph(0, 0, m_gateGraphHandle, TRUE);			//	GATE
			// ���߂����ɖ߂�
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			if (m_transpDownFlag)
			{
				m_transpVal2 -= m_addAlphaVal;
			}
			else if (!m_transpDownFlag)
			{
				m_transpVal2 += m_addAlphaVal;
			}

			if (m_transpVal2 >= MAX_TRANSP_VAL && !m_transpDownFlag)
			{
				m_displayCount++;
				m_transpVal2 = MAX_TRANSP_VAL;
				if (m_displayCount >= STOP_TIME)
				{
					m_transpDownFlag = true;
					m_displayCount = 0;
				}
			}
			if (m_transpVal2 <= 0 && m_transpDownFlag)
			{
				m_transpVal2 = 0;
				m_transpDownFlag = false;
				m_state = TITLE_SCENE_STATE::SECOND;
			}
		}
	}

	if (m_state == TITLE_SCENE_STATE::SECOND)
	{
		if (m_alphaVal <= 0)
		{
			// ���߂��ĕ`��
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_transpVal2);
			//	�`��
			DrawGraph(0, 0, m_maouGraphHandle, TRUE);			//	GATE
			// ���߂����ɖ߂�
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			if (m_transpDownFlag)
			{
				m_transpVal2 -= m_addAlphaVal;
			}
			else if (!m_transpDownFlag)
			{
				m_transpVal2 += m_addAlphaVal;
			}

			if (m_transpVal2 >= MAX_TRANSP_VAL && !m_transpDownFlag)
			{
				m_displayCount++;
				m_transpVal2 = MAX_TRANSP_VAL;
				if (m_displayCount >= STOP_TIME)
				{
					m_transpDownFlag = true;
					m_displayCount = 0;
				}
			}
			if (m_transpVal2 <= 0 && m_transpDownFlag)
			{
				m_transpVal2 = 0;
				m_transpDownFlag = false;
				m_titleFlag = true;
			}
		}
	}

	//	�t�F�[�h�C������
	if (m_state == TITLE_SCENE_STATE::FADE_IN)
	{
		// �A���t�@�l�̌��Z
		m_alphaVal -= m_addAlphaVal2;

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

void Ueyama_Title::Sound()
{
	//	���ʒ���
	ChangeVolumeSoundMem(m_volume->GetVolumePal(), m_backSoundHandle);
	ChangeVolumeSoundMem(m_volume->GetVolumePal() + DECVOLUMEPAL, m_spaClickSoundHandle);
	ChangeVolumeSoundMem(m_volume->GetVolumePal() + DECVOLUMEPAL, m_entClickSoundHandle);

	if (m_state != TITLE_SCENE_STATE::FIRST && m_state != TITLE_SCENE_STATE::SECOND)
	{
		//	BGM�𗬂�
		PlaySoundMem(m_backSoundHandle, DX_PLAYTYPE_BACK, FALSE);
	}
}

void Ueyama_Title::Load()
{
	//	�O���t�B�b�N�n���h���ɃZ�b�g
	m_backGraphHandle = LoadGraph("data/img/Title/Title_back.png");				//	�w�i
	m_logoGraphHandle = LoadGraph("data/img/Title/Title_logo.png");				//	���S
	m_startGuideGraphHandle = LoadGraph("data/img/Title/Title_start.png");		//	�X�^�[�g�̈ē�
	m_manualGuideGraphHandle = LoadGraph("data/img/Title/Title_manual.png");	//	�}�j���A���ւ̈ē�
	m_arrowGraphHandle = LoadGraph("data/img/Title/Title_arrow.png");			//	���
	m_blueDolphinGraphHandle = LoadGraph("data/img/Title/Title_blue.png");		//	�̃C���J
	m_yellowDolphinGraphHandle = LoadGraph("data/img/Title/Title_yellow.png");	//	���F�̃C���J
	m_gateGraphHandle = LoadGraph("data/img/Title/GATE.png");					//	GATE
	m_maouGraphHandle = LoadGraph("data/img/Title/maou.png");					//	������
	m_manualGraphHandle = LoadGraph("data/img/Manual/Manual_1.png");				//	�}�j���A��
	m_manual2GraphHandle = LoadGraph("data/img/Manual/Manual_2.png");			//	�}�j���A��2
	m_manual3GraphHandle = LoadGraph("data/img/Manual/Manual_3.png");			//	�}�j���A��3

	//	�T�E���h�n���h���ɃZ�b�g
	m_backSoundHandle = LoadSoundMem("data/sound/Title/Title4.ogg");			//	BGM
	m_entClickSoundHandle = LoadSoundMem("data/sound/Title/SE2.mp3");		//	ENTER�L�[���������Ƃ�
	m_spaClickSoundHandle = LoadSoundMem("data/sound/Title/SE1.mp3");		//	SPACE�L�[���������Ƃ�

	m_volume = new Volume();
}

/// �_�ŃG�t�F�N�g�ɗp���铧�ߗʂ̍X�V����
void Ueyama_Title::UpdateTransparent()
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

void Ueyama_Title::DlphinMotion()
{
	if (m_blueY <= DOLPHIN_MIN_Y)
	{
		m_blueUpFlag = false;
	}
	else if (m_blueY >= 0)
	{
		m_blueUpFlag = true;
	}
	if (m_blueUpFlag)
	{
		m_blueY--;
	}
	else if (!m_blueUpFlag)
	{
		m_blueY++;
	}

	if (m_yellowY <= DOLPHIN_MIN_Y)
	{
		m_yellowUpFlag = false;
	}
	else if (m_yellowY >= 0)
	{
		m_yellowUpFlag = true;
	}
	if (m_yellowUpFlag)
	{
		m_yellowY--;
	}
	else if (!m_yellowUpFlag)
	{
		m_yellowY++;
	}
}
