#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Gimmick.h"
#include "Incline.h"
#include "Game.h"
#include "PlayScence.h"

#include "Portal.h"
#include "Slide.h"

CGimmick::CGimmick(float x, float y) : CGameObject()
{
	untouchable = 0;
	SetState(GIMMICK_STATE_IDLE);

	start_x = x; 
	start_y = y; 
	this->x = x; 
	this->y = y; 
}

void CGimmick::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	int ids = CGame::GetInstance()->GetCurrentScene()->GetId();
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	if(holdJump!=1 && !isIncline)
	vy -= GIMMICK_GRAVITY*dt;


	if (holdJump == 1)
	{
		if (abs(y - startJump) <= 80)
			SetState(GIMMICK_STATE_HOLD_JUMP);
		else
		{
			holdJump = 0;
			startJump = -1;
		}
	}

	

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	if (ids == 1)
	{
		if (y < 0)
		{
			CPortal* p = new CPortal(2);
			CGame::GetInstance()->SwitchScene(p->GetSceneId());
			return;
		}
	}

	coEvents.clear();

	// turn off collision when die 
	if (state!= GIMMICK_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount() - untouchable_start > GIMMICK_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}


	
	// No collision occured, proceed normally
	if (coEvents.size()==0)
	{
		x += dx; 
		y += dy;
		isIncline = false;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0; 
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (e->ny !=0 )
			{
				holdJump = 0;
				jump = 0;
			}
			if (dynamic_cast<Incline*>(e->obj)) {

				isIncline = true;

				float tran_y = -99999;

				Incline* incline = dynamic_cast<Incline*>(e->obj);

				if (incline->size == 1)
					incline_size = 1;
				else
					incline_size = 2;

				if (( /*GetState() != GIMMICK_STATE_JUMP &&*/ CGame::GetInstance()->IsKeyDown(DIK_RIGHT))
					|| GetState() == GIMMICK_STATE_WALKING_RIGHT) {
					direct_go = 1;
					if (incline->direct == 1) {
						direct_incline = 1;
					}
					else {
						direct_incline = -1;
					}
				}
				else if (( CGame::GetInstance()->IsKeyDown(DIK_LEFT))
					|| GetState() == GIMMICK_STATE_WALKING_LEFT) {
							direct_go = -1;
							if (incline->direct == 1) {
								direct_incline = 1;
							}
							else {
								direct_incline = -1;
							}
				}
				else {
					if (incline->direct == 1) {
						if (incline->size == 1) {
							incline_vx = -GIMMICK_INCLINE_DOWN_SPEED_X_1;
							incline_vy = -GIMMICK_INCLINE_DOWN_SPEED_Y_1;
						}
						else {
							incline_vx = -GIMMICK_INCLINE_DOWN_SPEED_X_2;
							incline_vy = -GIMMICK_INCLINE_DOWN_SPEED_Y_2;
						}
					}
					else {
						if (incline->size == 1) {
							incline_vx = GIMMICK_INCLINE_DOWN_SPEED_X_1;
							incline_vy = -GIMMICK_INCLINE_DOWN_SPEED_Y_1;
						}
						else {
							incline_vx = GIMMICK_INCLINE_DOWN_SPEED_X_2;
							incline_vy = -GIMMICK_INCLINE_DOWN_SPEED_Y_2;
						}
					}

				}

				//if (incline->direct == 1) {
				//	tran_y = isOnTriangle(incline->x, incline->y - 1, incline->x + incline->size, incline->y, incline->x + incline->size, incline->y - 1, x, y);
				//	if (tran_y <= 0)
				//		y += 0.2f;			// discount y to fall through slide
				//}
				//else {
				//	tran_y = isOnTriangle(incline->x, incline->y, incline->x, incline->y - 1, incline->x + incline->size, incline->y - 1, x, y);
				//	if (tran_y <= 0)
				//		y += 0.2f;			// discount y to fall through slide
				//}
			}
			else {
				isIncline = false;
			}
			if (dynamic_cast<Slide*>(e->obj))
			{
				Slide* slide = dynamic_cast<Slide*>(e->obj);
			
					if (slide->GetType() == SLIDE_TYPE_LEFT)
					{
						x -= dt * SLIDE_WALKING_SPEED;
					}
					else if (slide->GetType() == SLIDE_TYPE_RIGHT)
					{
						x += dt * SLIDE_WALKING_SPEED;
					}
			}

			//if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			//{
			//	CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);

			//	// jump on top >> kill Goomba and deflect a bit 
			//	if (e->ny < 0)
			//	{
			//		if (goomba->GetState()!= GOOMBA_STATE_DIE)
			//		{
			//			goomba->SetState(GOOMBA_STATE_DIE);
			//			vy = -GRIMMICK_JUMP_DEFLECT_SPEED;
			//		}
			//	}
			//	else if (e->nx != 0)
			//	{
			//		if (untouchable==0)
			//		{
			//			if (goomba->GetState()!=GOOMBA_STATE_DIE)
			//			{
			//					SetState(GRIMMICK_STATE_DIE);
			//			}
			//		}
			//	}
			//} // if Goomba
			//else if (dynamic_cast<CPortal *>(e->obj))
			//{
			//	CPortal *p = dynamic_cast<CPortal *>(e->obj);
			//	CGame::GetInstance()->SwitchScene(p->GetSceneId());
			//}
		}
		if (!isIncline) {

			x += min_tx * dx + nx * 0.4f;
			y += min_ty * dy + ny * 0.4f;

			if (nx != 0) vx = 0;
			if (ny != 0) vy = 0;
		}
		else {
			x += dx;
			if (isIncline) {
				y += min_ty * dy + ny * 0.4f;
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}


float CGimmick::isOnTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float x, float y)
{
	float q, w, e;
	q = (x - x1) / (x2 - x1) - (y - y1) / (y2 - y1);
	w = (x - x2) / (x3 - x2) - (y - y2) / (y3 - y2);
	e = (x - x3) / (x1 - x3) - (y - y3) / (y1 - y3);

	float tich = q * w * e;

	return tich;
}

