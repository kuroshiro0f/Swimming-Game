#pragma once

#include <stdio.h>

class Ueyama_Save
{
	typedef struct {
		int TIME_FIRST;			//	��ʂ̃^�C��
		int TIME_SECOND;			//	��ʂ̃^�C��
		int TIME_THIRD;			//	�O�ʂ̃^�C��
	}SaveData_t;

public:
	Ueyama_Save();
	~Ueyama_Save();

	void SetTime(int _time);

	int GetFirstTime() { return m_firstTime; }
	int GetSecondTime() { return m_secondTime; }
	int GetThirdTime() { return m_thirdTime; }

private:
	int m_firstTime;
	int m_secondTime;
	int m_thirdTime;

	FILE* m_fp;
};