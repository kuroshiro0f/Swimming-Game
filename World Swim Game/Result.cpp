#include "Result.h"
#include "Title.h"
#include "Save.h"

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
const int TIME_X = 370;
const int TIME_Y = 460;

// RECORD��\������ꏊ
const int RECORD_X = 300;				//	RECORD�̕���	
const int RECORD_Y = 610;
const int RECORD_TIME_X = 300;			//	���^�C����X���W
const int FIRST_TIME_Y = 710;			//	��ʂ̏ꏊ
const int SECOND_TIME_Y = 810;			//	��ʂ̏ꏊ
const int THIRD_TIME_Y = 910;			//	�O�ʂ̏ꏊ

//	���U���g�̑J�ڂ��鎞��
const int TIME = 100;
const int MEDAL = 150;
const int EVA = 200;
const int ENTER = 250;

//	�ő哧�ߗ�
const int MAX_TRANSP_VAL = 255;
//	���ߗʕω��p�x�N�g��
const int TRANSP_MODERATION = -1;
//	�ŏ��̓��ߗ�
const int FIRST_TRANS_VAL = 100;

//	�]���̊�ƂȂ�b��
const int FIRST = 45;
const int SECOND = 55;
const int THIRD = 65;

//	���̊g��k���͈�
const int STAR_MIN = -30;

//	�t�F�[�h�C���E�t�F�[�h�A�E�g�̑��x
const int addAlphaVal = 5;


Result::Result(const int _time)
	: m_alphaVal(255)
	, m_fadeOutFinishFlag(false)
	, m_time(_time)
	, m_resultFlag(0)
	, m_resultTime(0)
	, m_bigStarX1(0)
	, m_bigStarX2(SCREEN_SIZE_W)
	, m_bigStarY1(0)
	, m_bigStarY2(SCREEN_SIZE_H)
	, m_smallStarX1(0)
	, m_smallStarX2(SCREEN_SIZE_W)
	, m_smallStarY1(0)
	, m_smallStarY2(SCREEN_SIZE_H)
	, m_starScallFlag(TRUE)
{
	// ���L�[���͏d���΍�̃t���O
	// ENTER�L�[��������Ă���ԁA����ENTER�̓��͂𖳌���
	if (CheckHitKey(KEY_INPUT_RETURN))
	{
		m_checkKeyFlag = TRUE;
	}

	SAVE->SetTime(_time);

	// ���ߗʕϐ���122�ɐݒ�
	m_transpVal = MAX_TRANSP_VAL;
	m_fadeTransVal = FIRST_TRANS_VAL;
	// �����ߗʕϐ���2�ɐݒ�
	m_permeationAmount = 2;

	//	�t�F�[�h�C������n�߂�
	m_state = RESULT_SCENE_STATE::FADE_IN;
}

Result::~Result()
{
	//	�������̉��
	DeleteGraph(m_backGraphHandle);
	DeleteGraph(m_logoGraphHandle);
	DeleteGraph(m_guidanceGraphHandle);
	if (m_time <= FIRST)
	{
		DeleteGraph(m_medalGraphHandle);
		DeleteGraph(m_evaluationGraphHandle);

	}
	if (m_time > FIRST && m_time <= SECOND)
	{
		DeleteGraph(m_medalGraphHandle);
		DeleteGraph(m_evaluationGraphHandle);
	}
	if (m_time > SECOND)
	{
		DeleteGraph(m_medalGraphHandle);
		DeleteGraph(m_evaluationGraphHandle);
	}
	DeleteGraph(m_bigStarGraphHandle);
	DeleteGraph(m_smallStarGraphHandle);
	DeleteSoundMem(m_bgmSoundHandle);
	DeleteSoundMem(m_se1SoundHandle);
	DeleteSoundMem(m_se2SoundHandle);
	DeleteSoundMem(m_se3SoundHandle);
	DeleteSoundMem(m_se4SoundHandle);
}

