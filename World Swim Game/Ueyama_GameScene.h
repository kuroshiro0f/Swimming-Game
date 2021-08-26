#pragma once

#include "SceneBase.h"
#include "Ueyama_PlayerActor.h"
#include "Ueyama_Camera.h"

class Ueyama_PlayerActor;			//PlayerActor�ւ̎Q��
class Ueyama_Camera;				//Camera�ւ̎Q��

class Ueyama_GameScene :public SceneBase
{
	Ueyama_PlayerActor* player;
	Ueyama_Camera* camera;

public:

	enum class GAME_SCENE_STATE
	{
		FADE_IN,
		GAME,
		FADE_OUT
	};

	Ueyama_GameScene();				//	�R���X�g���N�^
	~Ueyama_GameScene();			//	�f�X�g���N�^

	SceneBase* Update(float _deltaTime)override;	//	�X�V
	void Draw()override;			//	�`��
	void Sound()override;			//	���y
	void Load()override;			//	������

private:
	GAME_SCENE_STATE m_state;
	int m_backGraphHandle;			//	�w�i�̃O���t�B�b�N�n���h��
	int m_bgmSoundHandle;			//	�Q�[����ʁE�T�E���h�n���h��

	//	�A���t�@�l
	int m_alphaVal;

	bool m_checkKeyFlag;			//	�L�[�������ꂽ�܂܂��𔻒肷��t���O
	bool m_fadeOutFinishFlag;		//	�t�F�[�h�A�E�g�̏I������t���O

	class Stage* m_stage;   // �X�e�[�W�N���X�ւ̃|�C���^�����o�ϐ�
	class Ueyama_Camera* m_camera;	// �J�����N���X�ւ̃|�C���^�����o�ϐ�
	class Ueyama_PlayerActor* m_actor;   // �A�N�^�[�N���X�ւ̃|�C���^�����o�ϐ�
};