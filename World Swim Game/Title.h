#pragma once
#include "SceneBase.h"

class Title : public SceneBase
{
public:

	enum class TITLE_SCENE_STATE
	{
		FIRST,
		SECOND,
		FADE_IN,
		TITLE,
		MANUAL,
		FADE_OUT
	};

	Title();					//	�R���X�g���N�^
	~Title();					//	�f�X�g���N�^

	SceneBase* Update(float _deltaTime) override;	//	�X�V
	void Draw() override;			//	�`��
	void Sound() override;			//	���y
	void Load() override;			//	������

	void UpdateTransparent();			//	�����̓��ߗʍX�V

private:
	TITLE_SCENE_STATE m_state;

	//	�O���t�B�b�N�n���h��
	int m_backGraphHandle;				//	�w�i
	int m_logoGraphHandle;				//	���S
	int m_startGuideGraphHandle;		//	�X�^�[�g�̈ē�
	int m_manualGuideGraphHandle;		//	�}�j���A���ւ̈ē�
	int m_gateGraphHandle;				//	GATE
	int m_maouGraphHandle;				//	������

	//	�T�E���h�n���h��
	int m_backSoundHandle;				//	BGM
	int m_click_sound_handle;			//  �{�^�����������Ƃ��̉�

	//	�A���t�@�l
	int m_alphaVal;
	int m_addAlphaVal;
	int m_addAlphaVal2;

	// �����̓���
	int m_transpVal;				//	���ߗ�
	int m_fadeTransVal;				//	�t�F�[�h�A�E�g���̓��ߗ�
	int m_permeationAmount;			//	���t���[�����ߗ�

	//	�t���O
	bool m_checkKeyFlag;			//	�L�[�������ꂽ�܂܂��𔻒肷��t���O
	bool m_fadeOutFinishFlag;		//	�t�F�[�h�A�E�g�̏I������t���O
};