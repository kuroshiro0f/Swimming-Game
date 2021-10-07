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

	Nagatomo_PlayerActor();
	~Nagatomo_PlayerActor();

	//�A�b�v�f�[�g�֐�
	void Update(float _deltaTime);
	//�A�N�^�[�̃A�b�v�f�[�g�֐�
	void UpdateActor(float _deltaTime) override;
	//�X�^�[�g�����̊֐�
	void StartProcess(float _deltaTime);
	//�A�N�^�[�̕`��֐�
	void DrawActor();
	//�A�j���[�V�����֐�
	void PlayAnim(float _deltaTime);
	//�A�j���[�V�����̃A�^�b�`�֐�
	void attachAnim(int _animPlay);

	// �X�^�~�i�Q�[�W�̕`��
	void DrawSt(int _st, int _MaxSt, int _MinSt);
	// �S�[���܂ł̋����̕`��
	void DrawToGoal(float _playerPos, float _goalPos);
	//�����_���Ȑ����𐶐�
	void GenRandomKey(bool _randomFlag, bool _ultFlag);
	//�����ړ�
	void AutoMove(bool _turnFlag ,float _deltaTime);
	// ���X�g�X�p�[�g
	void LastSpurt();

	//���͏���
	void ProcessInput(bool _ultFlag,bool _skillFlag);
	//���͐������̏���
	void SuccessInput(int _st, int _MinSt);
	//���͎��s���̏���
	void FailureInput(int _st, int _MinSt);
	//���͎��Ԃ𒴂����Ƃ��̏���
	void InputOverTime(bool _inputArrowFlag, float _inputTime, float _inputLimitTime);

	//�^�[������
	void TurnProcess(bool _turnFlag, VECTOR _mPosition);

	//�����_���Ȑ����𐶐��iult�p�j
	void UltNumber(bool _randomFlag);
	//�E���g�̓��͏���
	void UltProcessInput(int _arrow[], int _size);

	//�c��X�^�~�i�ɂ�鑬�x
	void StaminaCoefficient(int _st, int _halfSt, int _quarterSt);

	//turnFlag�̃Q�b�^�[
	bool GetTurnFlag() { return turnFlag; };
	//inputSpaceFlag�̃Q�b�^�[
	bool GetInputSpaceFlag() { return inputSpaceFlag; };
	//inputArrowFlag�̃Q�b�^�[
	bool GetInputArrpwFlag() { return inputArrowFlag; };

	//���^�[���̕]�����邽�߂̃v���C���[�̍��W��⊮
	float mPosX;

	// �S�[���܂ł̋��� //
	float dCount;      // �ǂꂾ���i�񂾂�        
	float maxdCount;   // �ǂ��܂Ői�߂�̂�
	int NowPos;    // �v���C���[�̌��ݍ��W

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
	int countDown;		// �X�^�[�g�܂ł̃J�E���g�_�E��
	int skillCount;  // �X�L���̎g�p�J�E���g
	int skillTime;   // �X�L���̌��ʎ���

	int stopTime;    // ��~����

	float inputTime;			//���͎���
	float inputLimitTime;		//���͐�������

	int arrow[4];					//���X�g�X�p�[�g�̃L�[

	int turnGraphHandle;				//	�O���t�B�b�N�n���h��
	bool turnGraphFlag;					//	�\�����邩

	int randomKeyNumber;
	bool randomFlag;
	bool inputArrowFlag;				//���L�[���̓t���O

	bool countDownFinishFlag;		//	�J�E���g�_�E�����I�������
	bool ultLimitFlag;				//�@�E���g�̐���
	bool ultFlag;


	bool turnFlag;						 // �^�[���t���O

private:
	int inputStartTime;
	int inputEndTime;
	int inputCount;						//���͉�

	float addStaminaSpeed;				//���Z�����X�^�~�i�ɂ��X�s�[�h

	VECTOR mPrevPosition;

	PLAYER_STATE_ENUM mNowPlayerState;	//�v���C���[�̍��̏��

	KEY_STATE_ENUM mNowKeyState;		//�L�[�̍��̏��
	KEY_STATE_ENUM mPrevKeyState;		//�L�[��1�O�̏��

	Evaluation	   mEvlt;				//�]��

	const float D_COUNT = 0.355f;

	//	�X�^�~�i�Q�[�W�̐F���ς��c��Q�[�W��
	const int GREEN = 300;
	const int ORANGE = 150;
	//const int RED = 3;

	//	�X�^�~�i�Q�[�W�̕\���ʒu
	const int ST_FIRST_X = 650;
	const int ST_FIRST_Y = 1000;
	const int ST_END_X = 1250;
	const int ST_END_Y = 1035;

	//	�X�^�~�i�̌�����
	const int ST_SUC_DEC = 60;
	const int ST_FAI_DEC = 100;

	//�X�s�[�h�֘A
	const VECTOR missSpeed = VGet(5, 0, 0);		//���̓~�X�����Ƃ��̃X�s�[�h
	const float addSpeed = 3.0f;				//���Z�����X�s�[�h
	const float maxSpeed = 15.0f;				//�ő�X�s�[�h

	//���͊֘A
	const float limitTime = 1.5f;				//���͐�������
	const float maxTime = 0.5f;					//�ő厞��

	const int dCountUlt = 15;					//�E���g���g����悤�ɂȂ�c�苗��

	bool startFlag;
	bool inputSpaceFlag;				//SPACE���̓t���O
	bool mCheckKeyFlag;					//�������΍�t���O
	bool skillFlag;					  // �X�L�����g�p�������ǂ���
	bool finishFlag;					// �S�[���������ǂ���
	bool m_ultCheckFlag;					//	���X�g�X�p�[�g�̖���\�����I��������ǂ���
	int m_ultFinishFlag;					//	���X�g�X�p�[�g�Ŗ��������I�������
};