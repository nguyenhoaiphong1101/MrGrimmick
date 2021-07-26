#include "BulletBigCannon.h"
#include "Utils.h"
#include "Brick.h"
#include "Incline.h"

CBulletBigCannon::CBulletBigCannon()
{
	SetState(BULLET_BIG_STATE_DISAPPEAR);
	nx = 1;
}
void CBulletBigCannon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == BULLET_BIG_STATE_DISAPPEAR)
	{
		left = top = right = bottom = 0;
		return;
	}

	left = x;
	top = y;
	right = x + BULLET_BIG_BBOX_WIDTH;
	bottom = y - BULLET_BIG_BBOX_HEIGHT;
}

void CBulletBigCannon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	// Simple fall down
	if (!isBigCannon)
		vy -= BULLET_BIG_GRAVITY * dt;


	// thời gian hiện animation nổ
	if (state == BULLET_BIG_STATE_DESTROY)
	{
		if ((GetTickCount() - booming_start > 500))
		{
			SetState(BULLET_BIG_STATE_DISAPPEAR);
			booming_start = 0;
		}
	}
	x += dx;
	y += dy;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);

	}
	CalcPotentialCollisions(coObjects, coEvents);
	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		
	}
	else
	{
		// land ...fly
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);


		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{
				if (state != BULLET_BIG_STATE_DESTROY)
					SetState(BULLET_BIG_STATE_DESTROY);
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CBulletBigCannon::Render()
{
	int ani = BULLET_BIG_ANI_FALLING;
	if (state == BULLET_BIG_STATE_FALLING)
	{
		ani = BULLET_BIG_ANI_FALLING;
	}
	else if (state == BULLET_BIG_STATE_DESTROY)
	{
		ani = BULLET_BIG_ANI_DESTROY;
	}
	else if (state == BULLET_BIG_STATE_DISAPPEAR)
	{
		return;
	}
	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CBulletBigCannon::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BULLET_BIG_STATE_DESTROY:
		vx = 0;
		StartBooming();
		break;
	case BULLET_BIG_STATE_IDLING:

		break;
	case BULLET_BIG_STATE_FALLING:
		vx = -0.25f;
	}
}
