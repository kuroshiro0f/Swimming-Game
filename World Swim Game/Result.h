#pragma once

#include "SceneBase.h"

class Result : public SceneBase
{
public:

	enum class RESULT_SCENE_STATE
	{
		FADE_IN,
		RESULT,
		FADE_OUT
	};

	Result();			//	�R���X�g���N�^
	~Result();			//�@�f�X�g���N�^

	SceneBase* Update(float _deltaTime)override;	//	�X�V
	void Draw()override;			//	�`��
	void Sound()override;			//	���y
	void Load()override;			//	������

private:
	RESULT_SCENE_STATE m_state;

	//	�A���t�@�l
	int m_alphaVal;

	int m_backGraphHandle;				//	�w�i�̃O���t�B�b�N�n���h��
	int m_bgmSoundHandle;				//	BGM�̃T�E���h�n���h��
	bool m_checkKeyFlag;				//	�L�[�������ꂽ�܂܂��𔻒肷��t���O
	bool m_fadeOutFinishFlag;			//	�t�F�[�h�A�E�g�̏I������t���O
};