#pragma once

#include "DxLib.h"

class Ueyama_SceneBase
{
public:
	Ueyama_SceneBase();						//	�R���X�g���N�^
	virtual ~Ueyama_SceneBase() {};			//	���z�f�X�g���N�^	(�v�����j

	//	�������z�֐��i�v�����j
	virtual Ueyama_SceneBase* Update(float _deltaTime) = 0;	//	�X�V
	virtual void Draw() = 0;			//	�`��
	virtual void Sound() = 0;			//	���y
	virtual void Load() = 0;			//	������
	int getVolume() { return m_volumePal; }
	void setVolume(int _volume) { m_volumePal = _volume; }

protected:
	int m_volumePal;
	int m_loadFontSize;
	int m_normalFontSize;
};