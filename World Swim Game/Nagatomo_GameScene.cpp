#include "Nagatomo_GameScene.h"

#include "Nagatomo_Result.h"
#include "Stage.h"
#include "Nagatomo_PlayerActor.h"

#include "DxLib.h"

//	�X�N���[���̃T�C�Y
const int SCREEN_SIZE_W = 1920;
const int SCREEN_SIZE_H = 1080;

// �t�F�[�h�C���E�t�F�[�h�A�E�g�̑��x
const int addAlphaVal = 5;

GameScene::GameScene()
	: m_alphaVal(255)
	, m_fadeOutFinishFlag(false)
	, m_stage(nullptr)
	, m_camera(nullptr)
	, m_actor(nullptr)
{
	// �X�e�[�g�Z�b�g(�t�F�[�h�C������)
	m_state = GAME_SCENE_STATE::FADE_IN;

}

GameScene::~GameScene()
{
	StopSoundMem(m_bgmSoundHandle);
	DeleteGraph(m_backGraphHandle);

	delete m_stage;   // �X�e�[�W�̃|�C���^�����o�ϐ�������
	delete m_camera;  // �J�����̃|�C���^�����o�ϐ�������
	delete m_actor;   // �A�N�^�[�̃|�C���^�����o�ϐ����폜
}

SceneBase* GameScene::Update(float _deltaTime)
{
	m_camera->Update(*m_actor);

	switch (m_state)
	{
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

		// ENTER�Ŏ��̃X�e�[�g��
		if (CheckHitKey(KEY_INPUT_RETURN) && m_checkKeyFlag == false)
		{
			// ���L�[���͏d���΍�̃t���O
			m_checkKeyFlag = true;

			m_state = GAME_SCENE_STATE::FADE_OUT;
		}

		//�[�܂Ŗ߂��Ă��Ă��AturnFlag �� true �Ȃ玟�̃X�e�[�g��
		if (m_actor->GetPosX() >= 130 && m_actor->GetTurnFlag() == true)
		{
			m_state = GAME_SCENE_STATE::FADE_OUT;
		}

		break;
	case GAME_SCENE_STATE::FADE_OUT:
		if (m_fadeOutFinishFlag)
		{
			return new Result(m_actor->countUP);				//	���U���g�V�[���ɐ؂�ւ���
		}

		break;
	default:
		break;
	}
	return this;
}

