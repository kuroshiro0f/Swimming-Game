//-----------------------------------------------------------------------------
// @brief  ���C������.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include "SceneMgr.h"
#include "Title.h"
#include "GameScene.h"
#include "Ueyama_Title.h"
#include "Ueyama_GameScene.h"
#include "Ueyama_Result.h"

//-----------------------------------------------------------------------------
// @brief  ���C���֐�.
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// �c�w���C�u��������������
	if (DxLib_Init() == -1)
	{
		return -1;	// �G���[���N�����璼���ɏI��
	}
	// ��ʃ��[�h�̃Z�b�g
	SetGraphMode(1920, 1080, 16);
	ChangeWindowMode(TRUE);
	SetDrawScreen(DX_SCREEN_BACK);		//	����ʂ̐ݒ�

	//	�f���^�^�C���Ǘ��p�̕ϐ����Z�b�g
	LONGLONG NowTime;
	LONGLONG Time;
	float DeltaTime;

	//	�V�X�e�����Ԃ��擾
	Time = GetNowHiPerformanceCount();

	//	�ŏ��̌o�ߎ��Ԃ͉���0.0000001f�b�ɂ��Ă���
	DeltaTime = 0.000001f;

	//	�V�[���}�l�[�W���[�N���X�̃C���X�^���X�𐶐�
	SceneMgr* Scene = new SceneMgr;

	//	�^�C�g���V�[�����Z�b�g
	Scene->SetScene(new Title());

	// �G�X�P�[�v�L�[��������邩�E�C���h�E��������܂Ń��[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// �V�[������
		Scene->Update(DeltaTime);

		// �`�揈��
		Scene->Draw();

		// BGM����
		Scene->Sound();

		// ����ʂ̓��e��\��ʂɔ��f������
		ScreenFlip();

		//	���݂̃V�X�e�����Ԃ��擾
		NowTime = GetNowHiPerformanceCount();

		// �O��擾�������Ԃ���̌o�ߎ��Ԃ�b�ɕϊ����ăZ�b�g
		// ( GetNowHiPerformanceCount �Ŏ擾�ł���l�̓}�C�N���b�P�ʂȂ̂� 1000000 �Ŋ��邱�Ƃŕb�P�ʂɂȂ� )
		DeltaTime = (NowTime - Time) / 1000000.0f;

		//	����擾�������Ԃ�ۑ�
		Time = NowTime;

		//	��ʏ���
		ClearDrawScreen();
	}

	// �V�[�����폜
	delete(Scene);

	// �c�w���C�u�����̌�n��
	DxLib_End();

	// �\�t�g�̏I��
	return 0;
}

