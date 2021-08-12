#pragma once
#include "DxLib.h"

class Yamaoka_Actor
{
public:
	Yamaoka_Actor();
	virtual ~Yamaoka_Actor();

	// Update関数　Gameからコールされる
	void Update(float _deltaTime);
	//アクターの更新
	virtual void UpdateActor(float _deltaTime);
	//描画
	virtual void DrawActor();
	//アニメーション再生関数
	void PlayAnim();
	//再生するアニメーションの番号を取得する関数
	void attachAnim(int _animPlay);

	// スタミナゲージの描画
	void DrawSt(int _st, int _MaxSt, int _MinSt);
	// ゴールまでの距離の描画
	void DrawToGoal(int _playerPos, int _goalPos);

	// Getters/setters　ゲッター・セッター//
	//ポジションのGetters/setters
	const VECTOR& GetPos() const
	{
		return mPosition;
	}
	void SetPos(const VECTOR _set)
	{
		mPosition = _set;
	}
	//ディレクションのGetters/setters
	const VECTOR& GetDir() const
	{
		return mDirection;
	}
	void SetDir(const VECTOR _set)
	{
		mDirection = _set;
	}
	//rotのGetters/setters
	const VECTOR& GetRot()const
	{
		return mRotation;
	}
	void SetRot(const VECTOR _set)
	{
		mRotation = _set;
	}

	// ゴールまでの距離 //
	int dCount;      // どれだけ進んだか        
	int maxdCount;   // どこまで進めるのか
	int NowPos;
	// 未完成 //

	int st;      // スタミナ
	int MaxSt;   // スタミナ最大値
	int MinSt;   // スタミナ最小値

protected:
	VECTOR mPosition;	//ポジション
	VECTOR mDirection;	//方向
	VECTOR mRotation;	//回転
	VECTOR mVelocity;	//速度
	float mSpeed;		//移動速度

	//キャラモデル
	static int modelHandle;
	// アニメーション用
	float animTotal;
	float animNowTime;
	int animIndex;

};