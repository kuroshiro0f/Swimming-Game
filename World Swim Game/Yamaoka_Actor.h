#pragma once
#include "DxLib.h"

class Yamaoka_Actor
{
public:
	Yamaoka_Actor();
	virtual ~Yamaoka_Actor();

	// Update�֐��@Game����R�[�������
	void Update(float _deltaTime);
	//�A�N�^�[�̍X�V
	virtual void UpdateActor(float _deltaTime);
	//�`��
	virtual void DrawActor();
	//�A�j���[�V�����Đ��֐�
	void PlayAnim();
	//�Đ�����A�j���[�V�����̔ԍ����擾����֐�
	void attachAnim(int _animPlay);

	// Getters/setters�@�Q�b�^�[�E�Z�b�^�[//
	//�|�W�V������Getters/setters
	const VECTOR& GetPos() const
	{
		return mPosition;
	}
	void SetPos(const VECTOR _set)
	{
		mPosition = _set;
	}
	//�f�B���N�V������Getters/setters
	const VECTOR& GetDir() const
	{
		return mDirection;
	}
	void SetDir(const VECTOR _set)
	{
		mDirection = _set;
	}
	//rot��Getters/setters
	const VECTOR& GetRot()const
	{
		return mRotation;
	}
	void SetRot(const VECTOR _set)
	{
		mRotation = _set;
	}

	// �S�[���܂ł̋��� //
	int dCount;      // �ǂꂾ���i�񂾂�        
	int maxdCount;   // �ǂ��܂Ői�߂�̂�
	// ������ //

	float endStBar;    // �X�^�~�i�Q�[�W�̒���
	float startStBar;  // �X�^�~�i�Q�[�W�̎n�_
	float maxSt;       // �X�^�~�i�̍ő�l
	float nowSt;       // ���݂̃X�^�~�i
	float dSt;         // �X�^�~�i�̌����l

protected:
	VECTOR mPosition;	//�|�W�V����
	VECTOR mDirection;	//����
	VECTOR mRotation;	//��]
	VECTOR mVelocity;	//���x
	float mSpeed;		//�ړ����x

	//�L�������f��
	static int modelHandle;
	// �A�j���[�V�����p
	float animTotal;
	float animNowTime;
	int animIndex;

};