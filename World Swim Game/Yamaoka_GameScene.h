#pragma once
#include "SceneBase.h"

class Yamaoka_GameScene : public SceneBase
{
public:

	enum class GAME_SCENE_STATE
	{
		FADE_IN,
		GAME,
		FADE_OUT
	};

	Yamaoka_GameScene();				//	�R���X�g���N�^
	~Yamaoka_GameScene();			    //	�f�X�g���N�^

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

	class Yamaoka_Stage* m_stage;   // �X�e�[�W�N���X�ւ̃|�C���^�����o�ϐ�
	class Yamaoka_Camera* m_camera; // �J�����N���X�ւ̃|�C���^�����o�ϐ�
	class Yamaoka_Actor* m_actor;   // �A�N�^�[�N���X�ւ̃|�C���^�����o�ϐ�
	
	int m_startTime;                // �Q�[���J�n����
	int m_tmpTime;                  // ���ݎ��Ԃ��ꎞ�ۑ�
	int m_countUP;                  // �o�ߎ���
};