#pragma once

#include "SceneBase.h"

class Ueyama_Result : public SceneBase
{
public:

	enum class RESULT_SCENE_STATE
	{
		FADE_IN,
		RESULT,
		TIME,
		MEDAL,
		ENTER,
		FADE_OUT
	};

	Ueyama_Result(const int _time);			//	�R���X�g���N�^
	~Ueyama_Result();			//�@�f�X�g���N�^

	SceneBase* Update(float _deltaTime)override;	//	�X�V
	void Draw()override;			//	�`��
	void Sound()override;			//	���y
	void Load()override;			//	������

private:
	RESULT_SCENE_STATE m_state;

	//	�A���t�@�l
	int m_alphaVal;

	//	�N���A�^�C��
	int m_time;

	//	�O���t�B�b�N�n���h��
	int m_backGraphHandle;				//	�w�i
	int m_logoGraphHandle;				//	���S
	int m_evaluationGraphHandle;		//	�]��
	int m_guidanceGraphHandle;			//	���̃V�[���ւ̈ē�
	int m_medalGraphHandle;				//	���_��

	//	�T�E���h�n���h��
	int m_bgmSoundHandle;				//	BGM

	//	�t���O
	bool m_checkKeyFlag;				//	�L�[�������ꂽ�܂܂�
	bool m_fadeOutFinishFlag;			//	�t�F�[�h�A�E�g�I��
};