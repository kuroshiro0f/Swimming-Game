#include "Yamaoka_GameScene.h"
#include "Result.h"
#include "Stage.h"
#include "Yamaoka_PlayerActor.h"

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
const int SWEAT_REV = 500;

//	�X�^�~�i�Q�[�W�̐F���ς��c��Q�[�W��
const int GREEN = 300;
const int ORANGE = 150;

//	�j�̎q�̈ړ��͈�
const int BOY_MIN_Y = -50;

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
	, m_stage(nullptr)
	, m_camera(nullptr)
	, m_actor(nullptr)
	, m_mojiX(1450)
	, m_mojiY(1000)
	, m_boyY(0)
	, m_sweat1X(0)
	, m_sweat1Y(0)
	, m_sweat2X(0)
	, m_sweat2Y(0)
	, m_timeElapsed(0)
	, m_starX(STAR_FIRST_X)
	, m_starY(STAR_FIRST_Y)
	, m_starAngle(PI)
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
	DeleteGraph(m_starGraphHandle);
	DeleteGraph(m_sweat1GraphHandle);
	DeleteGraph(m_sweat2GraphHandle);
	DeleteSoundMem(m_bgmSoundHandle);
	DeleteSoundMem(m_whistleFinishFlag);
	DeleteSoundMem(m_countDownSoundHandle);
	DeleteSoundMem(m_goalSoundHandle);

	delete m_stage;   // �X�e�[�W�̃|�C���^�����o�ϐ�������
	delete m_camera;  // �J�����̃|�C���^�����o�ϐ�������
	delete m_actor;   // �A�N�^�[�̃|�C���^�����o�ϐ����폜

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

			if (CheckHitKey(KEY_INPUT_RETURN))
			{
				//	�񓯊��ǂݍ��݂��I�����A���̃X�e�[�g��
				SetUseASyncLoadFlag(FALSE);
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

		//	�f�o�b�O�p
		if (CheckHitKey(KEY_INPUT_RETURN))
		{
			m_state = GAME_SCENE_STATE::FADE_OUT;
		}
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
					DrawFormatStringToHandle(m_mojiX, m_mojiY, GetColor(255, 255, 0), loadFontHandle, "Now Loading");
				}
				if (m_loadingFlag >= LOADING_SPEED && m_loadingFlag < LOADING_SPEED * 2)
				{
					DrawFormatStringToHandle(m_mojiX, m_mojiY, GetColor(255, 255, 0), loadFontHandle, "Now Loading.");
				}
				if (m_loadingFlag >= LOADING_SPEED * 2 && m_loadingFlag < LOADING_SPEED * 3)
				{
					DrawFormatStringToHandle(m_mojiX, m_mojiY, GetColor(255, 255, 0), loadFontHandle, "Now Loading..");
				}
				if (m_loadingFlag >= LOADING_SPEED * 3 && m_loadingFlag < LOADING_SPEED * 4)
				{
					DrawFormatStringToHandle(m_mojiX, m_mojiY, GetColor(255, 255, 0), loadFontHandle, "Now Loading...");
				}
			}
			DrawGraph(0, m_boyY, m_boyGraphHandle, TRUE);
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
			DrawFormatStringToHandle(m_mojiX, m_mojiY, GetColor(255, 255, 0), loadFontHandle, "PLEASE ENTER");
		}
	}
	else
	{
		//	�w�i
		DrawGraph(0, 0, m_backGraphHandle, TRUE);

		// �X�e�[�W�̕`��
		m_stage->Draw();
		SetFontSize(40);

		//	�^�C���̕\��
		DrawBox(1550, 830, 1850, 880, GetColor(255, 255, 0), TRUE);
		DrawFormatString(1600, 835, GetColor(0, 0, 0), "TIME   %d", m_actor->countUP);

		// �v���C���[�`��
		m_actor->DrawActor();

		DrawBox(900, 800, 1000, 900, GetColor(0, 0, 0), FALSE);				//�{�b�N�X�̕\��(1�p)
		SetFontSize(100);

		if (m_actor->st > m_actor->MinSt && m_actor->ultLimitFlag == false)
		{
			//�����_���ɖ���\��
			switch (m_actor->randomKeyNumber)
			{
			case 1:		//�����_���ɐ����������� STATE_KEY_UP(1) �Ɠ����Ƃ�
				if (m_actor->inputArrowFlag && m_actor->randomFlag)
				{
					DrawBox(900, 800, 1000, 900, GetColor(255, 255, 255), TRUE);
					DrawRotaGraph(m_starX, m_starY, 1.0, m_starAngle, m_starGraphHandle, TRUE, FALSE);
					m_starX++;
					m_starY--;
					m_starAngle += STAR_ROTA_SPEED;

					if (m_starX >= STAR_END_X)
					{
						m_starX = STAR_FIRST_X;
						m_starY = STAR_FIRST_Y;
					}

					///	�G�t�F�N�g�̍Đ�
					m_effect->PlayEffekseer(m_actor->GetPos());
				}
				else if (!m_actor->randomFlag)
				{
					m_actor->inputArrowFlag = false;
					m_effect->StopEffect();
				}
				else if (CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_RIGHT) || CheckHitKey(KEY_INPUT_LEFT))
				{
					DrawBox(900, 800, 1000, 900, GetColor(255, 0, 0), TRUE);
				}
				DrawFormatString(900, 800, GetColor(0, 0, 0), "��");
				break;

			case 2:		//�����_���ɐ����������� STATE_KEY_DOWN(2) �Ɠ����Ƃ�
				if (m_actor->inputArrowFlag && m_actor->randomFlag)
				{
					DrawBox(900, 800, 1000, 900, GetColor(255, 255, 255), TRUE);
					DrawRotaGraph(m_starX, m_starY, 1.0, m_starAngle, m_starGraphHandle, TRUE, FALSE);
					m_starX++;
					m_starY--;
					m_starAngle += STAR_ROTA_SPEED;

					if (m_starX >= STAR_END_X)
					{
						m_starX = STAR_FIRST_X;
						m_starY = STAR_FIRST_Y;
					}

					//	�G�t�F�N�g�̍Đ�
					m_effect->PlayEffekseer(m_actor->GetPos());
				}
				else if (!m_actor->randomFlag)
				{
					m_actor->inputArrowFlag = false;
					m_effect->StopEffect();
				}
				else if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_RIGHT) || CheckHitKey(KEY_INPUT_LEFT))
				{
					DrawBox(900, 800, 1000, 900, GetColor(255, 0, 0), TRUE);
				}
				DrawFormatString(900, 800, GetColor(0, 0, 0), "��");
				break;

			case 3:		//�����_���ɐ����������� STATE_KEY_RIGHT(3) �Ɠ����Ƃ�
				if (m_actor->inputArrowFlag && m_actor->randomFlag)
				{
					DrawBox(900, 800, 1000, 900, GetColor(255, 255, 255), TRUE);
					DrawRotaGraph(m_starX, m_starY, 1.0, m_starAngle, m_starGraphHandle, TRUE, FALSE);
					m_starX++;
					m_starY--;
					m_starAngle += STAR_ROTA_SPEED;

					if (m_starX >= STAR_END_X)
					{
						m_starX = STAR_FIRST_X;
						m_starY = STAR_FIRST_Y;
					}

					//	�G�t�F�N�g�̍Đ�
					m_effect->PlayEffekseer(m_actor->GetPos());
				}
				else if (!m_actor->randomFlag)
				{
					m_actor->inputArrowFlag = false;
					m_effect->StopEffect();
				}
				else if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_LEFT))
				{
					DrawBox(900, 800, 1000, 900, GetColor(255, 0, 0), TRUE);
				}
				DrawFormatString(900, 800, GetColor(0, 0, 0), "��");
				break;

			case 4:		//�����_���ɐ����������� STATE_KEY_LEFT(4) �Ɠ����Ƃ�
				if (m_actor->inputArrowFlag && m_actor->randomFlag)
				{
					DrawBox(900, 800, 1000, 900, GetColor(255, 255, 255), TRUE);
					DrawRotaGraph(m_starX, m_starY, 1.0, m_starAngle, m_starGraphHandle, TRUE, FALSE);
					m_starX++;
					m_starY--;
					m_starAngle += STAR_ROTA_SPEED;

					if (m_starX >= STAR_END_X)
					{
						m_starX = STAR_FIRST_X;
						m_starY = STAR_FIRST_Y;
					}

					//	�G�t�F�N�g�̍Đ�
					m_effect->PlayEffekseer(m_actor->GetPos());
				}
				else if (!m_actor->randomFlag)
				{
					m_actor->inputArrowFlag = false;
					m_effect->StopEffect();
				}
				else if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_RIGHT))
				{
					DrawBox(900, 800, 1000, 900, GetColor(255, 0, 0), TRUE);
				}
				DrawFormatString(900, 800, GetColor(0, 0, 0), "��");
				break;
			default:
				break;
			}
		}

		if (m_actor->ultFlag)
		{
			switch (m_actor->arrow[0])
			{
			case 1:
				DrawFormatString(900, 800, GetColor(0, 0, 0), "��");
				break;
			case 2:
				DrawFormatString(900, 800, GetColor(0, 0, 0), "��");
				break;
			case 3:
				DrawFormatString(900, 800, GetColor(0, 0, 0), "��");
				break;
			case 4:
				DrawFormatString(900, 800, GetColor(0, 0, 0), "��");
				break;
			}

			switch (m_actor->arrow[1])
			{
			case 1:
				DrawFormatString(1000, 800, GetColor(0, 0, 0), "��");
				break;
			case 2:
				DrawFormatString(1000, 800, GetColor(0, 0, 0), "��");
				break;
			case 3:
				DrawFormatString(1000, 800, GetColor(0, 0, 0), "��");
				break;
			case 4:
				DrawFormatString(1000, 800, GetColor(0, 0, 0), "��");
				break;
			}
			switch (m_actor->arrow[2])
			{
			case 1:
				DrawFormatString(1100, 800, GetColor(0, 0, 0), "��");
				break;
			case 2:
				DrawFormatString(1100, 800, GetColor(0, 0, 0), "��");
				break;
			case 3:
				DrawFormatString(1100, 800, GetColor(0, 0, 0), "��");
				break;
			case 4:
				DrawFormatString(1100, 800, GetColor(0, 0, 0), "��");
				break;
			}
			switch (m_actor->arrow[3])
			{
			case 1:
				DrawFormatString(1200, 800, GetColor(0, 0, 0), "��");
				break;
			case 2:
				DrawFormatString(1200, 800, GetColor(0, 0, 0), "��");
				break;
			case 3:
				DrawFormatString(1200, 800, GetColor(0, 0, 0), "��");
				break;
			case 4:
				DrawFormatString(1200, 800, GetColor(0, 0, 0), "��");
				break;
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

		// ���p���L�[�̕\��
		DrawBox(600, 800, 700, 900, GetColor(0, 0, 0), FALSE);				//�{�b�N�X�̕\��(1�p)
		if (CheckHitKey(KEY_INPUT_C))
		{
			m_actor->inputArrowFlag = true;
			DrawBox(600, 800, 700, 900, GetColor(255, 255, 255), TRUE);
		}
		else if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_RIGHT) || CheckHitKey(KEY_INPUT_LEFT))
		{
			DrawBox(600, 800, 700, 900, GetColor(255, 0, 0), TRUE);
		}
		DrawFormatString(625, 800, GetColor(0, 0, 0), "C");

		//�X�y�[�X�L�[��BOX�`��
		if (-90 >= m_actor->GetPosX() && m_actor->GetPosX() > -136 && m_actor->GetInputSpaceFlag() == false && m_actor->GetTurnFlag() == false)
		{
			//���u�����h���[�h
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);

			if (CheckHitKey(KEY_INPUT_SPACE))
			{
				DrawBox(810, 900, 1100, 1000, GetColor(255, 255, 255), TRUE);
			}
			//�^�[���̕]���� BAD �͈̔�
			if (-90 >= m_actor->GetPosX() && m_actor->GetPosX() > -120)
			{
				DrawBox(810, 900, 1100, 1000, GetColor(255, 0, 0), TRUE);
			}
			//�^�[���̕]���� NORMAL �͈̔�
			if (-120 >= m_actor->GetPosX() && m_actor->GetPosX() > -130)
			{
				DrawBox(810, 900, 1100, 1000, GetColor(255, 255, 0), TRUE);
			}
			//�^�[���̕]���� GOOD �͈̔�
			if (-130 >= m_actor->GetPosX() && m_actor->GetPosX() > -140)
			{
				DrawBox(810, 900, 1100, 1000, GetColor(0, 100, 0), TRUE);
			}

			//�u�����h���[�h���f�t�H���g�ɖ߂�
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

			DrawFormatString(820, 900, GetColor(0, 0, 0), "SPACE");
		}

		SetFontSize(35);
		// �X�^�~�i�Q�[�W�̕\��
		m_actor->DrawSt(m_actor->st, m_actor->MaxSt, m_actor->MinSt);
		// �c�苗���̕\��
		m_actor->DrawToGoal(m_actor->dCount, m_actor->maxdCount);

		// �X�L��
		m_actor->LastSpurt();

		//	�X�^�~�i�������̊��̕\��
		if (m_actor->st <= ORANGE)
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
			SetFontSize(150);
			DrawFormatString(800, 400, GetColor(0, 0, 0), " %d ", m_actor->countDown / 60 + 1);
		}

		//	START!!�̕\��
		if (m_actor->countDown <= 0 && !m_startFinishFlag)
		{
			DrawFormatStringToHandle(700, 400, GetColor(255, 0, 0), startHandle, "START!!");
			m_startCount++;
			if (m_startCount >= 50)
			{
				m_startFinishFlag = true;
			}
		}

		//	GOAL!!�̕\��
		if (m_gameFinishFlag)
		{
			DrawFormatStringToHandle(700, 400, GetColor(255, 0, 0), startHandle, "GOAL!!");
			m_finishCount++;
			if (m_startCount >= 50)
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
		PlaySoundMem(m_bgmSoundHandle, DX_PLAYTYPE_BACK, FALSE);
		ChangeVolumeSoundMem(m_volumePal, m_bgmSoundHandle);
	}
	if (m_gameFinishFlag)
	{
		PlaySoundMem(m_goalSoundHandle, DX_PLAYTYPE_BACK, FALSE);
		ChangeVolumeSoundMem(m_volumePal + SE_VOLUME_PAL, m_goalSoundHandle);
	}
}

