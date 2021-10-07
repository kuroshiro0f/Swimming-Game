#pragma once
#include "SceneBase.h"
#include "Volume.h"

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
		MANUAL2,
		MANUAL3,
		FADE_OUT
	};

	Title(bool _roundFlag);					//	�R���X�g���N�^
	~Title();					//	�f�X�g���N�^

	SceneBase* Update(float _deltaTime) override;
	void Draw() override;
	void Sound() override;
	void Load() override;

	void UpdateTransparent();			//	�����̓��ߗʍX�V
	void DlphinMotion();				//	�C���J�̓���

private:
	TITLE_SCENE_STATE m_state;

	//	�O���t�B�b�N�n���h��
	int m_backGraphHandle;				//	�w�i
	int m_logoGraphHandle;				//	���S
	int m_startGuideGraphHandle;		//	�X�^�[�g�̈ē�
	int m_manualGuideGraphHandle;		//	�}�j���A���ւ̈ē�
	int m_arrowGraphHandle;				//	���
	int m_blueDolphinGraphHandle;		//	�̃C���J
	int m_yellowDolphinGraphHandle;		//	���F�̃C���J
	int m_gateGraphHandle;				//	GATE
	int m_maouGraphHandle;				//	������
	int m_manualGraphHandle;			//	�}�j���A��
	int m_manual2GraphHandle;			//	�}�j���A��2�y�[�W��
	int m_manual3GraphHandle;			//	�}�j���A��3�y�[�W��

	//	�T�E���h�n���h��
	int m_backSoundHandle;				//	BGM
	int m_entClickSoundHandle;			//  ENT�L�[���������Ƃ��̉�
	int m_spaClickSoundHandle;			//	SPACE�L�[���������Ƃ��̉�

	//	�A���t�@�l
	int m_alphaVal;
	int m_addAlphaVal;
	int m_addAlphaVal2;

	////	���H�̓���
	//double m_bigDropAngle;				//	�傫�Ȑ��H
	//double m_smallDropAngle;			//	�����Ȑ��H

	// �����̓���
	int m_transpVal;				//	���ߗ�
	int m_fadeTransVal;				//	�t�F�[�h�A�E�g���̓��ߗ�
	int m_permeationAmount;			//	���t���[�����ߗ�

	//	GATE�Ɩ������̓���
	int m_transpVal2;				//	���ߗ�
	int m_displayCount;				//	�\������
	bool m_transpDownFlag;			//	�t�F�[�h�C�����t�F�[�h�A�E�g��
	bool m_titleFlag;				//	�^�C�g����ʂɍs����

	//	���[�h�I��
	int m_cursolNum;				//	�J�[�\���ԍ�
	int m_arrowY;					//	����Y���W

	//	�C���J�̈ړ�
	int m_blueY;					//	�C���J		
	int m_yellowY;					//	���C���J
	bool m_blueUpFlag;				//	�C���J���オ�邩
	bool m_yellowUpFlag;			//	���C���J���オ�邩

	//	�f���^�^�C��
	//float m_deltaTime;

	//	�t���O
	bool m_checkKeyFlag;			//	�L�[�������ꂽ�܂܂��𔻒肷��t���O
	bool m_fadeOutFinishFlag;		//	�t�F�[�h�A�E�g�̏I������t���O

	class Volume* m_volume;
};