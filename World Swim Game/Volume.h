#pragma once
#include "DxLib.h"

class Volume
{
public:
	Volume();
	~Volume();

	void Update();
	void Draw();

	const int& GetVolumePal() const { return m_volumePal; }
private:
	int m_volumePal;
	bool m_checkKeyFlag;
};