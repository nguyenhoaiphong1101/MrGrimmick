
#include "Fish.h"
#include "Utils.h"
Fish::Fish(int type)
{
    //SetState(FISH_STATE_SWIMMING_LEFT);
    if (type == 1)
    {
        nx = 1;
        vx = FISH_SWIMMING_SPEED;
    }
    else
    {
        nx = -1;
        vx = -FISH_SWIMMING_SPEED;
    }
    vy = 0;
    StartSwimming();
}


void Fish::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = top = bottom = right = 0;
}

void Fish::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    CGameObject::Update(dt, coObjects);

    /*vector<LPCOLLISIONEVENT> coEvents;
    vector<LPCOLLISIONEVENT> coEventsResult;

     coEvents.clear();*/

    if (GetTickCount() - swimming_start > FISH_SWIMMING_TIME)
    {
        nx = -nx;
        vx = -vx;
        swimming_start = GetTickCount();
    }
    x += dx;

}

void Fish::Render()
{

    /*if (state == FISH_STATE_SWIMMING_LEFT)
    {
    {
    switch (Type)
    {
    case FISH_TYPE_YELLOW:
    ani = FISH_YELLOW_ANI_SWIMMING_LEFT;
    break;
    case FISH_TYPE_BLACK:
    ani = FISH_BLACK_ANI_SWIMMING_LEFT;
    break;
    case FISH_TYPE_RED:
    ani = FISH_RED_ANI_SWIMMING_LEFT;
    break;
    }
    }
    }*/
    int ani = FISH_ANI_LEFT;
    if (nx < 0)
    {
        ani = FISH_ANI_LEFT;
    }
    else
    {
        ani = FISH_ANI_RIGHT;
    }

    animation_set->at(ani)->Render(x, y);

    //RenderBoundingBox();
}

void Fish::SetState(int state)
{
    CGameObject::SetState(state);
    /*switch (state)
    {
    case FISH_STATE_SWIMMING_RIGHT:

     break;
    case FISH_STATE_SWIMMING_LEFT:
    break;
    }*/
}