#include "Nocollision.h"

void CNocollision::Render()
{
	animation_set->at(0)->Render(x, y);
	////RenderBoundingBox();
}

void CNocollision::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b = 0;
}