#include "GimmickDieEffect.h"'

GimmickDieEffect::GimmickDieEffect(int temp)
{
	switch (temp)
	{
	case 0:
		vx = 0;
		vy = GIMMICKDIEEFFECT_SPEED;
		break;
	case 1:
		vx = GIMMICKDIEEFFECT_SPEED_675;
		vy = GIMMICKDIEEFFECT_SPEED_225;
		break;
	case 2:
		vx = GIMMICKDIEEFFECT_SPEED_450;
		vy = GIMMICKDIEEFFECT_SPEED_450;
		break;
	case 3:
		vx = GIMMICKDIEEFFECT_SPEED_225;
		vy = GIMMICKDIEEFFECT_SPEED_675;
		break;
	case 4:
		vx = GIMMICKDIEEFFECT_SPEED;
		vy = 0;
		break;
	case 5:
		vx = GIMMICKDIEEFFECT_SPEED_225;
		vy = -GIMMICKDIEEFFECT_SPEED_675;
		break;
	case 6:
		vx = GIMMICKDIEEFFECT_SPEED_450;
		vy = -GIMMICKDIEEFFECT_SPEED_450;
		break;
	case 7:
		vx = GIMMICKDIEEFFECT_SPEED_675;
		vy = -GIMMICKDIEEFFECT_SPEED_225;
		break;
	case 8:
		vx = 0;
		vy = -GIMMICKDIEEFFECT_SPEED;
		break;
	case 9:
		vx = -GIMMICKDIEEFFECT_SPEED_675;
		vy = -GIMMICKDIEEFFECT_SPEED_225;
		break;
	case 10:
		vx = -GIMMICKDIEEFFECT_SPEED_450;
		vy = -GIMMICKDIEEFFECT_SPEED_450;
		break;
	case 11:
		vx = -GIMMICKDIEEFFECT_SPEED_225;
		vy = -GIMMICKDIEEFFECT_SPEED_675;
		break;
	case 12:
		vx = -GIMMICKDIEEFFECT_SPEED;
		vy = 0;
		break;
	case 13:
		vx = -GIMMICKDIEEFFECT_SPEED_225;
		vy = GIMMICKDIEEFFECT_SPEED_675;
		break;
	case 14:
		vx = -GIMMICKDIEEFFECT_SPEED_450;
		vy = GIMMICKDIEEFFECT_SPEED_450;
		break;
	case 15:
		vx = -GIMMICKDIEEFFECT_SPEED_675;
		vy = GIMMICKDIEEFFECT_SPEED_225;
		break;
	default:
		break;
	}
}

void GimmickDieEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	/*if (!CGame::GetInstance()->InCamera(this) && CGame::GetInstance()->InLargeCamera(this) && this->visible == true) {
		this->visible = false;
	}
	if (!visible)
		return;*/
	// Calculate dx, dy 
	CGameObject::Update(dt);

	x += dx;
	y += dy;
}

void GimmickDieEffect::Render()
{
	int ani = GIMMICKDIEEFFECT_ANI;

	animation_set->at(ani)->Render(x, y);
}

void GimmickDieEffect::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + GIMMICKDIEEFFECT_BBOX_WIDTH;
	bottom = y - GIMMICKDIEEFFECT_BBOX_HEIGHT;
}

void GimmickDieEffect::SetState(int state) 
{
}