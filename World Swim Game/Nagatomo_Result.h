#pragma once
#include "SceneBase.h"
#include "Nagatomo_GameScene.h"

class GameScene;

#pragma once

#include "SceneBase.h"

class Result : public SceneBase
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

	Result(const int _time);			//	�R���X�g���N�^
	~Result();			//�@�f�X�g���N�^

	SceneBase* Update(float _deltaTime)override;	//	�X�V
	void Draw()override;			//	�`��
	void Sound()override;			//	���y
	void Load()override;			//	������

private:
	RESULT_SCENE_STATE m_state;

	//	�A���t�@�l
	int m_alphaVal;

	//	�O���t�B�b�N�n���h��
	int m_time;							//�@�^�C��
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