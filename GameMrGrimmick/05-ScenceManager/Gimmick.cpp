﻿#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Gimmick.h"
#include "Incline.h"
#include "Game.h"
#include "PlayScence.h"

#include "Portal.h"
#include "Slide.h"

void CGimmick::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<Star*>(coObjects->at(i)))
		{
			continue;
		}
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

CGimmick::CGimmick(float x, float y) : CGameObject()
{
	CGame* game = CGame::GetInstance();

	untouchable = 0;
	SetState(GIMMICK_STATE_IDLE);

	start_x = x;
	start_y = y;
	if (game->isSwitchScene) {
		this->x = game->playerX;
		this->y = game->playerY;
		game->isSwitchScene = false;
	}
	else {
		this->x = x;
		this->y = y;
	}
}

void CGimmick::FollowObject(LPGAMEOBJECT obj)
{
	vx = obj->GetVx();
	//x = obj->GetX();
	if (!dynamic_cast<SuspensionBridge*>(obj))
	{
		y = obj->GetY() + GIMMICK_BIG_BBOX_HEIGHT + 0.4f;
	}
}
	
void CGimmick::FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy)
{

	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	bool check_rec = false;
	bool check_brickbroken = false;

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t; nx = c->nx; min_ix = i; rdx = c->dx;
		}

		if (c->t < min_ty && c->ny != 0) {
			min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
		}
		if (dynamic_cast<BlackEnemy*>(c->obj))
		{
			if(c->ny<0)
			ny = 0;
		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}

