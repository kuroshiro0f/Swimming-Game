//#pragma once
//#include "SceneBase.h"
//
//class Ueyama_Title : public SceneBase
//{
//public:
//
//	enum class TITLE_SCENE_STATE
//	{
//		FADE_IN,
//		TITLE,
//		MANUAL,
//		FADE_OUT
//	};
//
//	Ueyama_Title();						//	�R���X�g���N�^
//	~Ueyama_Title();					//	�f�X�g���N�^
//
//	SceneBase* Update(float _deltaTime) override;
//	void Draw() override;
//	void Sound() override;
//	void Load() override;
//
//private:
//	TITLE_SCENE_STATE m_state;
//
//	//	���f���n���h��
//	int m_swimModelHandle;				//	�j���L����
//	int m_crawlModelHandle;				//	�N���[���̃��[�V����
//
//	//	�O���t�B�b�N�n���h��
//	int m_backGraphHandle;				//	�w�i
//	int m_logoGraphHandle;				//	���S
//	int m_startGuideGraphHandle;		//	�X�^�[�g�̈ē�
//	int m_manualGuideGraphHandle;		//	�}�j���A���ւ̈ē�
//
//	//	�T�E���h�n���h��
//	int m_backSoundHandle;				//	BGM
//	int m_click_sound_handle;			//  �{�^�����������Ƃ��̉�
//
//	//	�A���t�@�l
//	int m_alphaVal;
//
//	bool m_checkKeyFlag;			//	�L�[�������ꂽ�܂܂��𔻒肷��t���O
//	bool m_fadeOutFinishFlag;		//	�t�F�[�h�A�E�g�̏I������t���O
//};