SceneBase* Result::Update(float _deltaTime)
{
	//	���U���g�̌o�ߎ��Ԃ�i�߂�
	m_resultTime++;

	switch (m_state)
	{
	case RESULT_SCENE_STATE::FADE_IN:
		break;
	case RESULT_SCENE_STATE::RESULT:
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

			//	���̃V�[���ֈڍs
			m_resultFlag++;

			//	�X�e�[�g���^�C����
			m_state = RESULT_SCENE_STATE::TIME;
		}

		if (m_resultTime >= TIME)
		{
			//	���̃V�[���ֈڍs
			m_resultFlag++;
			m_state = RESULT_SCENE_STATE::TIME;
		}

		break;
	case RESULT_SCENE_STATE::TIME:
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

			//	���̃V�[���ֈڍs
			m_resultFlag++;

			//	�X�e�[�g�����_����
			m_state = RESULT_SCENE_STATE::MEDAL;
		}

		if (m_resultTime >= MEDAL)
		{
			// ���̃V�[���ֈڍs
			m_resultFlag++;
			m_state = RESULT_SCENE_STATE::MEDAL;
		}

		break;
	case RESULT_SCENE_STATE::MEDAL:
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

			//	���̃V�[���ֈڍs
			m_resultFlag++;

			//	�X�e�[�g���G���^�[��
			m_state = RESULT_SCENE_STATE::EVA;
		}

		if (m_resultTime >= EVA)
		{
			//	���̃V�[���ֈڍs
			m_resultFlag++;
			m_state = RESULT_SCENE_STATE::EVA;
		}

		break;
	case RESULT_SCENE_STATE::EVA:
		m_resultFlag++;
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

			//	���̃V�[���ֈڍs
			m_resultFlag++;

			//	�X�e�[�g���G���^�[��
			m_state = RESULT_SCENE_STATE::ENTER;
		}

		if (m_resultTime >= ENTER)
		{
			//	���̃V�[���ֈڍs
			m_resultFlag++;
			m_state = RESULT_SCENE_STATE::ENTER;
		}

		break;
	case RESULT_SCENE_STATE::ENTER:
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

			//	�X�e�[�g���^�C����
			m_state = RESULT_SCENE_STATE::FADE_OUT;
		}

		break;
	case RESULT_SCENE_STATE::FADE_OUT:
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
	// ���ߗʍX�V
	UpdateTransparent();

	DrawGraph(0, 0, m_backGraphHandle, TRUE);			//	�w�i
	DrawGraph(0, 0, m_logoGraphHandle, TRUE);			//	���S

	// ���߂��ĕ`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 190);
	DrawBox(SQUARE_START_X, SQUARE_START_Y, SQUARE_END_X, SQUARE_END_Y, GetColor(255, 255, 255), TRUE);
	// ���߂����ɖ߂�
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


	if (m_resultFlag >= 1)
	{
		//	�N���A�^�C����\��
		DrawFormatStringToHandle(TIME_X, TIME_Y, GetColor(0, 0, 0), nowTimeHandle, "TIME : %d", m_time);
		DrawFormatStringToHandle(RECORD_X, RECORD_Y, GetColor(0, 0, 0), recordHandle, "RECORD");
		DrawFormatStringToHandle(RECORD_TIME_X, FIRST_TIME_Y, GetColor(0, 0, 0), recordHandle, "1�� : %d", SAVE->GetFirstTime());
		DrawFormatStringToHandle(RECORD_TIME_X, SECOND_TIME_Y, GetColor(0, 0, 0), recordHandle, "2�� : %d", SAVE->GetSecondTime());
		DrawFormatStringToHandle(RECORD_TIME_X, THIRD_TIME_Y, GetColor(0, 0, 0), recordHandle, "3�� : %d", SAVE->GetThirdTime());
	}
	if (m_resultFlag >= 2)
	{
		DrawGraph(0, 0, m_medalGraphHandle, TRUE);			//	���_��
	}
	if (m_resultFlag >= 3)
	{
		DrawGraph(0, 0, m_evaluationGraphHandle, TRUE);		//	�]��
	}
	if (m_resultFlag >= 4)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_transpVal);	//	���߂��ĕ`��
		DrawGraph(0, 0, m_guidanceGraphHandle, TRUE);		//	�ē�
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);			//	���߂����ɖ߂�
		DrawExtendGraph(m_bigStarX1, m_bigStarY1, m_bigStarX2, m_bigStarY2, m_bigStarGraphHandle, TRUE);			//	�傫�Ȑ�
		DrawExtendGraph(m_smallStarX1, m_smallStarY1, m_smallStarX2, m_smallStarY2, m_smallStarGraphHandle, TRUE);	//	�����Ȑ�


		if (m_bigStarX1 <= STAR_MIN)
		{
			m_starScallFlag = FALSE;
		}
		if (m_bigStarX1 >= 0)
		{
			m_starScallFlag = TRUE;
		}

		if (m_starScallFlag == TRUE)
		{
			//	�g�傷��
			m_bigStarX1--;
			m_bigStarX2++;
			m_bigStarY1--;
			m_bigStarY2++;
			m_smallStarX1--;
			m_smallStarX2++;
			m_smallStarY1--;
			m_smallStarY2++;
		}
		else if (m_starScallFlag == FALSE)
		{
			//	�k������
			m_bigStarX1++;
			m_bigStarX2--;
			m_bigStarY1++;
			m_bigStarY2--;
			m_smallStarX1++;
			m_smallStarX2--;
			m_smallStarY1++;
			m_smallStarY2--;
		}
	}

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
	switch (m_resultFlag)
	{
	case 1:
		//	���ʉ��𗬂�
		PlaySoundMem(m_se1SoundHandle, DX_PLAYTYPE_BACK, FALSE);
		break;
	case 2:
		//	���ʉ��𗬂�
		PlaySoundMem(m_se2SoundHandle, DX_PLAYTYPE_BACK, FALSE);
		break;
	case 3:
		//	���ʉ��𗬂�
		PlaySoundMem(m_se3SoundHandle, DX_PLAYTYPE_BACK, FALSE);
		break;
	case 4:
		//	���ʉ��𗬂�
		PlaySoundMem(m_se4SoundHandle, DX_PLAYTYPE_BACK, FALSE);
		break;
	default:
		break;
	}
}

