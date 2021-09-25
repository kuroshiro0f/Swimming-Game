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
		STATE_KEY_IDLE = 0,
		STATE_KEY_UP = 1,
		STATE_KEY_DOWN = 2,
		STATE_KEY_RIGHT = 3,
		STATE_KEY_LEFT = 4,
		STATE_KEY_C = 5,
		STATE_KEY_S,
		STATE_KEY_A,
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

	Yamaoka_PlayerActor();
	~Yamaoka_PlayerActor();

	// �A�b�v�f�[�g�֐�
	void Update(float _deltaTime);
	// �A�N�^�[�̃A�b�v�f�[�g�֐�
	void UpdateActor(float _deltaTime) override;
	// �X�^�[�g�����̊֐�
	void StartProcess(float _deltaTime);
	// �A�N�^�[�̕`��֐�
	void DrawActor();
	// �A�j���[�V�����֐�
	void PlayAnim(float _deltaTime);
	// �A�j���[�V�����̃A�^�b�`�֐�
	void attachAnim(int _animPlay);

	// �X�^�~�i�Q�[�W�̕`��
	void DrawSt(int _st, int _MaxSt, int _MinSt);
	// �S�[���܂ł̋����̕`��
	void DrawToGoal(float _playerPos, float _goalPos);

	//turnFlag�̃Q�b�^�[
	bool GetTurnFlag() { return turnFlag; };

	// ���X�g�X�p�[�g
	void LastSpurt();

	// inputSpaceFlag�̃Q�b�^�[
	bool GetInputSpaceFlag() { return inputSpaceFlag; };

	// inputArrowFlag�̃Q�b�^�[
	bool GetInputArrpwFlag() { return inputArrowFlag; };

	// ���^�[���̕]�����邽�߂̃v���C���[�̍��W��⊮
	float mPosX;

	// �S�[���܂ł̋��� //
	float dCount;      // �ǂꂾ���i�񂾂�
	float maxdCount;   // �ǂ��܂Ői�߂�̂�
	int NowPos;        // �v���C���[�̌��ݍ��W

	//�X�^�~�i�֘A
	int st;						// �X�^�~�i
	int MaxSt;					// �X�^�~�i�ő�l
	int MinSt;					// �X�^�~�i�ŏ��l
	int halfSt;					//�X�^�~�i�i50%�j
	int quarterSt;				//�X�^�~�i�i25%�j

	int startTime;   // �Q�[���J�n����
	int tmpTime;     // ���ݎ��Ԃ��ꎞ�ۑ�
	int countUP;     // �o�ߎ���

	int count;       // ���̃V�[���ɍs���܂ł̃J�E���g
	int countDown;   // �X�^�[�g�܂ł̃J�E���g�_�E��
	int skillCount;  // �X�L���̎g�p�J�E���g

	int stopTime;    // ��~����

	float inputTime;			//���͎���
	float inputLimitTime;		//���͐�������

	int turnGraphHandle;				//	�O���t�B�b�N�n���h��
	bool turnGraphFlag;					//	�\�����邩

	int randomKeyNumber;
	bool randomFlag;
	bool inputArrowFlag;			//���L�[���̓t���O

	bool countDownFinishFlag;		//	�J�E���g�_�E�����I�������

private:
	int inputStartTime;
	int inputEndTime;

	float addStaminaSpeed;				//���Z�����X�^�~�i�ɂ��X�s�[�h

	VECTOR mPrevPosition;

	PLAYER_STATE_ENUM mNowPlayerState;	//�v���C���[�̍��̏��

	KEY_STATE_ENUM mNowKeyState;		//�L�[�̍��̏��
	KEY_STATE_ENUM mPrevKeyState;		//�L�[��1�O�̏��

	Evaluation	   mEvlt;				//�]��

	bool startFlag;
	bool turnFlag;						// �^�[���t���O
	bool inputSpaceFlag;				// SPACE���̓t���O
	bool mCheckKeyFlag;					// �������΍�t���O
	bool skillFlag;						// �X�L�����g�p�������ǂ���
	bool stFlag;						// �X�^�~�i���X�L���g�p���ɐ؂ꂽ���ǂ���

	bool finishFlag;					// �S�[���������ǂ���
};