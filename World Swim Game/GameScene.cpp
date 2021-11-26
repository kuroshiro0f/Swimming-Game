#include "GameScene.h"
#include "Result.h"
#include "Stage.h"
#include "PlayerActor.h"

#include "DxLib.h"

//	�X�N���[���̃T�C�Y
const int SCREEN_SIZE_W = 1920;
const int SCREEN_SIZE_H = 1080;

// �t�F�[�h�C���E�t�F�[�h�A�E�g�̑��x
const int addAlphaVal = 5;

//	�J�E���g�_�E���̑��x
const int COUNTDOWN_SPEED = 50;

//	���[�h����t�@�C����
const int LOAD_FAILE_NUM = 5;

//	TIPS�̐�
const int TIPS_NUM = 3;

//	LOADING�̕����̐؂�ւ�鑬��
const int LOADING_SPEED = 60;

//	�^�C���̕]���̕\������
const int TURN_EVA_TIME = 80;

//	���̏����ʒu�Ə�����ʒu
const int STAR_FIRST_X = 960;
const int STAR_FIRST_Y = 750;
const int STAR_END_X = 1000;
const int STAR_END_Y = 700;

//	���̉�]���x
const double STAR_ROTA_SPEED = 0.1;

//	���̈ړ��͈�
const int SWEAT1_END_X = 40;
const int SWEAT1_END_Y = -40;
const int SWEAT2_END_X = -40;
const int SWEAT2_END_Y = -40;

//	���̔��]��
const int SWEAT_REV = 260;

//	���̒��S���W
const int ARROW_CENTER_X = 990;
const int ARROW_CENTER_Y = 370;

//	���̍��W����
const int ARROW_ADJUST_X = 28;
const int ARROW_ADJUST_Y = -170;

//	���X�g�X�p�[�g�ł̖��̈ʒu����
const int FIRST_ARROW_X = -210;
const int SECOND_ARROW_X = -70;
const int THIRD_ARROW_X = 70;
const int FORTH_ARROW_X = 210;

//	�X�^�~�i�Q�[�W�̐F���ς��c��Q�[�W��
const int GREEN = 300;
const int ORANGE = 150;

//	���p���̍��W
const int BREATH_X = 1260;
const int BREATH_Y = 80;
const int BREATH_X2 = 1370;
const int BREATH_Y2 = 142;

//	���p���̓��ߗ�
const int BREATH_TRANSPAL = 100;

//	���p���̊g��k���ύX�p���W
const int BREATH_CHANGE_X = 8;
const int BREATH_CHANGE_Y = 16;

//	�^�[���̓��ߗ�
const int TURN_TRANSPAL = 100;

//	�j�̎q�̈ړ��͈�
const int BOY_MIN_Y = -50;

//	�^�C���̕\��
const int TIME_X = 1697;
const int TIME_Y = 970;

//	�n�[�g�̕\���ꏊ
const int FIRST_HEART_X = 535;
const int FIRST_HEART_Y = 70;

//	�n�[�g�̌ۓ��̑��x
const int HEART_SPEED_1 = 0.01;
const int HEART_SPEED_2 = 0.04;
const int HEART_SPEED_3 = 0.07;
const int HEART_SPEED_4 = 0.1;

//	�n�[�g�̊g�嗦�̍ő�ƍŏ�
const double HEART_MAX_RATE = 2.0;
const double HEART_MIN_RATE = 0.8;

//	�X�^�~�i�Q�[�W�̕\���ʒu
const int ST_FIRST_X = 665;
const int ST_FIRST_Y = 80;
const int ST_END_X = 1250;
const int ST_END_Y = 1035;
const int ST_HEIGHT = 65;

//	���X�g�̉��o�̏������W�ƈړ����x
const int LAST_FIRST_X = 1920;
const int LAST_SPEED = 60;

//	���ʉ����ʒ���
const int SE_VOLUME_PAL = 50;

//	�~����
const double PI = 3.1415926535897932384626433832795f;
GameScene::GameScene()
	: m_alphaVal(255)
	, m_fadeOutFinishFlag(false)
	, m_loadFlag(true)
	, m_loadFinishFlag(false)
	, m_loadingFlag(0)
	, m_boyPlusFlag(false)
	, m_whistleFinishFlag(false)
	, m_startFinishFlag(false)
	, m_gameFinishFlag(false)
	, m_fadeOutFlag(false)
	, m_sweat1Flag(true)
	, m_sweat2Flag(false)
	, m_heartFlag(false)
	, m_lastFlag(false)
	, m_breathFlag(false)
	, m_stage(nullptr)
	, m_camera(nullptr)
	, m_actor(nullptr)
	, m_mojiX(1470)
	, m_mojiY(980)
	, m_boyY(0)
	, m_sweat1X(0)
	, m_sweat1Y(0)
	, m_sweat2X(0)
	, m_sweat2Y(0)
	, m_timeElapsed(0)
	, m_starX(STAR_FIRST_X)
	, m_starY(STAR_FIRST_Y)
	, m_starAngle(PI)
	, m_heartRate(1.0)
	, m_heartSpeed1(HEART_SPEED_1)
	, m_heartSpeed2(HEART_SPEED_2)
	, m_arrowRate(1.0)
	, m_arrowAngle(0)
	, m_arrow2Angle(0)
	, m_arrow3Angle(0)
	, m_arrow4Angle(0)
	, m_breathX(BREATH_X)
	, m_breathY(BREATH_Y)
	, m_breathX2(BREATH_X2)
	, m_breathY2(BREATH_Y2)
	, m_breathCount(0)
	, m_lastX(LAST_FIRST_X)
{
	m_tipsFlag = GetRand(TIPS_NUM - 1);

	// �X�e�[�g�Z�b�g(�t�F�[�h�C������)
	m_state = GAME_SCENE_STATE::LOAD;
}

