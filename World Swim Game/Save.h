#pragma once

#include <stdio.h>

#define SAVE Save::GetInstance()

class Save
{
	typedef struct {
		int Time_First;			//	一位のタイム
		int Time_Second;		//	二位のタイム
		int Time_Third;			//	三位のタイム
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