void GameScene::Draw()
{
	//	�w�i
	DrawGraph(0, 0, m_backGraphHandle, TRUE);

	// �X�e�[�W�̕`��
	m_stage->Draw();
	SetFontSize(40);

	//	�^�C���̕\��
	DrawBox(1550, 830, 1850, 880, GetColor(0, 255, 255), TRUE);
	DrawFormatString(1600, 835, GetColor(0, 0, 0), "TIME   %d", m_actor->countUP);

	// �v���C���[�`��
	m_actor->DrawActor();

	// ����{�^���i���j
	/*if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		DrawBox(1050, 800, 1150, 900, GetColor(255, 255, 255), TRUE);
	}
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		DrawBox(750, 800, 850, 900, GetColor(255, 255, 255), TRUE);
	}
	DrawBox(750, 800, 850, 900, GetColor(0, 0, 0), FALSE);				//�{�b�N�X�̕\��(2�p)
	DrawBox(1050, 800, 1150, 900, GetColor(0, 0, 0), FALSE);			//�{�b�N�X�̕\��(2�p)
	*/
	DrawBox(900, 800, 1000, 900, GetColor(0, 0, 0), FALSE);				//�{�b�N�X�̕\��(1�p)
	SetFontSize(100);

	//DrawFormatString(750, 800, GetColor(0, 0, 0), "��");
	//DrawFormatString(1050, 800, GetColor(0, 0, 0), "��");

	//�����_���ɖ���\��
	switch (m_actor->randomKeyNumber)
	{
	case 1:		//�����_���ɐ����������� STATE_KEY_UP(1) �Ɠ����Ƃ�
		if (CheckHitKey(KEY_INPUT_UP))
		{
			m_actor->inputArrowFlag = true;
		}

		if (m_actor->inputArrowFlag && m_actor->randomFlag)
		{
			DrawBox(900, 800, 1000, 900, GetColor(255, 255, 255), TRUE);
		}
		else if (!m_actor->randomFlag)
		{
			m_actor->inputArrowFlag = false;
		}
		else if (CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_RIGHT) || CheckHitKey(KEY_INPUT_LEFT))
		{
			DrawBox(900, 800, 1000, 900, GetColor(255, 0, 0), TRUE);
		}
		DrawFormatString(900, 800, GetColor(0, 0, 0), "��");
		break;

	case 2:		//�����_���ɐ����������� STATE_KEY_DOWN(2) �Ɠ����Ƃ�
		if (CheckHitKey(KEY_INPUT_DOWN))
		{
			m_actor->inputArrowFlag = true;
		}

		if (m_actor->inputArrowFlag && m_actor->randomFlag)
		{
			DrawBox(900, 800, 1000, 900, GetColor(255, 255, 255), TRUE);
		}
		else if (!m_actor->randomFlag)
		{
			m_actor->inputArrowFlag = false;
		}
		else if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_RIGHT) || CheckHitKey(KEY_INPUT_LEFT))
		{
			DrawBox(900, 800, 1000, 900, GetColor(255, 0, 0), TRUE);
		}
		DrawFormatString(900, 800, GetColor(0, 0, 0), "��");
		break;

	case 3:		//�����_���ɐ����������� STATE_KEY_RIGHT(3) �Ɠ����Ƃ�
		if (CheckHitKey(KEY_INPUT_RIGHT))
		{
			m_actor->inputArrowFlag = true;
		}

		if (m_actor->inputArrowFlag && m_actor->randomFlag)
		{
			DrawBox(900, 800, 1000, 900, GetColor(255, 255, 255), TRUE);
		}
		else if (!m_actor->randomFlag)
		{
			m_actor->inputArrowFlag = false;
		}
		else if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_LEFT))
		{
			DrawBox(900, 800, 1000, 900, GetColor(255, 0, 0), TRUE);
		}
		DrawFormatString(900, 800, GetColor(0, 0, 0), "��");
		break;

	case 4:		//�����_���ɐ����������� STATE_KEY_LEFT(4) �Ɠ����Ƃ�
		if (CheckHitKey(KEY_INPUT_LEFT))
		{
			m_actor->inputArrowFlag = true;
		}

		if (m_actor->inputArrowFlag && m_actor->randomFlag)
		{
			DrawBox(900, 800, 1000, 900, GetColor(255, 255, 255), TRUE);
		}
		else if (!m_actor->randomFlag)
		{
			m_actor->inputArrowFlag = false;
		}
		else if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_RIGHT))
		{
			DrawBox(900, 800, 1000, 900, GetColor(255, 0, 0), TRUE);
		}
		DrawFormatString(900, 800, GetColor(0, 0, 0), "��");
	}

	//�X�y�[�X�L�[��BOX�`��
	if (-90 >= m_actor->GetPosX() && m_actor->GetPosX() > -136 && m_actor->GetInputSpaceFlag() == false)
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
	m_actor->DrawToGoal(m_actor->dCount);

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
	m_backGraphHandle = LoadGraph("data/img/Game/gameBackTest.png");		//	�w�i

	//	�T�E���h�n���h���ɃZ�b�g
	m_bgmSoundHandle = LoadSoundMem("data/sound/Game/gameBgmTest.mp3");			//	BGM

	// �X�e�[�W�N���X�̃C���X�^���X�𐶐�
	m_stage = new Stage;
	// �A�N�^�[�N���X�ւ̃C���X�^���X�𐶐�
	m_actor = new Nagatomo_PlayerActor;
	// �J�����N���X�ւ̃C���X�^���X�𐶐�
	m_camera = new Camera(*m_actor);
}
