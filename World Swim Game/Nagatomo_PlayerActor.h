#pragma once

#include "Nagatomo_Actor.h"

class Nagatomo_PlayerActor : public Nagatomo_Actor
{
public:

	typedef enum PLAYER_STATE_ENUM
	{
		STATE_IDLE,
		STATE_SWIM,

		STATE_NUM,
	}PLAYER_STATE_ENUM;

	typedef enum KEY_STATE_ENUM
	{
		STATE_KEY_IDLE,
		STATE_KEY_RIGHT,
		STATE_KEY_LEFT,

		STATE_KEY_ENUM,
	}KEY_STATE_ENUM;

	//�]��
	typedef enum Evaluation
	{
		NONE,
		BAD,
		NORMAL,
		GOOD
	}Evaluation;

	Nagatomo_PlayerActor();
	~Nagatomo_PlayerActor();

	void Update(float _deltaTime);
	void UpdateActor(float _deltaTime) override;
	void StartProcess(float _deltaTime);
	void DrawActor();
	void PlayAnim(float _deltaTime);
	void attachAnim(int _animPlay);

	// �X�^�~�i�Q�[�W�̕`��
	void DrawSt(int _st, int _MaxSt, int _MinSt);
	// �S�[���܂ł̋����̕`��
	void DrawToGoal(float _playerPos, float _goalPos);

	//���^�[���̕]�����邽�߂̃v���C���[�̍��W��⊮
	float mPosX;

	// �S�[���܂ł̋��� //
	int dCount;      // �ǂꂾ���i�񂾂�        
	int maxdCount;   // �ǂ��܂Ői�߂�̂�
	int NowPos;		 // �v���C���[�̌��ݍ��W

	int st;      // �X�^�~�i
	int MaxSt;   // �X�^�~�i�ő�l
	int MinSt;   // �X�^�~�i�ŏ��l

	int startTime;   // �Q�[���J�n����
	int tmpTime;     // ���ݎ��Ԃ��ꎞ�ۑ�
	int countUP;     // �o�ߎ���

	int count;       // ���̃V�[���ɍs���܂ł̃J�E���g    
	int countDown;

private:
	PLAYER_STATE_ENUM mNowPlayerState;	//�v���C���[�̍��̏��

	KEY_STATE_ENUM mNowKeyState;		//�L�[�̍��̏��
	KEY_STATE_ENUM mPrevKeyState;		//�L�[��1�O�̏��

	Evaluation	   mEvlt;				//�]��

	bool startFlag;

};