GameScene::~GameScene()
{
	//	�������̉��
	StopSoundMem(m_bgmSoundHandle);

	DeleteGraph(m_backGraphHandle);
	DeleteGraph(m_tips1GraphHandle);
	DeleteGraph(m_tips2GraphHandle);
	DeleteGraph(m_tips3GraphHandle);
	DeleteGraph(m_boyGraphHandle);
	DeleteGraph(m_boy2GraphHandle);
	DeleteGraph(m_starGraphHandle);
	DeleteGraph(m_sweat1GraphHandle);
	DeleteGraph(m_sweat2GraphHandle);
	DeleteGraph(m_heartGraphHandle);
	DeleteGraph(m_spaceGraphHandle);
	DeleteGraph(m_arrowBaseGraphHandle);
	DeleteGraph(m_scoreBaseGraphHandle);
	DeleteGraph(m_spaceBaseGraphHandle);
	DeleteGraph(m_spaceBase2GraphHandle);
	DeleteGraph(m_upArrowGraphHandle);
	DeleteGraph(m_rightArrowGraphHandle);
	DeleteGraph(m_downArrowGraphHandle);
	DeleteGraph(m_leftArrowGraphHandle);
	DeleteGraph(m_failGraphHandle);
	DeleteGraph(m_stGraphHandle);
	DeleteGraph(m_st2GraphHandle);
	DeleteGraph(m_st3GraphHandle);
	DeleteGraph(m_finishArrowBaseGraphHandle);
	DeleteGraph(m_breathGraphHandle);
	DeleteGraph(m_stBaseGraphHandle);

	DeleteSoundMem(m_bgmSoundHandle);
	DeleteSoundMem(m_whistleFinishFlag);
	DeleteSoundMem(m_countDownSoundHandle);
	DeleteSoundMem(m_goalSoundHandle);
	DeleteSoundMem(m_arrowSoundHandle);
	DeleteSoundMem(m_arrowFailSoundHandle);
	DeleteSoundMem(m_spaceSoundHandle);
	DeleteSoundMem(m_breathSoundHandle);
	DeleteSoundMem(m_loadSoundHandle);

	delete m_stage;   // �X�e�[�W�̃|�C���^�����o�ϐ�������
	delete m_camera;  // �J�����̃|�C���^�����o�ϐ�������
	delete m_actor;   // �A�N�^�[�̃|�C���^�����o�ϐ����폜
	delete m_font;

	m_effect->StopEffect();
	m_effect->Delete();
	delete m_effect;
}

SceneBase* GameScene::Update(float _deltaTime)
{
	//	�J�������Z�b�g
	m_camera->Update(*m_actor);

	switch (m_state)
	{
	case GAME_SCENE_STATE::LOAD:
		//	���[�h���I�������Q�[����ʂ�
		if (GetASyncLoadNum() == 0)
		{
			m_loadFinishFlag = true;

			// ���L�[���͏d���΍�̃t���O
			// ENTER�L�[����w�𗣂�����A����ENTER�̓��͂�L����
			if (!CheckHitKey(KEY_INPUT_RETURN))
			{
				m_checkKeyFlag = false;
			}

			if (CheckHitKey(KEY_INPUT_RETURN) && !m_checkKeyFlag)
			{
				//	���L�[���͏d���΍�̃t���O
				m_checkKeyFlag == true;
				//	�񓯊��ǂݍ��݂��I��
				SetUseASyncLoadFlag(FALSE);
				//	���ʉ���炷
				PlaySoundMem(m_loadSoundHandle, DX_PLAYTYPE_BACK, FALSE);
				ChangeVolumeSoundMem(m_volumePal + SE_VOLUME_PAL, m_loadSoundHandle);
				//	���̃X�e�[�g��
				m_state = GAME_SCENE_STATE::FADE_IN;
			}
		}
		break;
	case GAME_SCENE_STATE::FADE_IN:
		//	�X�e�[�W���Z�b�g
		m_stage->Update();
		//	�A�N�^�[���Z�b�g
		m_actor->Update(_deltaTime);
		break;
	case GAME_SCENE_STATE::GAME:
		//	�A�N�^�[���X�V
		m_actor->UpdateActor(_deltaTime);

		//	�J�E���g�_�E���J�n
		if (m_actor->countDown >= 0)
		{
			m_actor->countDown -= COUNTDOWN_SPEED * _deltaTime;
		}

		// ���L�[���͏d���΍�̃t���O
		// ENTER�L�[����w�𗣂�����A����ENTER�̓��͂�L����
		if (!CheckHitKey(KEY_INPUT_RETURN))
		{
			m_checkKeyFlag = false;
		}

		//�[�܂Ŗ߂��Ă��Ă��AturnFlag �� true �Ȃ�S�[���̕�����\��
		if (m_actor->GetPosX() >= 130 && m_actor->GetTurnFlag() == true)
		{
			m_gameFinishFlag = true;
		}

		if (m_fadeOutFlag)
		{
			m_state = GAME_SCENE_STATE::FADE_OUT;
		}

		////	�f�o�b�O�p
		//if (CheckHitKey(KEY_INPUT_RETURN))
		//{
		//	m_state = GAME_SCENE_STATE::FADE_OUT;
		//}

		break;
	case GAME_SCENE_STATE::FADE_OUT:
		if (m_fadeOutFinishFlag)
		{
			//	�o�ߎ��Ԃ����U���g�ɓn��
			return new Result(m_actor->countUP);	//	���U���g�V�[���ɐ؂�ւ���
		}
		break;
	default:
		break;
	}

	m_deltaTime = _deltaTime;

	return this;
}

