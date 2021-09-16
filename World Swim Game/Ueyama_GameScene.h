#pragma once

#include "SceneBase.h"
#include "PlayEffect.h"
#include "Ueyama_PlayerActor.h"
#include "Ueyama_Camera.h"

//-----------------------------------------------------------------------------
//	�ύX�_
//
//-----------------------------------------------------------------------------

class Ueyama_PlayerActor;			//PlayerActor�ւ̎Q��
class Ueyama_Camera;				//Camera�ւ̎Q��

class Ueyama_GameScene :public SceneBase
{
	Ueyama_PlayerActor* player;
	Ueyama_Camera* camera;

public:

	enum class GAME_SCENE_STATE
	{
		LOAD,
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

	//	�����̈ʒu
	int m_mojiX;		//	���[�h������X���W
	int m_mojiY;		//	���[�h������Y���W

	//	�j�̎q�̈ړ�
	int m_boyY;

	//	�����̕\�����Ԃ̑���
	int m_startCount;			//	�X�^�[�g
	int m_finishCount;			//	�S�[��
	bool m_startFinishFlag;

	//	���̈ړ�
	int m_starX;				//	X����
	int m_starY;				//	Y����
	double m_starAngle;			//	�p�x

	//	���̈ړ�
	int m_sweat1X, m_sweat1Y;	//	��1
	int m_sweat2X, m_sweat2Y;	//	��2

	//	�O���t�B�b�N�n���h��
	int m_backGraphHandle;			//	�w�i
	int m_loadGraphHandle;			//	���[�h
	int m_tips1GraphHandle;			//	TIPS1
	int m_tips2GraphHandle;			//	TIPS2
	int m_tips3GraphHandle;			//	TIPS3
	int m_boyGraphHandle;			//	�j�̎q
	int m_starGraphHandle;			//	��
	int m_sweat1GraphHandle;		//	��1
	int m_sweat2GraphHandle;		//	��2

	//	�T�E���h�n���h��
	int m_bgmSoundHandle;			//	BGM
	int m_whistleSoundHandle;		//	�J
	int m_countDownSoundHandle;		//	�J�E���g�_�E��
	int m_goalSoundHandle;			//	�S�[��

	//	���Ԃ̌o��
	int m_timeElapsed;

	//	�^�[���̕]���֘A
	int m_turnEvaX, m_turnEvaY;				//	���W

	//	�A���t�@�l
	int m_alphaVal;

	//	�f���^�^�C��
	float m_deltaTime;

	//	�t���O
	int m_tipsFlag;					//	TIPS�̕\���ς���
	int m_loadingFlag;				//	���[�f�B���O�̕����̕\����ς���
	bool m_checkKeyFlag;			//	�L�[�������ꂽ�܂܂�
	bool m_fadeOutFinishFlag;		//	�t�F�[�h�A�E�g�̏I������
	bool m_loadFlag;				//	���[�h��ʂ�\�����邩
	bool m_loadFinishFlag;			//	���[�h���I�������
	bool m_boyPlusFlag;				//	�j�̎q�����������t���O
	bool m_whistleFinishFlag;		//	�J��炵�I�������
	bool m_gameFinishFlag;			//	�Q�[�����I�������
	bool m_fadeOutFlag;				//	�t�F�[�h�A�E�g���n�߂邩

	class Stage* m_stage;					//	�X�e�[�W�N���X�ւ̃|�C���^�����o�ϐ�
	class Ueyama_Camera* m_camera;			//	�J�����N���X�ւ̃|�C���^�����o�ϐ�
	class Ueyama_PlayerActor* m_actor;		//	�A�N�^�[�N���X�ւ̃|�C���^�����o�ϐ�
	class PlayEffect* m_effect;				//	�G�t�F�N�g�v���[���[
};