void Result::Load()
{
	//	�O���t�B�b�N�n���h���ɃZ�b�g
	m_backGraphHandle = LoadGraph("data/img/Result/Result_back.png");				//	�w�i
	m_logoGraphHandle = LoadGraph("data/img/Result/Result_logo.png");				//	���S
	m_guidanceGraphHandle = LoadGraph("data/img/Result/Result_guidance.png");		//	�ē�
	if (m_time <= FIRST)
	{
		m_medalGraphHandle = LoadGraph("data/img/Result/Result_gold.png");			//	�����_��
		m_evaluationGraphHandle = LoadGraph("data/img/Result/Result_best.png");		//�@CONGRATURATION
	}
	if (m_time > FIRST && m_time <= SECOND)
	{
		m_medalGraphHandle = LoadGraph("data/img/Result/Result_silver.png");		//	�⃁�_��
		m_evaluationGraphHandle = LoadGraph("data/img/Result/Result_great.png");	//	GREAT
	}
	if (m_time > SECOND)
	{
		m_medalGraphHandle = LoadGraph("data/img/Result/Result_copper.png");		//	�����_��
		m_evaluationGraphHandle = LoadGraph("data/img/Result/Result_good.png");		//�@GOOD
	}
	m_bigStarGraphHandle = LoadGraph("data/img/Result/bigStar.png");				//	�傫�Ȑ�
	m_smallStarGraphHandle = LoadGraph("data/img/Result/smallStar.png");			//	�����Ȑ�

	//	�T�E���h�n���h���ɃZ�b�g
	m_bgmSoundHandle = LoadSoundMem("data/sound/Result/Result.ogg");				//	BGM
	m_se1SoundHandle = LoadSoundMem("data/sound/Result/TestSE.mp3");				//	���ʉ�
	m_se2SoundHandle = LoadSoundMem("data/sound/Result/TestSE.mp3");				//	���ʉ�
	m_se3SoundHandle = LoadSoundMem("data/sound/Result/TestSE.mp3");				//	���ʉ�
	m_se4SoundHandle = LoadSoundMem("data/sound/Result/TestSE.mp3");				//	���ʉ�

	//	���ʒ���
	ChangeVolumeSoundMem(m_volumePal, m_bgmSoundHandle);
	ChangeVolumeSoundMem(m_volumePal, m_se1SoundHandle);
	ChangeVolumeSoundMem(m_volumePal, m_se2SoundHandle);
	ChangeVolumeSoundMem(m_volumePal, m_se3SoundHandle);
	ChangeVolumeSoundMem(m_volumePal, m_se4SoundHandle);
}

void Result::UpdateTransparent()
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
