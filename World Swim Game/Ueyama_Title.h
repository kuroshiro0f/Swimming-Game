#pragma once
#include "SceneBase.h"

//-----------------------------------------------------------------------------
//	�ύX�_
//	���H�֘A
//	�}�j���A���֘A
//-----------------------------------------------------------------------------


class Ueyama_Title : public SceneBase
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

	Ueyama_Title();						//	�R���X�g���N�^
	~Ueyama_Title();					//	�f�X�g���N�^

	SceneBase* Update(float _deltaTime) override;
	void Draw() override;
	void Sound() override;
	void Load() override;

	void UpdateTransparent();			//	�����̓��ߗʍX�V

private:
	TITLE_SCENE_STATE m_state;

	//	���f���n���h��
	int m_swimModelHandle;				//	�j���L����
	int m_crawlModelHandle;				//	�N���[���̃��[�V����

	//	�O���t�B�b�N�n���h��
	int m_backGraphHandle;				//	�w�i
	int m_logoGraphHandle;				//	���S
	int m_startGuideGraphHandle;		//	�X�^�[�g�̈ē�
	int m_manualGuideGraphHandle;		//	�}�j���A���ւ̈ē�
	int m_gateGraphHandle;				//	GATE
	int m_maouGraphHandle;				//	������
	int m_bigDropGraphHandle;			//	�傫�Ȑ��H
	int m_smallDropGraphHandle;			//	�����Ȑ��H
	int m_manualGraphHandle;			//	�}�j���A��

	//	�T�E���h�n���h��
	int m_backSoundHandle;				//	BGM
	int m_entClickSoundHandle;			//  ENT�L�[���������Ƃ��̉�
	int m_spaClickSoundHandle;			//	SPACE�L�[���������Ƃ��̉�

	//	�A���t�@�l
	int m_alphaVal;
	float m_addAlphaVal;
	float m_addAlphaVal2;

	//	���H�̓���
	double m_bigDropAngle;				//	�傫�Ȑ��H
	double m_smallDropAngle;			//	�����Ȑ��H

	//	�����̓���
	int m_transpVal;				//	���ߗ�
	int m_fadeTransVal;				//	�t�F�[�h�A�E�g���̓��ߗ�
	int m_permeationAmount;			//	���t���[�����ߗ�

	//	�f���^�^�C��
	float m_deltaTime;

	//	�t���O
	int m_bigDropFlag;				//	�傫�Ȑ��H�̏�ԊǗ�
	int m_smallDropFlag;			//	�����Ȑ��H�̏�ԊǗ�
	bool m_checkKeyFlag;			//	�L�[�������ꂽ�܂܂��𔻒肷��t���O
	bool m_fadeOutFinishFlag;		//	�t�F�[�h�A�E�g�̏I������t���O
};