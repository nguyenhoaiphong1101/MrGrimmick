#include "BlackBird.h"
#include "Brick.h"
#include "Utils.h"
#include "PlayScence.h"
#include <algorithm>
BlackBird::BlackBird()
{
	nx = 1;
	SetState(BLACKBIRD_STATE_IDLE);

}

void BlackBird::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (state != BLACKBIRD_STATE_FLYING)
	{
		right = x + BLACKBIRD_BBOX_WIDTH_IDLE;
		bottom = y - BLACKBIRD_BBOX_HEIGHT_IDLE;
	}
	else
	{
		right = x + BLACKBIRD_BBOX_WIDTH_FLY;
		bottom = y - BLACKBIRD_BBOX_HEIGHT_FLY;
	}
}

void BlackBird::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (dynamic_cast<CBrick*>(coObjects->at(i)))
		{
			continue;
		}

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);

}
void BlackBird::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	// Simple fall down

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

		if (state == BLACKBIRD_STATE_FLYING)
		{
			if (GetTickCount() - Start_change >= BLACKBIRD_TIME_CHANGE_FLY)
			{
				vy = -vy;
				StartChange();
			}
			if (x > 1685)
			{
				x = 1681;
				y = 702;
				nx = -1;
				SetState(BLACKBIRD_STATE_IDLE);
			}
		}

		CGimmick* gimmick = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		if (state == BLACKBIRD_STATE_IDLE && nx < 0)
		{
			if (x - gimmick->x < 150)
			{
				//DebugOut(L"toa do x:%d\n", gimmick->x);
				if (Start_idle_left == 0)
				{
					Start_idle_left = GetTickCount();
				}
			}
			if (GetTickCount() - Start_idle_left > 3000)
			{
				y -= 5;
				SetState(BLACKBIRD_STATE_FLYING);
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


		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		/*if (nx!=0) vx = 0;*/
		if (ny != 0) vy = 0;

	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void BlackBird::Render()
{
	int ani = BLACKBIRD_ANI_IDLE_RIGHT;
	if (state == BLACKBIRD_STATE_IDLE)
	{
		if (nx > 0)
		{
			ani = BLACKBIRD_ANI_IDLE_RIGHT;
		}
		else
		{
			ani = BLACKBIRD_ANI_IDLE_LEFT;
		}
	}
	else if (state == BLACKBIRD_STATE_FLYING)
	{
		if (nx > 0)
		{
			ani = BLACKBIRD_ANI_FLYING_RIGHT;
		}
		else
		{
			ani = BLACKBIRD_ANI_FLYING_LEFT;
		}
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void BlackBird::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BLACKBIRD_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case BLACKBIRD_STATE_FLYING:

		vy = -0.02f;
		if (nx < 0)
		{
			vx = -BLACKBIRD_FLYING_SPEED;
		}
		else
		{
			vx = BLACKBIRD_FLYING_SPEED;
		}
	}
}