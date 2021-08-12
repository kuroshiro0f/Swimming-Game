//#include "Result.h"
//#include "Title.h"
//
//#include "DxLib.h"
//
//const int SCREEN_SIZE_W = 1920;
//const int SCREEN_SIZE_H = 1080;
//
//const int addAlphaVal = 5;
//
//Result::Result()
//	: m_alphaVal(255)
//	, m_fadeOutFinishFlag(false)
//{
//	if (CheckHitKey(KEY_INPUT_RETURN))
//	{
//		m_checkKeyFlag = TRUE;
//	}
//
//	m_state = RESULT_SCENE_STATE::FADE_IN;
//}
//
//Result::~Result()
//{
//	DeleteGraph(m_backGraphHandle);
//	DeleteSoundMem(m_bgmSoundHandle);
//}
//
//SceneBase* Result::Update(float _deltaTime)
//{
//	switch (m_state)
//	{
//	case Result::RESULT_SCENE_STATE::FADE_IN:
//		break;
//	case Result::RESULT_SCENE_STATE::RESULT:
//		// ���L�[���͏d���΍�̃t���O
//		// ENTER�L�[����w�𗣂�����A����ENTER�̓��͂�L����
//		if (!CheckHitKey(KEY_INPUT_RETURN))
//		{
//			m_checkKeyFlag = false;
//		}
//
//		// ENTER�Ŏ��̃X�e�[�g��
//		if (CheckHitKey(KEY_INPUT_RETURN) && m_checkKeyFlag == false)
//		{
//			// ���L�[���͏d���΍�̃t���O
//			m_checkKeyFlag = true;
//
//			m_state = RESULT_SCENE_STATE::FADE_OUT;
//		}
//
//		break;
//	case Result::RESULT_SCENE_STATE::FADE_OUT:
//		if (m_fadeOutFinishFlag)
//		{
//			return new Title();
//		}
//
//		break;
//	default:
//		break;
//	}
//	return this;
//}
//
//void Result::Draw()
//{
//	// �w�i
//	DrawGraph(0, 0, m_backGraphHandle, TRUE);
//
//	//	�t�F�[�h�C������
//	if (m_state == RESULT_SCENE_STATE::FADE_IN)
//	{
//		// �A���t�@�l�̌��Z
//		m_alphaVal -= addAlphaVal;
//
//		// �A���t�@�u�����h�L����(�����ŃA���t�@�l���Z�b�g)
//		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alphaVal);
//
//		// ��ʑS�̂ɔC�ӂ̃J���[�̎l�p�`��`��
//		DrawBox(0, 0, SCREEN_SIZE_W, SCREEN_SIZE_H, GetColor(0, 0, 0), TRUE);
//
//		// �A���t�@�u�����h������
//		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
//
//		// �A���t�@�l���ő�(255)�ɂȂ�����t�F�[�h�A�E�g�I��
//		if (m_alphaVal <= 0)
//		{
//			m_state = RESULT_SCENE_STATE::RESULT;
//		}
//	}
//
//	// �t�F�[�h�A�E�g����
//	if (m_state == RESULT_SCENE_STATE::FADE_OUT)
//	{
//		// �A���t�@�l�̉��Z
//		m_alphaVal += addAlphaVal;
//		// �A���t�@�u�����h�L����(�����ŃA���t�@�l���Z�b�g)
//		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alphaVal);
//
//		// ��ʑS�̂ɔC�ӂ̃J���[�̎l�p�`��`��
//		DrawBox(0, 0, SCREEN_SIZE_W, SCREEN_SIZE_H, GetColor(0, 0, 0), TRUE);
//
//		// �A���t�@�u�����h������
//		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
//
//		// �A���t�@�l���ő�(255)�ɂȂ�����t�F�[�h�A�E�g�I��
//		if (m_alphaVal >= 255)
//		{
//			m_fadeOutFinishFlag = true;
//		}
//	}
//}
//
//void Result::Sound()
//{
//	PlaySoundMem(m_bgmSoundHandle, DX_PLAYTYPE_BACK, FALSE);
//	ChangeVolumeSoundMem(m_volumePal, m_bgmSoundHandle);
//}
//
//void Result::Load()
//{
//	//	�O���t�B�b�N�n���h���ɃZ�b�g
//	m_backGraphHandle = LoadGraph("data/img/resultBackTest.png");			//	�w�i
//
//	//	�T�E���h�n���h���ɃZ�b�g
//	m_bgmSoundHandle = LoadSoundMem("data/sound/resultBgmTest.mp3");			//	BGM
//}
