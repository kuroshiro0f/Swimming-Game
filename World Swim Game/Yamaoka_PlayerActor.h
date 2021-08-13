#pragma once

#include "Actor.h"

class Yamaoka_PlayerActor : public Actor
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

	Yamaoka_PlayerActor();
	~Yamaoka_PlayerActor();

	void Update(float _deltaTime);
	void UpdateActor(float _deltaTime) override;
	void StartProcess(float _deltaTime);
	void DrawActor();
	void PlayAnim();
	void attachAnim(int _animPlay);

	// �X�^�~�i�Q�[�W�̕`��
	void DrawSt(int _st, int _MaxSt, int _MinSt);
	// �S�[���܂ł̋����̕`��
	void DrawToGoal(int _playerPos, int _goalPos);


	// �S�[���܂ł̋��� //
	int dCount;      // �ǂꂾ���i�񂾂�        
	int maxdCount;   // �ǂ��܂Ői�߂�̂�
	int NowPos;
	// ������ //

	int st;      // �X�^�~�i
	int MaxSt;   // �X�^�~�i�ő�l
	int MinSt;   // �X�^�~�i�ŏ��l

private:
	PLAYER_STATE_ENUM mNowPlayerState;	//�v���C���[�̍��̏��

	KEY_STATE_ENUM mNowKeyState;		//�L�[�̍��̏��
	KEY_STATE_ENUM mPrevKeyState;		//�L�[��1�O�̏��

	int timer;
	bool startFlag;
};