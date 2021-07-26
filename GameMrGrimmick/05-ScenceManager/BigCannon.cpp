#include "BigCannon.h"
#include "Utils.h"

CBigCannon::CBigCannon()
{
	for (int i = 0; i < 6; i++) {

		CBulletBigCannon* bomb = new CBulletBigCannon();
		CAnimationSets* ani = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = ani->Get(71);
		bomb->SetAnimationSet(ani_set);
		bomb->isBigCannon = true;
		ListBomb.push_back(bomb);
		((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->addObject(bomb);
	}
	SetState(BIG_CANNON_STATE_IDLE);
	nx = 1;
}

void CBigCannon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BIG_CANNON_BBOX_WIDTH;
	bottom = y - BIG_CANNON_BBOX_HEIGHT;
}

void CBigCannon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	// Simple fall down

	if(!isGimmick)
	vy -= BIG_CANNON_GRAVITY * dt;

	CGimmick* gimmick = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	//if (abs(gimmick->x - this->x)>30 || gimmick->vx>0)
	//{
	//	SetState(HEIGHT_CANNON_STATE_IDLE);
	//}

	if (gimmick->isPush) {
		SetState(BIG_CANNON_STATE_MOVE);
	}
	else {
		vx = 0;
		SetState(BIG_CANNON_STATE_IDLE);
	}


	if (timeShoot != 0) {
		if (GetTickCount() - timeShoot > 4000)
		{
			DebugOut(L"Bắn\n");
			Fire();
			timeShoot = GetTickCount();
		}
	}


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



		// Collision logic with other objects
		//
		if(!isGimmick)
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			isGimmick = true;

		}
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		/*if (nx!=0) vx = 0;*/
		if (ny != 0) vy = 0;
	}

	// clean up collision events

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CBigCannon::Render()
{
	
	int ani = BIG_CANNON_ANI_IDLE;
	if (state == BIG_CANNON_STATE_IDLE)
	{
		ani = BIG_CANNON_ANI_IDLE;
	}
	else if (state == BIG_CANNON_STATE_MOVE)
	{
		ani = BIG_CANNON_ANI_MOVE;
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CBigCannon::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BIG_CANNON_STATE_MOVE:
		break;
	case BIG_CANNON_STATE_IDLE:
		//StartBooming();
		break;
	}
}


void CBigCannon::Fire()
{
	if (bullet < 6)
	{
		ListBomb[bullet]->SetPosition(x -16,y);
		ListBomb[bullet]->SetState(BULLET_BIG_STATE_FALLING);
		bullet++;
	}
}
