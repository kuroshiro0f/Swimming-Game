#pragma once

#include "Ueyama_SceneBase.h"
#include "Nagatomo_PlayerActor.h"
#include "Nagatomo_Camera.h"

class Nagatomo_PlayerActor;			//PlayerActor�ւ̎Q��
class Nagatomo_Camera;				//Camera�ւ̎Q��

class Ueyama_GameScene :public Ueyama_SceneBase
{
	Nagatomo_PlayerActor* player;
	Nagatomo_Camera* camera;

public:

	enum class GAME_SCENE_STATE
	{
		FADE_IN,
		GAME,
		FADE_OUT
	};

	Ueyama_GameScene();				//	�R���X�g���N�^
	~Ueyama_GameScene();			//	�f�X�g���N�^

	Ueyama_SceneBase* Update(float _deltaTime)override;	//	�X�V
	void Draw()override;			//	�`��
	void Sound()override;			//	���y
	void Load()override;			//	������

private:
	GAME_SCENE_STATE m_state;
	int m_backGraphHandle;			//	�w�i�̃O���t�B�b�N�n���h��
	int m_bgmSoundHandle;				//	�Q�[����ʁE�T�E���h�n���h��

	//	�A���t�@�l
	int m_alphaVal;

	bool m_checkKeyFlag;			//	�L�[�������ꂽ�܂܂��𔻒肷��t���O
	bool m_fadeOutFinishFlag;		//	�t�F�[�h�A�E�g�̏I������t���O
};