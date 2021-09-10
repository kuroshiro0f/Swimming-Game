#pragma once
#include "SceneBase.h"
#include "Yamaoka_PlayerActor.h"
#include "Yamaoka_Camera.h"

class PlayerActor;			//PlayerActor�ւ̎Q��
class Camera;				//Camera�ւ̎Q��

class GameScene :public SceneBase
{
	PlayerActor* player;
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

	GameScene();			//	�R���X�g���N�^
	~GameScene();			//	�f�X�g���N�^

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

	//	�O���t�B�b�N�n���h��
	int m_backGraphHandle;			//	�w�i
	int m_loadGraphHandle;			//	���[�h
	int m_tips1GraphHandle;			//	TIPS1
	int m_tips2GraphHandle;			//	TIPS2
	int m_tips3GraphHandle;			//	TIPS3
	int m_boyGraphHandle;			//	�j�̎q

	//	�T�E���h�n���h��
	int m_bgmSoundHandle;			//	BGM

	//	�A���t�@�l
	int m_alphaVal;

	//	�t���O
	int m_tipsFlag;					//	TIPS�̕\���ς���
	int m_loadingFlag;				//	���[�f�B���O�̕����̕\����ς���
	bool m_checkKeyFlag;			//	�L�[�������ꂽ�܂܂�
	bool m_fadeOutFinishFlag;		//	�t�F�[�h�A�E�g�̏I������
	bool m_loadFlag;				//	���[�h��ʂ�\�����邩
	bool m_loadFinishFlag;			//	���[�h���I�������
	bool m_boyPlusFlag;				//	�j�̎q�����������t���O

	class Stage* m_stage;   // �X�e�[�W�N���X�ւ̃|�C���^�����o�ϐ�
	class Camera* m_camera;	// �J�����N���X�ւ̃|�C���^�����o�ϐ�
	class Yamaoka_PlayerActor* m_actor;   // �A�N�^�[�N���X�ւ̃|�C���^�����o�ϐ�
};