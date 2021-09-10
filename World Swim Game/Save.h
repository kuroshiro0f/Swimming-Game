#pragma once

#include <stdio.h>

#define SAVE Save::GetInstance()

class Save
{
	typedef struct {
		int Time_First;			//	��ʂ̃^�C��
		int Time_Second;		//	��ʂ̃^�C��
		int Time_Third;			//	�O�ʂ̃^�C��
	}SaveData_t;

public:
	static Save* Instance();

	static Save* GetInstance() { return _Instance; }

	static void DeleteInstance();

	void SetTime(int _time);

	int GetFirstTime() { return m_firstTime; }
	int GetSecondTime() { return m_secondTime; }
	int GetThirdTime() { return m_thirdTime; }

private:
	Save();
	~Save();

	static Save* _Instance;

	int m_firstTime;
	int m_secondTime;
	int m_thirdTime;

	FILE* m_fp;
	SaveData_t m_save;
};