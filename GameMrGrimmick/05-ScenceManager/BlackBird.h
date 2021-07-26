#pragma once
#include "GameObject.h"
#include "Gimmick.h"
#define BLACKBIRD_FLYING_SPEED 0.04f;
#define BLACKBIRD_BBOX_WIDTH_FLY 48
#define BLACKBIRD_BBOX_HEIGHT_FLY 20
#define BLACKBIRD_BBOX_WIDTH_IDLE 25
#define BLACKBIRD_BBOX_HEIGHT_IDLE 46

#define BLACKBIRD_STATE_IDLE 100
#define BLACKBIRD_STATE_FLYING 200

#define BLACKBIRD_ANI_IDLE_RIGHT 0
#define BLACKBIRD_ANI_IDLE_LEFT	1
#define BLACKBIRD_ANI_FLYING_RIGHT 2
#define BLACKBIRD_ANI_FLYING_LEFT 3

#define BLACKBIRD_TIME_CHANGE_FLY 1000


class  BlackBird :
	public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	//virtual void FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy);
	DWORD Start_change = 0;
	DWORD Start_idle_left = 0;
public:
	void StartChange() { Start_change = GetTickCount(); }
	BlackBird();
	virtual void SetState(int state);
};