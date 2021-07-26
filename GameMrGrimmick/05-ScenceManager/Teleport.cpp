#include "Teleport.h"


CTeleport::CTeleport(float x,float y)
{
	teleX = x;
	teleY = y;
}

void CTeleport::Render()
{

}

void CTeleport::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + 16;
	b = y - 16;
}

