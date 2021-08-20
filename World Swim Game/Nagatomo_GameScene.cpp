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
	, m_startTime(0)
	, m_tmpTime(0)
	, m_countUP(0)
{
	// �J�n���̎��Ԃ��擾
	m_startTime = GetNowCount() / 1000;

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
		break;
	case GAME_SCENE_STATE::GAME:
		m_stage->Update();
		m_actor->Update(_deltaTime);
		m_actor->UpdateActor(_deltaTime);

		//���ݎ������擾
		m_tmpTime = GetNowCount() / 1000;
		// �o�ߎ��Ԃ��v�Z  (-�Z �͉��}���u)
		m_countUP = (m_tmpTime - m_startTime)-5;

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

		//�[�܂ōs���Ǝ��̃X�e�[�g��
		if (m_actor->GetPosX() <= -136)
		{
			m_state = GAME_SCENE_STATE::FADE_OUT;
		}

		break;
	case GAME_SCENE_STATE::FADE_OUT:
		if (m_fadeOutFinishFlag)
		{
			return new Result(m_countUP);				//	���U���g�V�[���ɐ؂�ւ���
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
	// 
	DrawBox(1450, 800, 1750, 890, GetColor(0, 255, 255), TRUE);
	DrawFormatString(1500, 800, GetColor(0, 0, 0), "TIME   %d", m_countUP);

	// �v���C���[�`��
	m_actor->DrawActor();

	// ����{�^���i���j �v���C���[�̎�̎΂ߏ�ɕ\��
	DrawBox(750, 600, 850, 700, GetColor(0, 0, 0), FALSE);
	DrawBox(1050, 600, 1150, 700, GetColor(0, 0, 0), FALSE);
	SetFontSize(100);
	DrawFormatString(770, 600, GetColor(0, 0, 0), "��");
	DrawFormatString(1070, 600, GetColor(0, 0, 0), "��");

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

		//�v���C���[
		m_actor->DrawActor();

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
