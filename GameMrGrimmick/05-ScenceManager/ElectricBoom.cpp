
#include "ElectricBoom.h"
ElectricBoom::ElectricBoom()
{
	SetState(ELECTRICBOOM_STATE_WALKING);
	nx = 1;
}

void ElectricBoom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + ELECTRICBOOM_BBOX_WIDTH;
	bottom = y - ELECTRICBOOM_BBOX_HEIGHT;
}

void ElectricBoom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	// Simple fall down
	vy -= ELECTRICBOOM_GRAVITY * dt;

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

		x += dx;
		y += dy;

	}
	else
	{

		// land ...fly
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		/*if (nx!=0) vx = 0;*/
		if (ny != 0) vy = 0;

		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (e->nx != 0 && ny == 0)
			{
				this->vx = -this->vx;
				this->nx = -this->nx;

			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void ElectricBoom::Render()
{
	int ani = ELECTRICBOOM_ANI_WALK_RIGHT;
	if (state == ELECTRICBOOM_STATE_WALKING)
	{
		if (nx > 0)
			ani = ELECTRICBOOM_ANI_WALK_RIGHT;
		else
			ani = ELECTRICBOOM_ANI_WALK_LEFT;
	}
	else if (state == ELECTRICBOOM_STATE_FLYING)
	{
		if (nx > 0)
			ani = ELECTRICBOOM_ANI_FLY_RIGHT;
		else
			ani = ELECTRICBOOM_ANI_FLY_LEFT;
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void ElectricBoom::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case ELECTRICBOOM_STATE_DIE:
		vx = 0;
		vy = 0;
		break;
	case ELECTRICBOOM_STATE_WALKING:
		if (nx < 0)
		{
			vx = -ELECTRICBOOM_WALKING_SPEED;
		}
		else
		{
			vx = ELECTRICBOOM_WALKING_SPEED;
		}
	}
}
