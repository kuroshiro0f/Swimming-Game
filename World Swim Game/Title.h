//#pragma once
//#include "SceneBase.h"
//
//class Title : public SceneBase
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
//	Title();					//	�R���X�g���N�^
//	~Title();					//	�f�X�g���N�^
//
//	SceneBase* Update(float _deltaTime) override;
//	void Draw() override;
//	void Sound() override;
//	void Load() override;
//
//private:
//	TITLE_SCENE_STATE m_state;
//
//	int m_backGraphHandle;				//	�^�C�g����ʁE�w�i�̃O���t�B�b�N�n���h��
//	int m_soundHandle;					//	�^�C�g����ʁE�T�E���h�n���h��
//	int m_click_sound_handle;			//  ENTER�Ői�ނƂ��̃T�E���h�n���h��
//
//	int m_permeationAmount;				// ���t���[�����ߗ�
//
//	//	�A���t�@�l
//	int m_alphaVal;
//
//	// ���ߗ�
//	int m_transpVal;
//	int m_fadeTransVal;				//	�t�F�[�h�A�E�g��
//
//	bool m_checkKeyFlag;			//	�L�[�������ꂽ�܂܂��𔻒肷��t���O
//	bool m_fadeOutFinishFlag;		//	�t�F�[�h�A�E�g�̏I������t���O
//};