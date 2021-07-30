#pragma once
#include "DxLib.h"

class Actor
{
public:
	Actor();
	virtual ~Actor();

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


protected:
	VECTOR mPosition;	//ポジション
	VECTOR mDirection;	//方向
	VECTOR mRotation;	//回転
	VECTOR mVelosity;	//速度
	float mSpeed;		//移動速度

	//キャラモデル
	static int modelHandle;
	// アニメーション用
	float animTotal;
	float animNowTime;
	int animIndex;

};