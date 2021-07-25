
#include "Item.h"
Item::Item()
{
	SetState(ITEM_STATE_IDLING);
	nx = 1;
}
Item::Item(int Type)
{
	this->Type = Type;
}

void Item::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state != ITEM_STATE_DISAPPEAR)
	{
		left = x;
		top = y;
		right = x + ITEM_BBOX_WIDTH;
		if (Type == ITEM_TYPE_FLOWER)
		{
			bottom = y - 30;
		}else
		bottom = y - ITEM_BBOX_HEIGHT;
	}
	else
	{
		left = top = right = bottom = 0;
	}
}

void Item::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();


}

void Item::Render()
{
	if (state != ITEM_STATE_DISAPPEAR)
		animation_set->at(0)->Render(x, y);

	//RenderBoundingBox();
}

void Item::SetState(int state)
{
	CGameObject::SetState(state);
	
}
