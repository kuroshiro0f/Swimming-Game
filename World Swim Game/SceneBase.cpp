#include "SceneBase.h"

//	初期音量
const int VOLUME_PAL = 40;

//	フォントサイズ
const int NORMAL_FONT_SIZE = 17;

SceneBase::SceneBase()
	: m_volumePal(VOLUME_PAL)
	, m_normalFontSize(NORMAL_FONT_SIZE)
{
}
