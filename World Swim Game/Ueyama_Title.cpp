#include "Ueyama_Title.h"
#include "Ueyama_GameScene.h"
#include "Yamaoka_GameScene.h"

#include "DxLib.h"

//	�E�C���h�E�̃T�C�Y
const int SCREEN_SIZE_W = 1920;
const int SCREEN_SIZE_H = 1080;

//	�t�F�[�h�C���E�t�F�[�h�A�E�g�̑��x
const int addAlphaVal = 5;


Ueyama_Title::Ueyama_Title()
	: m_state(TITLE_SCENE_STATE::TITLE)
	, m_alphaVal(0)
	, m_fadeOutFinishFlag(false)
{
	if (CheckHitKey(KEY_INPUT_RETURN))
	{
		m_checkKeyFlag = TRUE;
	}

	SetFontSize(m_normalFontSize);
}

Ueyama_Title::~Ueyama_Title()
{
	StopSoundMem(m_backSoundHandle);

	DeleteGraph(m_backGraphHandle);
	DeleteSoundMem(m_backSoundHandle);
}

SceneBase* Ueyama_Title::Update(float _deltaTime)
{
	// �X�e�[�g�����g���Ƃɏ�����ύX
	switch (m_state)
	{
	case TITLE_SCENE_STATE::FADE_IN:
		break;
	case TITLE_SCENE_STATE::TITLE:
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

			m_state = TITLE_SCENE_STATE::TITLE;
		}

		break;
	case TITLE_SCENE_STATE::FADE_OUT:
		if (m_fadeOutFinishFlag)
		{
			//return new Ueyama_GameScene();
			return new Yamaoka_GameScene();
		}
		break;
	default:
		break;
	}
	return this;
}

void Ueyama_Title::Draw()
{
	// �`��
	DrawGraph(0, 0, m_backGraphHandle, TRUE);			//	�w�i
	DrawGraph(0, 0, m_logoGraphHandle, TRUE);			//	���S
	DrawGraph(0, 0, m_startGuideGraphHandle, TRUE);		//	�X�^�[�g�̈ē�
	DrawGraph(0, 0, m_manualGuideGraphHandle, TRUE);	//	�}�j���A���ւ̈ē�


	//// ��ʂɉf��ʒu�ɂR�c���f�����ړ�
	//MV1SetPosition(m_swimModelHandle, VGet(320.0f, -300.0f, 600.0f));


	//// 3D���f���̃X�P�[�����g��
	//MV1SetScale(m_swimModelHandle, VGet(50.0f, 50.0f, 50.0f));

	////	�N���[���̃��f����`��
	//MV1DrawModel(m_swimModelHandle);


	if (m_state == TITLE_SCENE_STATE::MANUAL)
	{
		// ���߂��ĕ`��
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 190);
		DrawBox(0, 0, SCREEN_SIZE_W, SCREEN_SIZE_H, GetColor(0, 0, 0), TRUE);
		// ���߂����ɖ߂�
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	//	�t�F�[�h�C������
	if (m_state == TITLE_SCENE_STATE::FADE_IN)
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
			m_state = TITLE_SCENE_STATE::TITLE;
		}
	}

	// �t�F�[�h�A�E�g����
	if (m_state == TITLE_SCENE_STATE::FADE_OUT)
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

void Ueyama_Title::Sound()
{
	//	BGM�𗬂�
	PlaySoundMem(m_backSoundHandle, DX_PLAYTYPE_BACK, FALSE);
	ChangeVolumeSoundMem(m_volumePal, m_backSoundHandle);
}

void Ueyama_Title::Load()
{
	//	�O���t�B�b�N�n���h���ɃZ�b�g
	m_backGraphHandle = LoadGraph("data/img/Title/Title_back.png");				//	�w�i
	m_logoGraphHandle = LoadGraph("data/img/Title/Title_logo.png");				//	���S
	m_startGuideGraphHandle = LoadGraph("data/img/Title/Title_start.png");		//	�X�^�[�g�̈ē�
	m_manualGuideGraphHandle = LoadGraph("data/img/Title/Title_manual.png");	//	�}�j���A���ւ̈ē�

	//	���f���n���h���ɃZ�b�g
	m_swimModelHandle = MV1LoadModel("data/asset/swim/natu12b.pmx");			//	�j���L����
	m_crawlModelHandle = MV1LoadModel("data/asset/�N���[��.vmd");				//	�N���[��

	// �A�j���[�V�������A�^�b�`
	MV1AttachAnim(m_swimModelHandle,0,m_crawlModelHandle,FALSE);				//	�N���[��

	//	�T�E���h�n���h���ɃZ�b�g
	m_backSoundHandle = LoadSoundMem("data/sound/Title/titleBgmTest.mp3");		//	BGM
}
