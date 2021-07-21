#pragma once
#include "GameObject.h"
#include "Utils.h"

#define GIMMICKDIEEFFECT_ANI		0


#define GIMMICKDIEEFFECT_SPEED		0.1f
#define GIMMICKDIEEFFECT_SPEED_225	0.0924f
#define GIMMICKDIEEFFECT_SPEED_450	0.0707f
#define GIMMICKDIEEFFECT_SPEED_675	0.0383f

#define GIMMICKDIEEFFECT_BBOX_WIDTH		16
#define GIMMICKDIEEFFECT_BBOX_HEIGHT	16

class GimmickDieEffect : public CGameObject
{

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
public:
	GimmickDieEffect(int);
	virtual void SetState(int state);
};

