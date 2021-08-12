#pragma once

#include "Actor.h"

class PlayerActor : public Actor
{
public:

	typedef enum PLAYER_STATE_ENUM
	{
		STATE_IDLE,
		STATE_SWIM,

		STATE_NUM,
	}PLAYER_STATE_ENUM;

	typedef enum KEY_STATE_ENUM
	{
		STATE_KEY_IDLE,
		STATE_KEY_RIGHT,
		STATE_KEY_LEFT,

		STATE_KEY_ENUM,
	}KEY_STATE_ENUM;

	PlayerActor();
	~PlayerActor();

	void Update(float _deltaTime);
	void UpdateActor(float _deltaTime) override;
	void StartProcess(float _deltaTime);
	void DrawActor();
	void PlayAnim();
	void attachAnim(int _animPlay);

private:
	PLAYER_STATE_ENUM mNowPlayerState;	//プレイヤーの今の状態

	KEY_STATE_ENUM mNowKeyState;		//キーの今の状態
	KEY_STATE_ENUM mPrevKeyState;		//キーの1つ前の状態

	int timer;
	bool startFlag;
};