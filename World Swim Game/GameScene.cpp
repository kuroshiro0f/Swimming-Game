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

//	���[�h����t�@�C����
const int LOAD_FAILE_NUM = 5;

//	TIPS�̐�
const int TIPS_NUM = 3;

//	LOADING�̕����̐؂�ւ�鑬��
const int LOADING_SPEED = 60;

//	�j�̎q�̈ړ��͈�
const int BOY_MIN_Y = -50;

GameScene::GameScene()
	: m_alphaVal(255)
	, m_fadeOutFinishFlag(false)
	, m_loadFlag(true)
	, m_loadFinishFlag(false)
	, m_tipsFlag(GetRand(TIPS_NUM - 1))
	, m_loadingFlag(0)
	, m_boyPlusFlag(false)
	, m_stage(nullptr)
	, m_camera(nullptr)
	, m_actor(nullptr)
	, m_mojiX(1450)
	, m_mojiY(1000)
	, m_boyY(0)
{
	// �X�e�[�g�Z�b�g(�t�F�[�h�C������)
	m_state = GAME_SCENE_STATE::LOAD;
}

GameScene::~GameScene()
{
	StopSoundMem(m_bgmSoundHandle);
	DeleteGraph(m_backGraphHandle);
	DeleteGraph(m_tips1GraphHandle);
	DeleteGraph(m_tips2GraphHandle);
	DeleteGraph(m_tips3GraphHandle);
	DeleteGraph(m_boyGraphHandle);

	delete m_stage;   // �X�e�[�W�̃|�C���^�����o�ϐ�������
	delete m_camera;  // �J�����̃|�C���^�����o�ϐ�������
	delete m_actor;   // �A�N�^�[�̃|�C���^�����o�ϐ����폜
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
		m_actor->countDown--;

		// ���L�[���͏d���΍�̃t���O
		// ENTER�L�[����w�𗣂�����A����ENTER�̓��͂�L����
		if (!CheckHitKey(KEY_INPUT_RETURN))
		{
			m_checkKeyFlag = false;
		}

		////�[�܂ōs���Ǝ��̃X�e�[�g��
		//if (m_actor->GetPosX() <= -136)
		//{
		//	m_state = GAME_SCENE_STATE::FADE_OUT;
		//}

		//�[�܂Ŗ߂��Ă��Ă��AturnFlag �� true �Ȃ玟�̃X�e�[�g��
		if (m_actor->GetPosX() >= 130 && m_actor->GetTurnFlag() == true)
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
			case 1:
				DrawGraph(0, 0, m_tips2GraphHandle, TRUE);
			case 2:
				DrawGraph(0, 0, m_tips3GraphHandle, TRUE);
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

		//// ����{�^���i���j
		//if (CheckHitKey(KEY_INPUT_RIGHT))
		//{
		//	DrawBox(1050, 600, 1150, 700, GetColor(255, 255, 255), TRUE);
		//}
		//if (CheckHitKey(KEY_INPUT_LEFT))
		//{
		//	DrawBox(750, 600, 850, 700, GetColor(255, 255, 255), TRUE);
		//}
		//DrawBox(750, 600, 850, 700, GetColor(0, 0, 0), FALSE);
		//DrawBox(1050, 600, 1150, 700, GetColor(0, 0, 0), FALSE);
		//SetFontSize(100);
		//DrawFormatString(750, 600, GetColor(0, 0, 0), "��");
		//DrawFormatString(1050, 600, GetColor(0, 0, 0), "��");

		DrawBox(900, 800, 1000, 900, GetColor(0, 0, 0), FALSE);				//�{�b�N�X�̕\��(1�p)
		SetFontSize(100);

		//�����_���ɖ���\��
		if (m_actor->randomKeyNumber == 1)		//�����_���ɐ����������� STATE_KEY_UP(1) �Ɠ����Ƃ�
		{
			if (CheckHitKey(KEY_INPUT_UP))
			{
				DrawBox(900, 800, 1000, 900, GetColor(255, 255, 255), TRUE);
			}
			else if (CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_RIGHT) || CheckHitKey(KEY_INPUT_LEFT))
			{
				DrawBox(900, 800, 1000, 900, GetColor(255, 0, 0), TRUE);
			}
			DrawFormatString(900, 800, GetColor(0, 0, 0), "��");
		}
		if (m_actor->randomKeyNumber == 2)		//�����_���ɐ����������� STATE_KEY_DOWN(1) �Ɠ����Ƃ�
		{
			if (CheckHitKey(KEY_INPUT_DOWN))
			{
				DrawBox(900, 800, 1000, 900, GetColor(255, 255, 255), TRUE);
			}
			else if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_RIGHT) || CheckHitKey(KEY_INPUT_LEFT))
			{
				DrawBox(900, 800, 1000, 900, GetColor(255, 0, 0), TRUE);
			}
			DrawFormatString(900, 800, GetColor(0, 0, 0), "��");
		}
		if (m_actor->randomKeyNumber == 3)		//�����_���ɐ����������� STATE_KEY_LEFT(1) �Ɠ����Ƃ�
		{
			if (CheckHitKey(KEY_INPUT_RIGHT))
			{
				DrawBox(900, 800, 1000, 900, GetColor(255, 255, 255), TRUE);
			}
			else if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_LEFT))
			{
				DrawBox(900, 800, 1000, 900, GetColor(255, 0, 0), TRUE);
			}
			DrawFormatString(900, 800, GetColor(0, 0, 0), "��");
		}
		if (m_actor->randomKeyNumber == 4)		//�����_���ɐ����������� STATE_KEY_RIGHT(1) �Ɠ����Ƃ�
		{
			if (CheckHitKey(KEY_INPUT_LEFT))
			{
				DrawBox(900, 800, 1000, 900, GetColor(255, 255, 255), TRUE);
			}
			else if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_RIGHT))
			{
				DrawBox(900, 800, 1000, 900, GetColor(255, 0, 0), TRUE);
			}
			DrawFormatString(900, 800, GetColor(0, 0, 0), "��");
		}

		//�X�y�[�X�L�[��BOX�`��
		if (-90 >= m_actor->GetPosX() && m_actor->GetPosX() > -136 && m_actor->GetTurnFlag() == false)
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
		m_actor->Skill(m_actor->dCount, m_actor->maxdCount);

		// �J�E���g�_�E���̕\��
		if (m_actor->countDown >= 0)
		{
			SetFontSize(150);
			DrawFormatString(800, 400, GetColor(0, 0, 0), " %d ", m_actor->countDown / 60 + 1);
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
	if (m_state == GAME_SCENE_STATE::GAME)
	{
		PlaySoundMem(m_bgmSoundHandle, DX_PLAYTYPE_BACK, FALSE);
		ChangeVolumeSoundMem(m_volumePal, m_bgmSoundHandle);
	}
}

void GameScene::Load()
{
	// �O���t�B�b�N�n���h���ɃZ�b�g
	m_tips1GraphHandle = LoadGraph("data/img/Load/TIPS1.png");
	m_tips2GraphHandle = LoadGraph("data/img/Load/TIPS2.png");
	m_tips3GraphHandle = LoadGraph("data/img/Load/TIPS3.png");
	m_boyGraphHandle = LoadGraph("data/img/Load/boy.png");

	SetUseASyncLoadFlag(TRUE);

	// �O���t�B�b�N�n���h���ɃZ�b�g
	m_backGraphHandle = LoadGraph("data/img/Game/gameBackTest.png");		//	�w�i

	//	�T�E���h�n���h���ɃZ�b�g
	m_bgmSoundHandle = LoadSoundMem("data/sound/Game/Game.ogg");			//	BGM

	// �X�e�[�W�N���X�̃C���X�^���X�𐶐�
	m_stage = new Stage();
	// �A�N�^�[�N���X�ւ̃C���X�^���X�𐶐�
	m_actor = new PlayerActor;
	// �J�����N���X�ւ̃C���X�^���X�𐶐�
	m_camera = new Camera(*m_actor);
}
