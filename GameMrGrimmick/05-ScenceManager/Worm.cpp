
#include "Worm.h"
Worm::Worm()
{
	nx = 1;
	SetState(WORM_STATE_WALKING);

}

void Worm::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == WORM_STATE_DIE)
	{
		left = top = right = bottom = 0;
	}
	else
	{
		left = x;
		top = y;
		right = x + WORM_BBOX_WIDTH;
		bottom = y - WORM_BBOX_HEIGHT;
	}
}

void Worm::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	// Simple fall down
	vy -= WORM_GRAVITY * dt;

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
		if (state != WORM_STATE_DIE)
		{
			if (back)
			{
				if (tempbacky - y >= 0.5f)
				{
					y += 5;
					if (vx < 0)
						x += 12;
					else
						x -= 12;
					vx = -vx;
					nx = -nx;
				}
			}
		}
	}
	else
	{

		// land ...fly
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		back = true;

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		/*if (nx!=0) vx = 0;*/
		if (ny != 0) vy = 0;

		tempbacky = y;
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Worm::Render()
{
	int ani = WORM_ANI_WALK_RIGHT;
	if (state == WORM_STATE_WALKING)
	{
		if (nx > 0)
			ani = WORM_ANI_WALK_RIGHT;
		else
			ani = WORM_ANI_WALK_LEFT;
	}


	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void Worm::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case WORM_STATE_DIE:
		vx = 0;
		vy = 0.02f;
		break;
	case WORM_STATE_WALKING:
		if (nx < 0)
		{
			vx = -WORM_WALKING_SPEED;
		}
		else
		{
			vx = WORM_WALKING_SPEED;
		}
	}
}
