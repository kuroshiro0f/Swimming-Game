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

	//�A�b�v�f�[�g�֐�
	void Update(float _deltaTime);
	//�A�N�^�[�̃A�b�v�f�[�g�֐�
	void UpdateActor(float _deltaTime) override;
	//�X�^�[�g�����̊֐�
	void StartProcess(float _deltaTime);
	//�A�N�^�[�̕`��֐�
	void DrawActor();

	//------------------------YAMAOKA--------------------------��
	/// <summary>
	/// �A�j���[�V�����֐�
	/// �A�j���[�V�����̍Đ����Ԃ̏���������֐�
	/// </summary> 
	/// <param name="_deltaTime"> �f���^�^�C���@</param>
	void PlayAnim(float _deltaTime);

	/// <summary>
	/// �A�j���[�V�����֐�
	/// ���Ԗڂ̃A�j���[�V�������Đ����邩��I������֐�
	/// </summary>
	/// <param name="_animPlay">�@�Đ�����A�j���[�V�����̔ԍ��@</param>
	void attachAnim(int _animPlay);

	/// <summary>
	/// �X�^�~�i�`��
	/// </summary>
	/// <param name="_st">�@���݂̃X�^�~�i�̒l�@</param>
	/// <param name="_MaxSt">�@�X�^�~�i�̍ő�l�@</param>
	/// <param name="_MinSt">�@�X�^�~�i�̍ŏ��l�@</param>
	void DrawSt(int _st, int _MaxSt, int _MinSt);

	/// <summary>
	/// �S�[���܂ł̋����̕`��
	/// </summary>
	/// <param name="_playerPos">�@�v���C���[�̌��݂̍��W�̒l�@</param>
	/// <param name="_goalPos">�@�S�[���̍��W�@</param>
	void DrawToGoal(float _playerPos, float _goalPos);

	/// <summary>
	/// ���X�g�X�p�[�g�̏���
	/// </summary>
	void LastSpurt();
	//------------------------YAMAOKA--------------------------��

	void UltNumber(bool _randomFlag);
	//void ProcessInput(int _randomKeyNumber);
	void UltProcessInput(int _arrow[], int _size);

	//turnFlag�̃Q�b�^�[
	bool GetTurnFlag() { return turnFlag; };

	//inputSpaceFlag�̃Q�b�^�[
	bool GetInputSpaceFlag() { return inputSpaceFlag; };

	//inputArrowFlag�̃Q�b�^�[
	bool GetInputArrpwFlag() { return inputArrowFlag; };

	//���^�[���̕]�����邽�߂̃v���C���[�̍��W��⊮
	float mPosX;

	//------------------------YAMAOKA--------------------------��
	// �S�[���܂ł̋��� //
	float dCount;      // �ǂꂾ���i�񂾂�        
	float maxdCount;   // �ǂ��܂Ői�߂�̂�
	int NowPos;    // �v���C���[�̌��ݍ��W

	//�X�^�~�i�֘A
	int st;						// �X�^�~�i
	int MaxSt;					// �X�^�~�i�ő�l
	int MinSt;					// �X�^�~�i�ŏ��l
	//------------------------YAMAOKA--------------------------��

	int halfSt;					//�X�^�~�i�i50%�j
	int quarterSt;				//�X�^�~�i�i25%�j
	
	//------------------------YAMAOKA--------------------------��
	int startTime;   // �Q�[���J�n����
	int tmpTime;     // ���ݎ��Ԃ��ꎞ�ۑ�
	int countUP;     // �o�ߎ���

	int count;       // ���̃V�[���ɍs���܂ł̃J�E���g    
	int countDown;		// �X�^�[�g�܂ł̃J�E���g�_�E��
	int skillCount;  // �X�L���̎g�p�J�E���g
	int skillTime;   // �X�L���̌��ʎ���

	int stopTime;    // ��~����
	//------------------------YAMAOKA--------------------------��

	float inputTime;			//���͎���
	float inputLimitTime;		//���͐�������

	int arrow[4];				//���X�g�X�p�[�g�̃L�[

	int turnGraphHandle;		//	�O���t�B�b�N�n���h��
	bool turnGraphFlag;			//	�\�����邩

	int randomKeyNumber;
	bool randomFlag;
	bool inputArrowFlag;			//���L�[���̓t���O

	bool countDownFinishFlag;		//	�J�E���g�_�E�����I�������
	bool ultLimitFlag;				//�@�E���g�̐���
	bool ultFlag;

	//------------------------YAMAOKA--------------------------��
	bool turnFlag;					// �^�[���t���O
	//------------------------YAMAOKA--------------------------��
private:
	int inputStartTime;
	int inputEndTime;
	int inputCount;					//���͉�

	int countSpeed;

	float addStaminaSpeed;			//���Z�����X�^�~�i�ɂ��X�s�[�h

	VECTOR mPrevPosition;

	PLAYER_STATE_ENUM mNowPlayerState;	//�v���C���[�̍��̏��

	KEY_STATE_ENUM mNowKeyState;		//�L�[�̍��̏��
	KEY_STATE_ENUM mPrevKeyState;		//�L�[��1�O�̏��

	Evaluation	   mEvlt;				//�]��

	bool inputSpaceFlag;				//SPACE���̓t���O
	bool mCheckKeyFlag;					//�������΍�t���O
	bool m_ultCheckFlag;					//	���X�g�X�p�[�g�̖���\�����I��������ǂ���
	int m_ultFinishFlag;					//	���X�g�X�p�[�g�Ŗ��������I�������

	//------------------------YAMAOKA--------------------------��
	bool skillFlag;					  // �X�L�����g�p�������ǂ���
	bool startFlag;
	bool finishFlag;					// �S�[���������ǂ���
	//------------------------YAMAOKA--------------------------��
};