void CGimmick::Render()
{
	int ani = -1;

	if (jump == 1)
	{
		if (nx > 0)
			ani = GIMMICK_ANI_JUMPING_RIGHT;
		else
			ani = GIMMICK_ANI_JUMPING_LEFT;
	}
	else if(state == GIMMICK_STATE_WALKING_RIGHT )
	{
		ani = GIMMICK_ANI_WALKING_RIGHT;
	}
	else if (state == GIMMICK_STATE_WALKING_LEFT )
	{
		ani = GIMMICK_ANI_WALKING_LEFT;
	}
	else if (state == GIMMICK_STATE_INCLINE_UP)
	{
		if (vx > 0)
			ani = GIMMICK_ANI_WALKING_RIGHT;
		else
			ani = GIMMICK_ANI_WALKING_LEFT;
	}
	else if (state == GIMMICK_STATE_INCLINE_DOWN)
	{
		if (vx > 0)
			ani = GIMMICK_ANI_WALKING_RIGHT;
		else
			ani = GIMMICK_ANI_WALKING_LEFT;
	}
	/*else if (state == GIMMICK_STATE_DIE)
	{
		if (waitToReset)
			die_effect->Render();

		return;
	}*/
	else if (state == GIMMICK_STATE_IDLE)
	{
		if (nx > 0)
			ani = GIMMICK_ANI_IDLE_RIGHT;
		else
			ani = GIMMICK_ANI_IDLE_LEFT;
	} 
	else //if (state == GIMMICK_STATE_AUTO_GO)
	{
		if (key_down == 1)
			ani = GIMMICK_ANI_IDLE_RIGHT;
		else
			ani = GIMMICK_ANI_IDLE_LEFT;
	}

	int alpha = 255;
	if (untouchable) alpha = 128;

	animation_set->at(ani)->Render((int)x, (int)y, alpha);

	RenderBoundingBox();
}

