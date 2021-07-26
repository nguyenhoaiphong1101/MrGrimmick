#pragma once
#include "GameObject.h"
#include "Gimmick.h"
#include "PlayScence.h"

#define BIG_CANNON_GRAVITY 0.0008f
#define BIG_CANNON_BBOX_WIDTH 20
#define BIG_CANNON_BBOX_HEIGHT 30


#define BIG_CANNON_STATE_IDLE 100
#define BIG_CANNON_STATE_MOVE 200

#define BIG_CANNON_ANI_IDLE 0

#define BIG_CANNON_ANI_MOVE 1


class CBigCannon :
	public CGameObject
{
	float getTimeFire = -1;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	DWORD booming_start = 0;
	//virtual void FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy);
public:
	bool isGimmick = false;
	bool isFly = false;
	int isFlying = 0;
	void StartBooming() { booming_start = GetTickCount(); }

	CBigCannon();
	virtual void SetState(int state);
};

