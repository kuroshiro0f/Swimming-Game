#include "SceneBase.h"

//	��������
const int VOLUME_PAL = 40;

//	�t�H���g�T�C�Y
const int NORMAL_FONT_SIZE = 17;

SceneBase::SceneBase()
	: m_volumePal(VOLUME_PAL)
	, m_normalFontSize(NORMAL_FONT_SIZE)
{
	nowTimeHandle = CreateFontToHandle("MS ����", 80, 2, DX_FONTTYPE_ANTIALIASING_4X4);
	loadFontHandle = CreateFontToHandle("Comic Sans MS", 60, 3, DX_FONTTYPE_ANTIALIASING_4X4);
	recordHandle = CreateFontToHandle("MS ����", 60, 3, DX_FONTTYPE_ANTIALIASING_4X4);
	startHandle = CreateFontToHandle("MS ����", 150, 2, DX_FONTTYPE_ANTIALIASING_4X4);
}
