#pragma once
#include "SceneBase.h"
#include "PlayEffect.h"
#include "PlayerActor.h"
#include "Camera.h"
#include "Font.h"

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

	//	�����̕\�����Ԃ̑���
	int m_startCount;			//	�X�^�[�g
	int m_finishCount;			//	�S�[��
	bool m_startFinishFlag;

	//	���̈ړ�
	int m_starX;				//	X����
	int m_starY;				//	Y����
	double m_starAngle;			//	�p�x

	//	�n�[�g�̊g�嗦
	double m_heartRate;
	double m_heartSpeed1;
	double m_heartSpeed2;

	//	���̕\���֌W
	double m_arrowRate;			//	�g�嗦
	double m_arrowAngle;		//	�p�x
	double m_arrow2Angle;		//	�p�x
	double m_arrow3Angle;		//	�p�x
	double m_arrow4Angle;		//	�p�x

	//	���p���̊g��k��
	int m_breathX, m_breathY;
	int m_breathX2, m_breathY2;
	int m_breathCount;
	bool m_breathFlag;

	//	���̈ړ�
	int m_sweat1X, m_sweat1Y;	//	��1
	int m_sweat2X, m_sweat2Y;	//	��2

	//	���X�g�̉��o�p
	int m_lastX, m_lastY;

	//	�O���t�B�b�N�n���h��
	int m_backGraphHandle;				//	�w�i
	int m_loadGraphHandle;				//	���[�h
	int m_tips1GraphHandle;				//	TIPS1
	int m_tips2GraphHandle;				//	TIPS2
	int m_tips3GraphHandle;				//	TIPS3
	int m_boyGraphHandle;				//	�j�̎q
	int m_boy2GraphHandle;				//	�j�̎q2
	int m_starGraphHandle;				//	��
	int m_sweat1GraphHandle;			//	��1
	int m_sweat2GraphHandle;			//	��2
	int m_heartGraphHandle;				//	�n�[�g
	int m_upArrowGraphHandle;			//	����
	int m_rightArrowGraphHandle;		//	�E���
	int m_downArrowGraphHandle;			//	�����
	int m_leftArrowGraphHandle;			//	�����
	int m_failGraphHandle;				//	���s
	int m_spaceGraphHandle;				//	�X�y�[�X
	int m_stGraphHandle;				//	�X�^�~�i
	int m_st2GraphHandle;				//	�X�^�~�i2
	int m_st3GraphHandle;				//	�X�^�~�i3
	int m_breathGraphHandle;			//	���p��
	int m_arrowBaseGraphHandle;			//	���̓y��
	int m_scoreBaseGraphHandle;			//	�X�R�A�̓y��
	int m_spaceBaseGraphHandle;			//	�X�y�[�X�̓y��
	int m_spaceBase2GraphHandle;		//	�X�y�[�X�̓y��2
	int m_stBaseGraphHandle;			//	�X�^�~�i�̓y��
	int m_finishArrowBaseGraphHandle;	//	���X�g�̖��̓y��
	int m_lastGraphHandle;				//	���X�g�̉��o


	//	�T�E���h�n���h��
	int m_bgmSoundHandle;			//	BGM
	int m_whistleSoundHandle;		//	�J
	int m_countDownSoundHandle;		//	�J�E���g�_�E��
	int m_goalSoundHandle;			//	�S�[��
	int m_arrowSoundHandle;			//	���
	int m_arrowFailSoundHandle;		//	���̎��s
	int m_spaceSoundHandle;			//	�X�y�[�X
	int m_breathSoundHandle;		//	���p��
	int m_loadSoundHandle;			//	���[�h

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
	bool m_sweat1Flag;				//	��1��\�����邩
	bool m_sweat2Flag;				//	��2��\�����邩
	bool m_heartFlag;				//	�n�[�g�̊g��Ək���̕ύX�p
	bool m_lastFlag;				//	���X�g�X�p�[�g���n�܂�����
	bool m_last1SucFlag;
	bool m_last2SucFlag;
	bool m_last3SucFlag;
	bool m_last4SucFlag;

	class Stage* m_stage;   // �X�e�[�W�N���X�ւ̃|�C���^�����o�ϐ�
	class Camera* m_camera;	// �J�����N���X�ւ̃|�C���^�����o�ϐ�
	class PlayerActor* m_actor;   // �A�N�^�[�N���X�ւ̃|�C���^�����o�ϐ�
	class PlayEffect* m_effect;				//	�G�t�F�N�g�v���[���[
	class Font* m_font;
};