void CGimmick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	int ids = CGame::GetInstance()->GetCurrentScene()->GetId();

	if (GetState() != GIMMICK_STATE_IDLE) {

		isFollow = false;
	}

	if (isFollow) {

		if (dynamic_cast<SuspensionBridge*>(obj))
		{
			SuspensionBridge* bridge = dynamic_cast<SuspensionBridge*>(obj);
			if (bridge->isOpening)
			{
				isFollow = false;
				isOnBridge = false;
			}
			else
			{
				FollowObject(obj);
			}
		}
		else {
			FollowObject(obj);
		}
	}
	else {
		obj = NULL;
	}
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	if (holdJump != 1 && !isIncline && !isPiping && !isFollow && state!=GIMMICK_STATE_DIE)
		vy -= GIMMICK_GRAVITY * dt;


	if (holdJump == 1)
	{
		if (abs(y - startJump) <= 50)
			SetState(GIMMICK_STATE_HOLD_JUMP);
		else
		{
			holdJump = 0;
			startJump = -1;
		}
	}


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;


	coEvents.clear();

	// turn off collision when die 
	if (state != GIMMICK_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > GIMMICK_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}



	// No collision occured, proceed normally
	if (coEvents.size() == 0 && !isFollow)
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

			if (e->ny != 0)
			{
				holdJump = 0;
				jump = 0;
			}
			if (dynamic_cast<BlackEnemy*>(e->obj)) {

				BlackEnemy* be = dynamic_cast<BlackEnemy*>(e->obj);

				if (e->t > 0 && e->t <= 1)
				{
					if (e->ny > 0) {
						isFollow = true;
						obj = be;
					}
					if (e->nx != 0)
					{
						callDeclineLight();
					}
				}
			}
			if (dynamic_cast<Worm*>(e->obj)) {

				Worm* be = dynamic_cast<Worm*>(e->obj);

				if (e->t > 0 && e->t <= 1)
				{
					callDeclineLight();
				}
			}
			if (dynamic_cast<Rocket*>(e->obj)) {

				isMoveCol = true;
				callDeclineLight();
			}
			if (dynamic_cast<Bullet*>(e->obj)) {

				isMoveCol = true;
				callDeclineLight();
			}
			if (dynamic_cast<Item*>(e->obj))
			{
				Item* item = dynamic_cast<Item*>(e->obj);
				GetItem(item->GetType());
				if (ids == 3)
				{
					if (item->GetType() == ITEM_TYPE_MEDICINE_PINK_BOMB)
					{
						CGame::GetInstance()->SetCamPos(0, 170);
						CGame::GetInstance()->SwitchScene(13);
						return;
					}
				}
				item->SetState(ITEM_STATE_DISAPPEAR);

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
				else if ((CGame::GetInstance()->IsKeyDown(DIK_LEFT))
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
			if (dynamic_cast<CPipes*>(e->obj)) {

				CPipes* pipe = dynamic_cast<CPipes*>(e->obj);
				isPiping = true;
				x = pipe->x;
				if (vy > 0) y += 0.1f;
				else if (vy < 0)y -= 0.1f;
			}
			else {
				isPiping = false;
			}
			if (dynamic_cast<Slide*>(e->obj))
			{
				isSlide = true;
				Slide* slide = dynamic_cast<Slide*>(e->obj);
				if (slide->GetType() == SLIDE_TYPE_LEFT)
				{
					slideType = -1;
				}
				else if (slide->GetType() == SLIDE_TYPE_RIGHT)
				{
					slideType = 1;
				}
			}
			else
			{
				isSlide = false;
			}
			if (dynamic_cast<CThunder*>(e->obj))
			{
				CThunder* thunder = dynamic_cast<CThunder*>(e->obj);
				this->SetState(GIMMICK_STATE_DIE);

			}
			if (dynamic_cast<SuspensionBridge*>(e->obj))
			{
				
				SuspensionBridge* bridge = dynamic_cast<SuspensionBridge*>(e->obj);
				if (!bridge->isOpening)
				{
					isOnBridge = true;
					isFollow = true;
					obj = bridge;
				}
				if (bridge->GetState() != BRIDGE_STATE_MOVING && !bridge->GetIsOpening())
				{
					bridge->SetState(BRIDGE_STATE_MOVING);

					//DebugOut(L"[INFO] Vô đây hoài: \n");
				}
				/*	else
					{
						isOnBridge = false;
					}*/
					/*this->x += bridge->dt * BRIDGE_MOVING_SPEED;*/

			}
			else
			{
				isOnBridge = false;
			}
			if (dynamic_cast<CMovingBrick*>(e->obj)) {

				CMovingBrick* mb = dynamic_cast<CMovingBrick*>(e->obj);

				if (e->t > 0 && e->t <= 1)

					if (e->ny > 0) {

						isFollow = true;
						obj = mb;
					}

			}
			if (dynamic_cast<CPortal*>(e->obj)) {
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->playerX = p->getOldX();
				CGame::GetInstance()->playerY = p->getOldY();
				CGame::GetInstance()->isSwitchScene = true;
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
				return;
			}
		}
		if (!isIncline && !isPiping && !isSlide) {

			if (isMoveCol)
			{
				isMoveCol = false;
			}
			else
			{
				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.4f;
			}
			if (nx != 0) vx = 0;
			if (ny != 0) vy = 0;
		}
		else {
			x += dx;
			if (isIncline || isSlide) {
				y += min_ty * dy + ny * 0.4f;
			}
			else if (isPiping)
			{
				y += dy;
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
	if (untouchable == 1)
	{
		if (nx > 0)
			ani = GIMMICK_ANI_COLLISION_RIGHT;
		else
			ani = GIMMICK_ANI_COLLISION_LEFT;
	}
	else if (jump == 1)
	{
		if (nx > 0)
			ani = GIMMICK_ANI_JUMPING_RIGHT;
		else
			ani = GIMMICK_ANI_JUMPING_LEFT;
	}
	else if (state == GIMMICK_STATE_WALKING_RIGHT)
	{
		ani = GIMMICK_ANI_WALKING_RIGHT;
	}
	else if (state == GIMMICK_STATE_WALKING_LEFT)
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
	else if (state == GIMMICK_STATE_AUTO_GO_SLIDE_RIGHT)
	{
		ani = GIMMICK_ANI_WALKING_RIGHT;
	}
	else if (state == GIMMICK_STATE_AUTO_GO_SLIDE_LEFT)
	{
		ani = GIMMICK_ANI_WALKING_LEFT;
	}
	else if (state == GIMMICK_STATE_DIE)
	{
		createDieEffect();
		return;
	}
	else //if (state == GIMMICK_STATE_AUTO_GO)
	{
		if (key_down == 1)
			ani = GIMMICK_ANI_IDLE_RIGHT;
		else
			ani = GIMMICK_ANI_IDLE_LEFT;
	}

	int alpha = 255;

	animation_set->at(ani)->Render((int)x, (int)y, alpha);

	RenderBoundingBox();
}

void CGimmick::callDeclineLight()
{
	if (untouchable == 0)
	{
		if (CGame::GetInstance()->GetLight() == 1)
		{
			this->SetState(GIMMICK_STATE_DIE);
			CGame::GetInstance()->IncLight(-1);
		}
		else
		{
			// stun???
			y += 7;
			CGame::GetInstance()->IncLight(-1);
			StartUntouchable();
		}
	}
}

void CGimmick::KeyState(BYTE* state)
{
	CGame* game = CGame::GetInstance();

	// disable control key when Mario die 
	if (GetState() == GIMMICK_STATE_DIE) return;

	if (!isPiping)
	{
		if (game->IsKeyDown(DIK_RIGHT)) {

			key_down = 1;

			if (!isIncline && !isSlide) {
				SetState(GIMMICK_STATE_WALKING_RIGHT);
			}
			else if (isIncline) {

				if (direct_incline == GIMMICK_TREND_INCLINE_RIGHT)

					SetState(GIMMICK_STATE_INCLINE_UP);

				else
					SetState(GIMMICK_STATE_INCLINE_DOWN);
			}
			else if (isSlide)
			{
				SetState(GIMMICK_STATE_AUTO_GO_SLIDE_RIGHT);
			}
		}
		else if (game->IsKeyDown(DIK_LEFT)) {

			key_down = -1;

			if (!isIncline && !isSlide) {

				SetState(GIMMICK_STATE_WALKING_LEFT);
			}

			else if (isIncline) {

				if (direct_incline == GIMMICK_TREND_INCLINE_LEFT)

					SetState(GIMMICK_STATE_INCLINE_UP);

				else
					SetState(GIMMICK_STATE_INCLINE_DOWN);
			} if (isSlide)
			{
				SetState(GIMMICK_STATE_AUTO_GO_SLIDE_LEFT);
			}
		}
		else if (isIncline || isSlide) {
			//gimmick->key_down = 0;
			SetState(GIMMICK_STATE_AUTO_GO);
		}
		else if (vy == 0 /*&& gimmick->vx != 0*/) {

			//gimmick->key_down = 0;

			SetState(GIMMICK_STATE_IDLE);
		}
	}

}


void CGimmick::SetState(int state)
{
	CGame* game = CGame::GetInstance();
	CGameObject::SetState(state);

	switch (state)
	{
	case GIMMICK_STATE_WALKING_RIGHT:
		vx = GIMMICK_WALKING_SPEED;
		nx = 1;
		break;
	case GIMMICK_STATE_AUTO_GO_SLIDE_RIGHT:
		if (slideType == 1)
		{
			vx = GIMMICK_WALKING_SPEED_SLIDE_TRUE;
		}
		else
		{
			vx = GIMMICK_WALKING_SPEED_SLIDE_FALSE;
		}
		nx = 1;
		break;
	case GIMMICK_STATE_AUTO_GO_SLIDE_LEFT:
		if (slideType == -1)
		{
			vx = -GIMMICK_WALKING_SPEED_SLIDE_TRUE;
		}
		else
		{
			vx = -GIMMICK_WALKING_SPEED_SLIDE_FALSE;
		}
		nx = -1;
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
		this->isDeath = true;
		this->vx = 0;
		this->vy = 0;
		positionX = this->x;
		positionY = this->y;
		if (game->GetRest() > 0) game->IncRest(-1);
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
		else if (isSlide)
		{
			if (slideType == 1)
			{
				vx = GIMMICK_WALKING_SPEED;
			}
			else
			{
				vx = -GIMMICK_WALKING_SPEED;
			}
		}
		break;
	}

}


void CGimmick::createDieEffect() {
	deltaTimeDie += 10;
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	animation_sets->Get(80)->at(0)->Render(positionX, positionY + GIMMICKDIEEFFECT_SPEED * deltaTimeDie);
	animation_sets->Get(80)->at(0)->Render(positionX + GIMMICKDIEEFFECT_SPEED_675 * deltaTimeDie, positionY + GIMMICKDIEEFFECT_SPEED_225 * deltaTimeDie);
	animation_sets->Get(80)->at(0)->Render(positionX + GIMMICKDIEEFFECT_SPEED_450 * deltaTimeDie, positionY + GIMMICKDIEEFFECT_SPEED_450 * deltaTimeDie);
	animation_sets->Get(80)->at(0)->Render(positionX + GIMMICKDIEEFFECT_SPEED_225 * deltaTimeDie, positionY + GIMMICKDIEEFFECT_SPEED_675 * deltaTimeDie);
	animation_sets->Get(80)->at(0)->Render(positionX + GIMMICKDIEEFFECT_SPEED * deltaTimeDie, positionY);
	animation_sets->Get(80)->at(0)->Render(positionX + GIMMICKDIEEFFECT_SPEED_225 * deltaTimeDie, positionY + -GIMMICKDIEEFFECT_SPEED_675 * deltaTimeDie);
	animation_sets->Get(80)->at(0)->Render(positionX + GIMMICKDIEEFFECT_SPEED_450 * deltaTimeDie, positionY + -GIMMICKDIEEFFECT_SPEED_450 * deltaTimeDie);
	animation_sets->Get(80)->at(0)->Render(positionX + GIMMICKDIEEFFECT_SPEED_675 * deltaTimeDie, positionY + -GIMMICKDIEEFFECT_SPEED_225 * deltaTimeDie);
	animation_sets->Get(80)->at(0)->Render(positionX, positionY + -GIMMICKDIEEFFECT_SPEED * deltaTimeDie);
	animation_sets->Get(80)->at(0)->Render(positionX + -GIMMICKDIEEFFECT_SPEED_675 * deltaTimeDie, positionY + -GIMMICKDIEEFFECT_SPEED_225 * deltaTimeDie);
	animation_sets->Get(80)->at(0)->Render(positionX + -GIMMICKDIEEFFECT_SPEED_450 * deltaTimeDie, positionY + -GIMMICKDIEEFFECT_SPEED_450 * deltaTimeDie);
	animation_sets->Get(80)->at(0)->Render(positionX + -GIMMICKDIEEFFECT_SPEED_225 * deltaTimeDie, positionY + -GIMMICKDIEEFFECT_SPEED_675 * deltaTimeDie);
	animation_sets->Get(80)->at(0)->Render(positionX + -GIMMICKDIEEFFECT_SPEED * deltaTimeDie, positionY);
	animation_sets->Get(80)->at(0)->Render(positionX + -GIMMICKDIEEFFECT_SPEED_225 * deltaTimeDie, positionY + GIMMICKDIEEFFECT_SPEED_675 * deltaTimeDie);
	animation_sets->Get(80)->at(0)->Render(positionX + -GIMMICKDIEEFFECT_SPEED_450 * deltaTimeDie, positionY + GIMMICKDIEEFFECT_SPEED_450 * deltaTimeDie);
	animation_sets->Get(80)->at(0)->Render(positionX + -GIMMICKDIEEFFECT_SPEED_675 * deltaTimeDie, positionY + GIMMICKDIEEFFECT_SPEED_225 * deltaTimeDie);
}


void CGimmick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + GIMMICK_BIG_BBOX_WIDTH;
	if (untouchable == 1)
	{
		bottom = y - 25;
	}else
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
	CGame::GetInstance()->SetLight(4);
}

void CGimmick::GetItem(int type)
{
	CGame* game = CGame::GetInstance();
	vector <int> itemlist = game->GetItem();
	if (type == ITEM_TYPE_MEDICINE_PINK || type == ITEM_TYPE_MEDICINE_PINK_BOMB || type == ITEM_TYPE_MEDICINE_BLACK_BOMB)
	{
		//cộng điểm 
		game->IncScore(720);
		if (type == ITEM_TYPE_MEDICINE_PINK)
		{
			game->IncLight(1);
		}
		// neu khong co san trong itemlist thì thêm 
		for each (int item in itemlist)
		{
			if (item == type)
				return; // khoi them nua 
		}
		itemlist.push_back(type);
		game->SetItem(itemlist);

	}
	else if (type == ITEM_TYPE_MEDICINE_ORANGE)
	{
		// tăng mạng
		game->IncLight(2);
	}
	else if (type == ITEM_TYPE_FLOWER)
	{
		game->IncScore(50000);
		game->IncRest(2);
	}

}