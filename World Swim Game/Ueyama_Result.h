#pragma once

#include "SceneBase.h"
#include "Font.h"

//-----------------------------------------------------------------------------
// �@�ύX�Ȃ�
//-----------------------------------------------------------------------------

class Ueyama_Result : public SceneBase
{
public:

	enum class RESULT_SCENE_STATE
	{
		FADE_IN,
		RESULT,
		TIME,
		MEDAL,
		EVA,
		ENTER,
		FADE_OUT
	};

	typedef struct {
		int TIME_FIRST;			//	��ʂ̃^�C��
		int TIME_SECOND;		//	��ʂ̃^�C��
		int TIME_THIRD;			//	�O�ʂ̃^�C��
	}ranking;

	Ueyama_Result(const int _time);			//	�R���X�g���N�^
	~Ueyama_Result();			//�@�f�X�g���N�^

	SceneBase* Update(float _deltaTime)override;	//	�X�V
	void Draw()override;			//	�`��
	void Sound()override;			//	���y
	void Load()override;			//	������

	void UpdateTransparent();			//	�����̓��ߗʍX�V

private:
	RESULT_SCENE_STATE m_state;

	//	�A���t�@�l
	int m_alphaVal;

	//	�N���A�^�C��
	int m_time;

	//	���ԊǗ�
	int m_resultTime;

	//	���̊g��k���̊Ǘ�
	int m_bigStarX1;
	int m_bigStarX2;
	int m_bigStarY1;
	int m_bigStarY2;
	int m_smallStarX1;
	int m_smallStarX2;
	int m_smallStarY1;
	int m_smallStarY2;

	// �����̓���
	int m_transpVal;				//	���ߗ�
	int m_fadeTransVal;				//	�t�F�[�h�A�E�g���̓��ߗ�
	int m_permeationAmount;			//	���t���[�����ߗ�

	//	�O���t�B�b�N�n���h��
	int m_backGraphHandle;				//	�w�i
	int m_logoGraphHandle;				//	���S
	int m_evaluationGraphHandle;		//	�]��
	int m_guidanceGraphHandle;			//	���̃V�[���ւ̈ē�
	int m_medalGraphHandle;				//	���_��
	int m_bigStarGraphHandle;			//	�傫�Ȑ�
	int m_smallStarGraphHandle;			//	�����Ȑ�
	int m_nowGraphHandle;				//	���̃^�C��
	int m_firstGraphHandle;				//	��ʂ̃^�C��
	int m_secondGraphHandle;			//	��ʂ̃^�C��
	int m_thirdGraphHandle;				//	�O�ʂ̃^�C��

	//	�T�E���h�n���h��
	int m_bgmSoundHandle;				//	BGM
	int m_se1SoundHandle;				//	���ʉ�
	int m_se2SoundHandle;				//	���ʉ�
	int m_se3SoundHandle;				//	���ʉ�
	int m_se4SoundHandle;				//	���ʉ�

	//	�t���O
	int m_resultFlag;					//	���U���g�̐��ڂ��Ǘ�����
	bool m_starScallFlag;				//	�����g�傷�邩�i���Ȃ��ꍇ�͏k���j
	bool m_checkKeyFlag;				//	�L�[�������ꂽ�܂܂�
	bool m_fadeOutFinishFlag;			//	�t�F�[�h�A�E�g�I��

	class Save* m_save;
	class Font* m_font;
};