void GameScene::Draw()
{
	if (m_state == GAME_SCENE_STATE::LOAD)
	{
		m_loadingFlag++;
		if (m_loadingFlag >= LOADING_SPEED * 4)
		{
			m_loadingFlag = 0;
		}
		//	���[�h���
		if (m_loadFlag)
		{
			switch (m_tipsFlag)
			{
			case 0:
				DrawGraph(0, 0, m_tips1GraphHandle, TRUE);
				break;
			case 1:
				DrawGraph(0, 0, m_tips2GraphHandle, TRUE);
				break;
			case 2:
				DrawGraph(0, 0, m_tips3GraphHandle, TRUE);
				break;
			default:
				break;
			}
			if (!m_loadFinishFlag)
			{
				if (m_loadingFlag >= 0 && m_loadingFlag < LOADING_SPEED)
				{
					DrawFormatStringToHandle(m_mojiX, m_mojiY, GetColor(255, 255, 0), m_font->loadFontHandle, "Now Loading");
				}
				if (m_loadingFlag >= LOADING_SPEED && m_loadingFlag < LOADING_SPEED * 2)
				{
					DrawFormatStringToHandle(m_mojiX, m_mojiY, GetColor(255, 255, 0), m_font->loadFontHandle, "Now Loading.");
				}
				if (m_loadingFlag >= LOADING_SPEED * 2 && m_loadingFlag < LOADING_SPEED * 3)
				{
					DrawFormatStringToHandle(m_mojiX, m_mojiY, GetColor(255, 255, 0), m_font->loadFontHandle, "Now Loading..");
				}
				if (m_loadingFlag >= LOADING_SPEED * 3 && m_loadingFlag < LOADING_SPEED * 4)
				{
					DrawFormatStringToHandle(m_mojiX, m_mojiY, GetColor(255, 255, 0), m_font->loadFontHandle, "Now Loading...");
				}
				DrawGraph(0, m_boyY, m_boyGraphHandle, TRUE);
			}
			if (m_boyPlusFlag == true)
			{
				m_boyY++;
				if (m_boyY >= 0)
				{
					m_boyY = 0;
					m_boyPlusFlag = false;
				}
			}
			if (m_boyPlusFlag == false)
			{
				m_boyY--;
				if (m_boyY <= BOY_MIN_Y)
				{
					m_boyY = BOY_MIN_Y;
					m_boyPlusFlag = true;
				}
			}
		}

		if (m_loadFinishFlag)
		{
			DrawGraph(0, m_boyY, m_boy2GraphHandle, TRUE);
			DrawFormatStringToHandle(m_mojiX, m_mojiY, GetColor(255, 255, 0), m_font->loadFontHandle, " PRESS ENTER");
		}
	}
	else
	{
		//	�w�i
		DrawGraph(0, 0, m_backGraphHandle, TRUE);

		// �X�e�[�W�̕`��
		m_stage->Draw();
		//SetFontSize(40);

		//	���̓y��̕`��
		if (!m_actor->ultFlag && m_actor->countDown <= 0)
		{
			DrawGraph(0, 0, m_arrowBaseGraphHandle, TRUE);
		}

		//	�n�[�g�̓���
		if (m_actor->st > ORANGE)
		{
			m_heartSpeed1 = HEART_SPEED_2;
			m_heartSpeed2 = -HEART_SPEED_1;
		}
		else if (m_actor->st <= ORANGE)
		{
			m_heartSpeed1 = HEART_SPEED_3;
			m_heartSpeed2 = -HEART_SPEED_2;
		}
		else if (m_actor->st <= ORANGE / 2)
		{
			m_heartSpeed1 = HEART_SPEED_4;
			m_heartSpeed2 = -HEART_SPEED_3;
		}

		//	�n�[�g�̕\��
		DrawRotaGraph(SCREEN_SIZE_W / 2, SCREEN_SIZE_H / 2, m_heartRate, 0, m_heartGraphHandle, TRUE, FALSE);

		//	�n�[�g�̓���
		if (m_heartFlag)
		{
			m_heartSpeed1 = m_heartSpeed2;
		}

		m_heartRate += m_heartSpeed1;

		if (m_heartRate >= HEART_MAX_RATE)
		{
			m_heartRate = HEART_MAX_RATE;
			m_heartFlag = true;
		}
		else if (m_heartRate <= HEART_MIN_RATE)
		{
			m_heartRate = HEART_MIN_RATE;
			m_heartFlag = false;
		}

		//	�^�C���̕\��
		DrawGraph(0, 0, m_scoreBaseGraphHandle, TRUE);		//	�^�C���Ƌ����̓y��
		DrawFormatStringToHandle(TIME_X, TIME_Y, GetColor(255, 255, 255), m_font->gameSceneScoreHandle, "%d", m_actor->countUP);		//	�^�C���̕\��

		//	�X�^�~�i�Ƒ��p���̕\��
		DrawGraph(0, 0, m_stBaseGraphHandle, TRUE);
		if (m_actor->st >= GREEN)
		{
			m_breathCount = 0;
			DrawRectGraph(ST_FIRST_X, ST_FIRST_Y, ST_FIRST_X, ST_FIRST_Y, m_actor->st, ST_HEIGHT, m_stGraphHandle, TRUE, FALSE);
			m_breathX = BREATH_X;
			m_breathY = BREATH_Y;
			// ���߂��ĕ`��
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, BREATH_TRANSPAL);
			DrawGraph(m_breathX, m_breathY, m_breathGraphHandle, TRUE);
			// ���߂����ɖ߂�
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
		else if (m_actor->st < GREEN && m_actor->st >= ORANGE)
		{
			m_breathCount = 0;
			DrawRectGraph(ST_FIRST_X, ST_FIRST_Y, ST_FIRST_X, ST_FIRST_Y, m_actor->st, ST_HEIGHT, m_st2GraphHandle, TRUE, FALSE);
			m_breathX = BREATH_X;
			m_breathY = BREATH_Y;
			DrawGraph(m_breathX, m_breathY, m_breathGraphHandle, TRUE);
		}
		else if (m_actor->st < ORANGE)
		{
			DrawRectGraph(ST_FIRST_X, ST_FIRST_Y, ST_FIRST_X, ST_FIRST_Y, m_actor->st, ST_HEIGHT, m_st3GraphHandle, TRUE, FALSE);
			DrawExtendGraph(m_breathX, m_breathY, m_breathX2, m_breathY2, m_breathGraphHandle, TRUE);
			if (m_breathX <= BREATH_X - BREATH_CHANGE_X)
			{
				m_breathFlag = false;
			}
			else if (m_breathX >= BREATH_X + BREATH_CHANGE_X)
			{
				m_breathFlag = true;
			}

			m_breathCount++;

			if (m_breathFlag && m_breathCount % 2 == 0)
			{
				m_breathX--;
				m_breathY--;
				m_breathX2++;
				m_breathY2++;
			}
			else if (!m_breathFlag && m_breathCount % 2 == 0)
			{
				m_breathX++;
				m_breathY++;
				m_breathX2--;
				m_breathY2--;
			}
		}


		// �v���C���[�`��
		m_actor->DrawActor();

		//	���̕\��
		if (m_actor->st > m_actor->MinSt && m_actor->ultLimitFlag == false)
		{
			//�����_���ɖ���\��
			switch (m_actor->randomKeyNumber)
			{
			case 1:		//�����_���ɐ����������� STATE_KEY_UP(1) �Ɠ����Ƃ�
				DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_arrowAngle, m_upArrowGraphHandle, TRUE, FALSE);
				if (m_actor->inputArrowFlag && m_actor->randomFlag)
				{
					if (m_actor->arrowSoundFlag)
					{
						PlaySoundMem(m_arrowSoundHandle, DX_PLAYTYPE_BACK, FALSE);
						ChangeVolumeSoundMem(m_volumePal + SE_VOLUME_PAL, m_arrowSoundHandle);
						m_actor->arrowSoundFlag = false;
					}
					//	������
					m_arrowAngle++;

					///	�G�t�F�N�g�̍Đ�
					m_effect->PlayEffekseer(m_actor->GetPos());
				}
				else if (!m_actor->randomFlag)
				{
					m_actor->inputArrowFlag = false;
					m_arrowAngle = 0;
					m_effect->StopEffect();
				}
				else if (CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_RIGHT) || CheckHitKey(KEY_INPUT_LEFT))
				{
					//DrawBox(900, 800, 1000, 900, GetColor(255, 0, 0), TRUE);
					m_actor->arrowFailFlag = true;
				}
				if (m_actor->arrowFailFlag)
				{
					if (m_actor->arrowFailSoundFlag)
					{
						PlaySoundMem(m_arrowFailSoundHandle, DX_PLAYTYPE_BACK, FALSE);
						ChangeVolumeSoundMem(m_volumePal + SE_VOLUME_PAL, m_arrowFailSoundHandle);
						m_actor->arrowFailSoundFlag = false;
					}
					DrawGraph(0, 0, m_failGraphHandle, TRUE);
				}
				//DrawFormatString(900, 800, GetColor(0, 0, 0), "��");
				break;

			case 2:		//�����_���ɐ����������� STATE_KEY_DOWN(2) �Ɠ����Ƃ�
				DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_arrowAngle, m_downArrowGraphHandle, TRUE, FALSE);
				if (m_actor->inputArrowFlag && m_actor->randomFlag)
				{
					if (m_actor->arrowSoundFlag)
					{
						PlaySoundMem(m_arrowSoundHandle, DX_PLAYTYPE_BACK, FALSE);
						ChangeVolumeSoundMem(m_volumePal + SE_VOLUME_PAL, m_arrowSoundHandle);
						m_actor->arrowSoundFlag = false;
					}
					//	������
					m_arrowAngle++;

					//	�G�t�F�N�g�̍Đ�
					m_effect->PlayEffekseer(m_actor->GetPos());
				}
				else if (!m_actor->randomFlag)
				{
					m_actor->inputArrowFlag = false;
					m_arrowAngle = 0;
					m_effect->StopEffect();
				}
				else if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_RIGHT) || CheckHitKey(KEY_INPUT_LEFT))
				{
					//DrawBox(900, 800, 1000, 900, GetColor(255, 0, 0), TRUE);
					m_actor->arrowFailFlag = true;
				}
				if (m_actor->arrowFailFlag)
				{
					if (m_actor->arrowFailSoundFlag)
					{
						PlaySoundMem(m_arrowFailSoundHandle, DX_PLAYTYPE_BACK, FALSE);
						ChangeVolumeSoundMem(m_volumePal + SE_VOLUME_PAL, m_arrowFailSoundHandle);
						m_actor->arrowFailSoundFlag = false;
					}
					DrawGraph(0, 0, m_failGraphHandle, TRUE);
				}
				//DrawFormatString(900, 800, GetColor(0, 0, 0), "��");
				break;

			case 3:		//�����_���ɐ����������� STATE_KEY_RIGHT(3) �Ɠ����Ƃ�
				DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_arrowAngle, m_rightArrowGraphHandle, TRUE, FALSE);
				if (m_actor->inputArrowFlag && m_actor->randomFlag)
				{
					if (m_actor->arrowSoundFlag)
					{
						PlaySoundMem(m_arrowSoundHandle, DX_PLAYTYPE_BACK, FALSE);
						ChangeVolumeSoundMem(m_volumePal + SE_VOLUME_PAL, m_arrowSoundHandle);
						m_actor->arrowSoundFlag = false;
					}
					//	������
					m_arrowAngle++;

					//	�G�t�F�N�g�̍Đ�
					m_effect->PlayEffekseer(m_actor->GetPos());
				}
				else if (!m_actor->randomFlag)
				{
					m_actor->inputArrowFlag = false;
					m_arrowAngle = 0;
					m_effect->StopEffect();
				}
				else if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_LEFT))
				{
					//DrawBox(900, 800, 1000, 900, GetColor(255, 0, 0), TRUE);
					m_actor->arrowFailFlag = true;
				}
				if (m_actor->arrowFailFlag)
				{
					if (m_actor->arrowFailSoundFlag)
					{
						PlaySoundMem(m_arrowFailSoundHandle, DX_PLAYTYPE_BACK, FALSE);
						ChangeVolumeSoundMem(m_volumePal + SE_VOLUME_PAL, m_arrowFailSoundHandle);
						m_actor->arrowFailSoundFlag = false;
					}
					DrawGraph(0, 0, m_failGraphHandle, TRUE);
				}
				//DrawFormatString(900, 800, GetColor(0, 0, 0), "��");
				break;

			case 4:		//�����_���ɐ����������� STATE_KEY_LEFT(4) �Ɠ����Ƃ�
				DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_arrowAngle, m_leftArrowGraphHandle, TRUE, FALSE);
				if (m_actor->inputArrowFlag && m_actor->randomFlag)
				{
					if (m_actor->arrowSoundFlag)
					{
						PlaySoundMem(m_arrowSoundHandle, DX_PLAYTYPE_BACK, FALSE);
						ChangeVolumeSoundMem(m_volumePal + SE_VOLUME_PAL, m_arrowSoundHandle);
						m_actor->arrowSoundFlag = false;
					}
					//	������
					m_arrowAngle++;

					//	�G�t�F�N�g�̍Đ�
					m_effect->PlayEffekseer(m_actor->GetPos());
				}
				else if (!m_actor->randomFlag)
				{
					m_actor->inputArrowFlag = false;
					m_arrowAngle = 0;
					m_effect->StopEffect();
				}
				else if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_RIGHT))
				{
					//DrawBox(900, 800, 1000, 900, GetColor(255, 0, 0), TRUE);
					m_actor->arrowFailFlag = true;
				}
				if (m_actor->arrowFailFlag)
				{
					if (m_actor->arrowFailSoundFlag)
					{
						PlaySoundMem(m_arrowFailSoundHandle, DX_PLAYTYPE_BACK, FALSE);
						ChangeVolumeSoundMem(m_volumePal + SE_VOLUME_PAL, m_arrowFailSoundHandle);
						m_actor->arrowFailSoundFlag = false;
					}
					DrawGraph(0, 0, m_failGraphHandle, TRUE);
				}
				//DrawFormatString(900, 800, GetColor(0, 0, 0), "��");
				break;
			default:
				break;
			}
		}

		//	���X�g�X�p�[�g�ł̖��̕`��
		if (m_actor->ultFlag)
		{
			if (m_actor->skillCount == 0)
			{
				DrawGraph(m_lastX, 0, m_lastGraphHandle, TRUE);
				if (m_lastX > 0)
				{
					m_lastX -= LAST_SPEED;
				}
			}
			else if (m_actor->skillCount >= 1)
			{
				//	�y��̕`��
				DrawGraph(0, 0, m_finishArrowBaseGraphHandle, TRUE);
				if (!m_lastFlag)
				{
					m_arrowAngle = 0;
					m_lastFlag = true;
				}
				switch (m_actor->arrow[0])
				{
				case 1:
					DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X + FIRST_ARROW_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_actor->arrowAngle[0], m_upArrowGraphHandle, TRUE, FALSE);
					break;
				case 2:
					DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X + FIRST_ARROW_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_actor->arrowAngle[0], m_downArrowGraphHandle, TRUE, FALSE);
					break;
				case 3:
					DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X + FIRST_ARROW_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_actor->arrowAngle[0], m_rightArrowGraphHandle, TRUE, FALSE);
					break;
				case 4:
					DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X + FIRST_ARROW_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_actor->arrowAngle[0], m_leftArrowGraphHandle, TRUE, FALSE);
					break;
				}

				switch (m_actor->arrow[1])
				{
				case 1:
					DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X + SECOND_ARROW_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_actor->arrowAngle[1], m_upArrowGraphHandle, TRUE, FALSE);
					break;
				case 2:
					DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X + SECOND_ARROW_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_actor->arrowAngle[1], m_downArrowGraphHandle, TRUE, FALSE);
					break;
				case 3:
					DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X + SECOND_ARROW_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_actor->arrowAngle[1], m_rightArrowGraphHandle, TRUE, FALSE);
					break;
				case 4:
					DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X + SECOND_ARROW_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_actor->arrowAngle[1], m_leftArrowGraphHandle, TRUE, FALSE);
					break;
				}
				switch (m_actor->arrow[2])
				{
				case 1:
					DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X + THIRD_ARROW_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_actor->arrowAngle[2], m_upArrowGraphHandle, TRUE, FALSE);
					break;
				case 2:
					DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X + THIRD_ARROW_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_actor->arrowAngle[2], m_downArrowGraphHandle, TRUE, FALSE);
					break;
				case 3:
					DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X + THIRD_ARROW_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_actor->arrowAngle[2], m_rightArrowGraphHandle, TRUE, FALSE);
					break;
				case 4:
					DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X + THIRD_ARROW_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_actor->arrowAngle[2], m_leftArrowGraphHandle, TRUE, FALSE);
					break;
				}
				switch (m_actor->arrow[3])
				{
				case 1:
					DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X + FORTH_ARROW_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_actor->arrowAngle[3], m_upArrowGraphHandle, TRUE, FALSE);
					break;
				case 2:
					DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X + FORTH_ARROW_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_actor->arrowAngle[3], m_downArrowGraphHandle, TRUE, FALSE);
					break;
				case 3:
					DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X + FORTH_ARROW_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_actor->arrowAngle[3], m_rightArrowGraphHandle, TRUE, FALSE);
					break;
				case 4:
					DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X + FORTH_ARROW_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_actor->arrowAngle[3], m_leftArrowGraphHandle, TRUE, FALSE);
					break;
				}
				m_actor->UltArrowMotion(m_actor->ultSucFlag[0], m_actor->ultSucFlag[1], m_actor->ultSucFlag[2], m_actor->ultSucFlag[3]);
			}
		}

		//	�^�[���̕]���̕\��
		if (m_actor->turnGraphFlag)
		{
			m_timeElapsed++;
			DrawGraph(m_turnEvaX, m_turnEvaY, m_actor->turnGraphHandle, TRUE);
			if (m_timeElapsed == TURN_EVA_TIME)
			{
				m_actor->turnGraphFlag = false;
			}
		}

		//// ���p���L�[�̕\��
		//DrawBox(600, 800, 700, 900, GetColor(0, 0, 0), FALSE);				//�{�b�N�X�̕\��(1�p)
		if (CheckHitKey(KEY_INPUT_C))
		{
			m_actor->inputArrowFlag = true;
			//DrawBox(600, 800, 700, 900, GetColor(255, 255, 255), TRUE);
		}
		//�X�y�[�X�L�[��BOX�`��
		if (-100 >= m_actor->GetPosX() && m_actor->GetPosX() > -136 && m_actor->GetInputSpaceFlag() == false && m_actor->GetTurnFlag() == false)
		{
			//�^�[���̕]���� NORMAL �͈̔�
			if (-100 >= m_actor->GetPosX() && m_actor->GetPosX() > -130 || -130 >= m_actor->GetPosX() && m_actor->GetPosX() > -140)
			{
				DrawGraph(0, 0, m_spaceBase2GraphHandle, TRUE);
				DrawGraph(0, 0, m_spaceGraphHandle, TRUE);
			}
			//�^�[���̕]���� GOOD �͈̔�
			if (-130 >= m_actor->GetPosX() && m_actor->GetPosX() > -140)
			{
				DrawGraph(0, 0, m_spaceBaseGraphHandle, TRUE);
				DrawGraph(0, 0, m_spaceGraphHandle, TRUE);
			}
		}

		// �c�苗���̕\��
		m_actor->DrawToGoal(m_actor->dCount, m_actor->maxdCount);

		// �X�L��
		m_actor->LastSpurt();

		//	�X�^�~�i�������̊��̕\��
		if (m_actor->st <= ORANGE && !m_actor->ultFlag)
		{
			if (!m_actor->turnFlag)
			{
				if (m_sweat1Flag)
				{
					DrawGraph(m_sweat1X, m_sweat1Y, m_sweat1GraphHandle, TRUE);
					m_sweat1X++;
					m_sweat1Y--;
					if (m_sweat1X > SWEAT1_END_X)
					{
						m_sweat1X = 0;
						m_sweat1Flag = false;
					}
					if (m_sweat1Y < SWEAT1_END_Y)
					{
						m_sweat1Y = 0;
						m_sweat1Flag = false;
					}
					if (m_sweat1X > SWEAT1_END_X / 2)
					{
						m_sweat2Flag = true;
					}
				}
				if (m_sweat2Flag)
				{
					DrawGraph(m_sweat2X, m_sweat2Y, m_sweat2GraphHandle, TRUE);
					m_sweat2X--;
					m_sweat2Y--;
					if (m_sweat2X < SWEAT2_END_X)
					{
						m_sweat2X = 0;
						m_sweat2Flag = false;
						m_sweat1Flag = true;
					}
					if (m_sweat2Y < SWEAT2_END_Y)
					{
						m_sweat2Y = 0;
						m_sweat2Flag = false;
						m_sweat1Flag = true;
					}
				}
			}
			if (m_actor->turnFlag)
			{
				if (m_sweat1Flag)
				{
					DrawGraph(m_sweat1X - SWEAT_REV, m_sweat1Y, m_sweat1GraphHandle, TRUE);
					m_sweat1X++;
					m_sweat1Y--;
					if (m_sweat1X > SWEAT1_END_X)
					{
						m_sweat1X = 0;
						m_sweat1Flag = false;
					}
					if (m_sweat1Y < SWEAT1_END_Y)
					{
						m_sweat1Y = 0;
						m_sweat1Flag = false;
					}
					if (m_sweat1X > SWEAT1_END_X / 2)
					{
						m_sweat2Flag = true;
					}
				}
				if (m_sweat2Flag)
				{
					DrawGraph(m_sweat2X - SWEAT_REV, m_sweat2Y, m_sweat2GraphHandle, TRUE);
					m_sweat2X--;
					m_sweat2Y--;
					if (m_sweat2X < SWEAT2_END_X)
					{
						m_sweat2X = 0;
						m_sweat2Flag = false;
						m_sweat1Flag = true;
					}
					if (m_sweat2Y < SWEAT2_END_Y)
					{
						m_sweat2Y = 0;
						m_sweat2Flag = false;
						m_sweat1Flag = true;
					}
				}
			}
		}

		// �J�E���g�_�E���̕\��
		if (m_actor->countDown > 0 && m_actor->countDown <= 150)
		{
			//SetFontSize(150);
			DrawFormatStringToHandle(870, 500, GetColor(0, 0, 0), m_font->startHandle, " %d ", m_actor->countDown / 60 + 1);
		}

		//	START!!�̕\��
		if (m_actor->countDown <= 0 && !m_startFinishFlag)
		{
			DrawFormatStringToHandle(730, 500, GetColor(255, 0, 0), m_font->startHandle, "START!!");
			m_startCount++;
			if (m_startCount >= 50)
			{
				m_startFinishFlag = true;
			}
		}

		//	GOAL!!�̕\��
		if (m_gameFinishFlag)
		{
			DrawFormatStringToHandle(740, 500, GetColor(255, 0, 0), m_font->startHandle, "GOAL!!");
			m_finishCount++;
			if (m_finishCount >= 200)
			{
				m_fadeOutFlag = true;
			}
		}

		//	�t�F�[�h�C������
		if (m_state == GAME_SCENE_STATE::FADE_IN)
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
				m_state = GAME_SCENE_STATE::GAME;
			}
		}

		// �t�F�[�h�A�E�g����
		if (m_state == GAME_SCENE_STATE::FADE_OUT)
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
}