void CGimmick::KeyState(BYTE* state)
{
	CGame* game = CGame::GetInstance();

	// disable control key when Mario die 
	if (GetState() == GIMMICK_STATE_DIE) return;

	if (game->IsKeyDown(DIK_RIGHT)) {

		key_down = 1;

		if (!isIncline) {

			SetState(GIMMICK_STATE_WALKING_RIGHT);
		}
		else if (isIncline) {

			if (direct_incline == GIMMICK_TREND_INCLINE_RIGHT)

				SetState(GIMMICK_STATE_INCLINE_UP);

			else
				SetState(GIMMICK_STATE_INCLINE_DOWN);
		}
	}
	else if (game->IsKeyDown(DIK_LEFT)) {

		key_down = -1;

		if (!isIncline) {

			SetState(GIMMICK_STATE_WALKING_LEFT);
		}

		else if (isIncline) {

			if (direct_incline == GIMMICK_TREND_INCLINE_LEFT)

				SetState(GIMMICK_STATE_INCLINE_UP);

			else
				SetState(GIMMICK_STATE_INCLINE_DOWN);
		}
	}
	else if (isIncline) {

		//gimmick->key_down = 0;

		SetState(GIMMICK_STATE_AUTO_GO);
	}
	else if (vy == 0 /*&& gimmick->vx != 0*/) {

		//gimmick->key_down = 0;

		SetState(GIMMICK_STATE_IDLE);
	}


	
}


void CGimmick::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case GIMMICK_STATE_WALKING_RIGHT:
		vx = GIMMICK_WALKING_SPEED;
		nx = 1;
		break;
	case GIMMICK_STATE_WALKING_LEFT:
		vx = -GIMMICK_WALKING_SPEED;
		nx = -1;
		break;
	case GIMMICK_STATE_JUMP:
		vy = GIMMICK_JUMP_SPEED_Y;
		jump = 1;
		break;
	case GIMMICK_STATE_HOLD_JUMP:
		vy = GIMMICK_JUMP_SPEED_Y;
		break;
	case GIMMICK_STATE_IDLE:
		vx = 0;
		break;
	case GIMMICK_STATE_DIE:
		vy = -GIMMICK_DIE_DEFLECT_SPEED;
		break;
	case GIMMICK_STATE_INCLINE_UP:
	{
		if (direct_go == 1)
		{
			if (incline_size == 1) {
				vx = GIMMICK_INCLINE_UP_SPEED_X_1;
				vy = GIMMICK_INCLINE_UP_SPEED_Y_1;
			}
			else {
				vx = GIMMICK_INCLINE_UP_SPEED_X_2;
				vy = GIMMICK_INCLINE_UP_SPEED_Y_2;
			}
		}
		else //if (direct_go == -1)
		{
			if (incline_size == 1) {
				vx = -GIMMICK_INCLINE_UP_SPEED_X_1;
				vy = GIMMICK_INCLINE_UP_SPEED_Y_1;
			}
			else {

				vx = -GIMMICK_INCLINE_UP_SPEED_X_2;
				vy = GIMMICK_INCLINE_UP_SPEED_Y_2;
			}
		}
	}
	break;

	case GIMMICK_STATE_INCLINE_DOWN:
	{
		if (direct_go == 1)
		{
			if (incline_size == 1) {

				vx = GIMMICK_INCLINE_DOWN_SPEED_X_1;
				vy = -GIMMICK_INCLINE_DOWN_SPEED_Y_1;
			}
			else {
				vx = GIMMICK_INCLINE_DOWN_SPEED_X_2;
				vy = -GIMMICK_INCLINE_DOWN_SPEED_Y_2;
			}
		}
		else if (direct_go == -1)
		{
			if (incline_size == 1) {

				vx = -GIMMICK_INCLINE_DOWN_SPEED_X_1;
				vy = -GIMMICK_INCLINE_DOWN_SPEED_Y_1;
			}
			else {

				vx = -GIMMICK_INCLINE_DOWN_SPEED_X_2;
				vy = -GIMMICK_INCLINE_DOWN_SPEED_Y_2;
			}
		}
	}
	break;

	case GIMMICK_STATE_AUTO_GO:
		if (isIncline) {
			vx = incline_vx;
			vy = incline_vy;
		}
		break;
	}

}

void CGimmick::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + GIMMICK_BIG_BBOX_WIDTH ;
	bottom = y - GIMMICK_BIG_BBOX_HEIGHT;
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CGimmick::Reset()
{
	SetState(GIMMICK_STATE_IDLE);
	SetPosition(950, 122);
	SetSpeed(0, 0);
}

