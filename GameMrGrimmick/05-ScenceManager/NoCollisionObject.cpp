
#include "NoCollisionObject.h"
NoCollisionObject::NoCollisionObject()
{
	SetState(ITEM_STATE_IDLING);
	nx = 1;
}

void NoCollisionObject::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = 0;
	top = 0;
	right = 0;
	bottom = 0;
}

void NoCollisionObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);


	//vector<LPCOLLISIONEVENT> coEvents;
	//vector<LPCOLLISIONEVENT> coEventsResult;

	//coEvents.clear();


	//for (UINT i = 0; i < coObjects->size(); i++)
	//{
	//	LPGAMEOBJECT obj = coObjects->at(i);

	//}
	//CalcPotentialCollisions(coObjects, coEvents);
	//// No collision occured, proceed normally
	//if (coEvents.size() == 0)
	//{

	//	x += dx;
	//	y += dy;

	//}
	//else
	//{

	//	// land ...fly
	//	float min_tx, min_ty, nx = 0, ny;
	//	float rdx = 0;
	//	float rdy = 0;
	//	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

	//	x += min_tx * dx + nx * 0.4f;
	//	y += min_ty * dy + ny * 0.4f;

	//	/*if (nx!=0) vx = 0;*/
	//	if (ny != 0) vy = 0;

	//	// Collision logic with other objects
	//	//
	//	for (UINT i = 0; i < coEventsResult.size(); i++)
	//	{
	//		LPCOLLISIONEVENT e = coEventsResult[i];

	//		if (e->nx != 0 && ny == 0)
	//		{
	//			this->vx = -this->vx;
	//			this->nx = -this->nx;

	//		}
	//	}
	//}

	//// clean up collision events
	//for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void NoCollisionObject::Render()
{
	int ani = ITEM_ANI_BOTTLE_ORANGE;
	ani = 0;
	/*if (state == ITEM_STATE_IDLING)
	{
		if (isAppear)
		{
			switch (Type)
			{
			case ITEM_TYPE_BOTTLE_ORANGE:
				ani = ITEM_ANI_BOTTLE_ORANGE;
				break;
			case ITEM_TYPE_BOTTLE_RED:
				ani = ITEM_ANI_BOTTLE_RED;
				break;
			case ITEM_TYPE_BOTTLE_BLACK:
				ani = ITEM_ANI_BOTTLE_BLACK;
				break;
			case ITEM_TYPE_BOTTLE_BIG_RED:
				ani = ITEM_ANI_BOTTLE_BIG_RED;
				break;
			}
		}
	}*/

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void NoCollisionObject::SetState(int state)
{
	CGameObject::SetState(state);
	/*switch (state)
	{
	case CANNON_STATE_GREEN:

		break;
	case CANNON_STATE_RED:
		break;
	}*/
}