void GameScene::Sound()
{
	if (m_state == GAME_SCENE_STATE::GAME && !m_whistleFinishFlag)
	{
		PlaySoundMem(m_countDownSoundHandle, DX_PLAYTYPE_BACK, FALSE);
		ChangeVolumeSoundMem(m_volumePal + SE_VOLUME_PAL, m_countDownSoundHandle);
	}
	//	�J�E���g�_�E���I����A�J��炷
	if (m_actor->countDownFinishFlag)
	{
		PlaySoundMem(m_whistleSoundHandle, DX_PLAYTYPE_BACK, FALSE);
		ChangeVolumeSoundMem(m_volumePal + SE_VOLUME_PAL, m_whistleSoundHandle);
		m_actor->countDownFinishFlag = false;
		m_whistleFinishFlag = true;
	}
	//	�J����I�������ABGM�𗬂�
	if (m_state == GAME_SCENE_STATE::GAME && m_whistleFinishFlag)
	{
		//PlaySoundMem(m_bgmSoundHandle, DX_PLAYTYPE_BACK, FALSE);
		ChangeVolumeSoundMem(m_volumePal, m_bgmSoundHandle);
	}
	if (m_gameFinishFlag)
	{
		PlaySoundMem(m_goalSoundHandle, DX_PLAYTYPE_BACK, FALSE);
		ChangeVolumeSoundMem(m_volumePal + SE_VOLUME_PAL, m_goalSoundHandle);

		if (m_finishCount >= 100)
		{
			StopSoundMem(m_goalSoundHandle);
		}
	}

}