void GameScene::Load()
{
	// �O���t�B�b�N�n���h���ɃZ�b�g
	m_tips1GraphHandle = LoadGraph("data/img/Load/TIPS1.png");
	m_tips2GraphHandle = LoadGraph("data/img/Load/TIPS2.png");
	m_tips3GraphHandle = LoadGraph("data/img/Load/TIPS3.png");
	m_boyGraphHandle = LoadGraph("data/img/Load/boy.png");

	//	�G�t�F�N�g����
	m_effect = new PlayEffect("data/effects/water4.efk", 3.0f);

	SetUseASyncLoadFlag(TRUE);

	// �O���t�B�b�N�n���h���ɃZ�b�g
	m_backGraphHandle = LoadGraph("data/img/Game/gameBackTest.png");			//	�w�i
	m_starGraphHandle = LoadGraph("data/img/Game/star.png");					//	��
	m_sweat1GraphHandle = LoadGraph("data/img/Game/Sweat1.png");				//	��1
	m_sweat2GraphHandle = LoadGraph("data/img/Game/Sweat2.png");				//	��2

	//	�T�E���h�n���h���ɃZ�b�g
	m_bgmSoundHandle = LoadSoundMem("data/sound/Game/Game.ogg");				//	BGM
	m_whistleSoundHandle = LoadSoundMem("data/sound/Game/whistle.ogg");			//	�J
	m_countDownSoundHandle = LoadSoundMem("data/sound/Game/Countdown2.ogg");	//	�J�E���g�_�E��
	m_goalSoundHandle = LoadSoundMem("data/sound/Game/goalWhistle.ogg");		//	�S�[��

	// �X�e�[�W�N���X�̃C���X�^���X�𐶐�
	m_stage = new Stage();
	// �A�N�^�[�N���X�ւ̃C���X�^���X�𐶐�
	m_actor = new Yamaoka_PlayerActor;
	// �J�����N���X�ւ̃C���X�^���X�𐶐�
	m_camera = new Camera(*m_actor);
}
