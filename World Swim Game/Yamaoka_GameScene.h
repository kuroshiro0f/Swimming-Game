#pragma once
#include "SceneBase.h"
#include "Yamaoka_PlayerActor.h"
#include "Yamaoka_Camera.h"

class Yamaoka_PlayerActor;			//PlayerActor�ւ̎Q��
class Camera;				//Camera�ւ̎Q��

class Yamaoka_GameScene :public SceneBase
{
	Yamaoka_PlayerActor* player;
	Camera* camera;

public:
	//int m_countUP;                  // �o�ߎ���

	enum class GAME_SCENE_STATE
	{
		LOAD,
		FADE_IN,
		GAME,
		FADE_OUT
	};

	Yamaoka_GameScene();			//	�R���X�g���N�^
	~Yamaoka_GameScene();			//	�f�X�g���N�^

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

	//	�O���t�B�b�N�n���h��
	int m_backGraphHandle;			//	�w�i
	int m_loadGraphHandle;			//	���[�h
	int m_tips1GraphHandle;			//	TIPS1
	int m_tips2GraphHandle;			//	TIPS2
	int m_tips3GraphHandle;			//	TIPS3
	int m_boyGraphHandle;			//	�j�̎q

	//	�T�E���h�n���h��
	int m_bgmSoundHandle;			//	BGM
	int m_whistleSoundHandle;		//	�J
	int m_countDownSoundHandle;		//	�J�E���g�_�E��
	int m_goalSoundHandle;			//	�S�[��

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

	class Stage* m_stage;   // �X�e�[�W�N���X�ւ̃|�C���^�����o�ϐ�
	class Camera* m_camera;	// �J�����N���X�ւ̃|�C���^�����o�ϐ�
	class Yamaoka_PlayerActor* m_actor;   // �A�N�^�[�N���X�ւ̃|�C���^�����o�ϐ�
};