void GameScene::Load()
{
	//	�񓯊��ǂݍ��݂��Ȃ��t�@�C��

	// �O���t�B�b�N�n���h���ɃZ�b�g
	m_tips1GraphHandle = LoadGraph("data/img/Load/TIPS1.png");
	m_tips2GraphHandle = LoadGraph("data/img/Load/TIPS2.png");
	m_tips3GraphHandle = LoadGraph("data/img/Load/TIPS3.png");
	m_boyGraphHandle = LoadGraph("data/img/Load/boy.png");
	
	//	�G�t�F�N�g����
	m_effect = new PlayEffect("data/effects/water4.efk", 3.0f);

	//	�t�H���g�N���X�ւ̃C���X�^���X�𐶐�
	m_font = new Font();

	//	�񓯊��ǂݍ��݊J�n
	SetUseASyncLoadFlag(TRUE);

	// �O���t�B�b�N�n���h���ɃZ�b�g
	m_backGraphHandle = LoadGraph("data/img/Game/gameBackTest.png");							//	�w�i
	m_starGraphHandle = LoadGraph("data/img/Game/star.png");									//	��
	m_sweat1GraphHandle = LoadGraph("data/img/Game/Sweat1.png");								//	��1
	m_sweat2GraphHandle = LoadGraph("data/img/Game/Sweat2.png");								//	��2
	m_boy2GraphHandle = LoadGraph("data/img/Load/boy2.png");									//	�j�̎q2
	m_heartGraphHandle = LoadGraph("data/img/Game/Game_heart.png");								//	�n�[�g
	m_upArrowGraphHandle = LoadGraph("data/img/Game/Game_up.png");								//	����
	m_rightArrowGraphHandle = LoadGraph("data/img/Game/Game_right.png");						//	�E���
	m_downArrowGraphHandle = LoadGraph("data/img/Game/Game_down.png");							//	�����
	m_leftArrowGraphHandle = LoadGraph("data/img/Game/Game_left.png");							//	�����
	m_failGraphHandle = LoadGraph("data/img/Game/Game_fail.png");								//	���s
	m_spaceGraphHandle = LoadGraph("data/img/Game/Game_space.png");								//	�X�y�[�X
	m_stGraphHandle = LoadGraph("data/img/Game/Game_st.png");									//	�X�^�~�i
	m_st2GraphHandle = LoadGraph("data/img/Game/Game_st2.png");									//	�X�^�~�i2
	m_st3GraphHandle = LoadGraph("data/img/Game/Game_st3.png");									//	�X�^�~�i3
	m_breathGraphHandle = LoadGraph("data/img/Game/Game_breath.png");							//	���p��
	m_arrowBaseGraphHandle = LoadGraph("data/img/Game/Game_arrowBase.png");						//	���̓y��
	m_scoreBaseGraphHandle = LoadGraph("data/img/Game/Game_score.png");							//	�X�R�A�̓y��
	m_spaceBaseGraphHandle = LoadGraph("data/img/Game/Game_spaceBase.png");						//	�X�y�[�X�̓y��
	m_spaceBase2GraphHandle = LoadGraph("data/img/Game/Game_spaceBase2.png");					//	�X�y�[�X�̓y��2
	m_stBaseGraphHandle = LoadGraph("data/img/Game/Game_stBase.png");							//	�X�^�~�i�̓y��
	m_finishArrowBaseGraphHandle = LoadGraph("data/img/Game/Game_finishArrowBase.png");			//	���X�g�̖��̓y��
	m_lastGraphHandle = LoadGraph("data/img/Game/Game_last.png");								//	���X�g�̉��o

	//	�T�E���h�n���h���ɃZ�b�g
	m_bgmSoundHandle = LoadSoundMem("data/sound/Game/Game.ogg");								//	BGM
	m_whistleSoundHandle = LoadSoundMem("data/sound/Game/whistle.ogg");							//	�J
	m_countDownSoundHandle = LoadSoundMem("data/sound/Game/Countdown2.ogg");					//	�J�E���g�_�E��
	m_goalSoundHandle = LoadSoundMem("data/sound/Game/goalWhistle.ogg");						//	�S�[��
	m_arrowSoundHandle = LoadSoundMem("data/sound/Game/arrow.ogg");								//	���
	m_arrowFailSoundHandle = LoadSoundMem("data/sound/Game/arrowFail.ogg");						//	���̎��s
	m_spaceSoundHandle = LoadSoundMem("data/sound/Game/space.ogg");								//	�X�y�[�X
	m_breathSoundHandle = LoadSoundMem("data/sound/Game/breath.ogg");							//	���p��
	m_loadSoundHandle = LoadSoundMem("data/sound/Game/load.ogg");								//	���[�h

	// �X�e�[�W�N���X�̃C���X�^���X�𐶐�
	m_stage = new Stage();
	// �A�N�^�[�N���X�ւ̃C���X�^���X�𐶐�
	m_actor = new PlayerActor;
	// �J�����N���X�ւ̃C���X�^���X�𐶐�
	m_camera = new Camera(*m_actor);
}
