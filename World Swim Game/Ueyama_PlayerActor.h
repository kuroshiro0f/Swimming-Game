#pragma once
#include "Actor.h"

//-----------------------------------------------------------------------------
// �@�ύX�Ȃ�
//-----------------------------------------------------------------------------

class Ueyama_PlayerActor : public Actor
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
		STATE_KEY_IDLE = 0,
		STATE_KEY_UP = 1,
		STATE_KEY_DOWN = 2,
		STATE_KEY_RIGHT = 3,
		STATE_KEY_LEFT = 4,
		STATE_KEY_SPACE,

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

	Ueyama_PlayerActor();
	~Ueyama_PlayerActor();

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

	//turnFlag�̃Q�b�^�[
	bool GetTurnFlag() { return turnFlag; };

	// �K�E�Z
	void Skill(float _playerPos, float _goalPos);

	bool GetInputSpaceFlag() { return inputSpaceFlag; };

	//inputArrowFlag�̃Q�b�^�[
	bool GetInputArrpwFlag() { return inputArrowFlag; };

	//���^�[���̕]�����邽�߂̃v���C���[�̍��W��⊮
	float mPosX;

	// �S�[���܂ł̋��� //
	float dCount;      // �ǂꂾ���i�񂾂�        
	float maxdCount;   // �ǂ��܂Ői�߂�̂�
	int NowPos;    // �v���C���[�̌��ݍ��W

	int st;      // �X�^�~�i
	int MaxSt;   // �X�^�~�i�ő�l
	int MinSt;   // �X�^�~�i�ŏ��l

	int startTime;   // �Q�[���J�n����
	int tmpTime;     // ���ݎ��Ԃ��ꎞ�ۑ�
	int countUP;     // �o�ߎ���

	int count;       // ���̃V�[���ɍs���܂ł̃J�E���g    
	int countDown;		// �X�^�[�g�܂ł̃J�E���g�_�E��
	int skillCount;  // �X�L���̎g�p�J�E���g

	int inputTime;

	int turnGraphHandle;				//	�O���t�B�b�N�n���h��
	bool turnGraphFlag;					//	�\�����邩

	int randomKeyNumber;
	bool randomFlag;
	bool inputArrowFlag;				//���L�[���̓t���O

	bool countDownFinishFlag;		//	�J�E���g�_�E�����I�������

private:
	int inputStartTime;
	int inputEndTime;

	VECTOR mPrevPosition;

	PLAYER_STATE_ENUM mNowPlayerState;	//�v���C���[�̍��̏��

	KEY_STATE_ENUM mNowKeyState;		//�L�[�̍��̏��
	KEY_STATE_ENUM mPrevKeyState;		//�L�[��1�O�̏��

	Evaluation	   mEvlt;				//�]��

	bool startFlag;
	bool turnFlag;        // �^�[���t���O
	bool inputSpaceFlag;				//SPACE���̓t���O
};