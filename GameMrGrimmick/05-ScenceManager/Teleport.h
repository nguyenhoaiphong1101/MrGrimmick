
#pragma once
#include "GameObject.h"


class CTeleport : public CGameObject
{
public:

	CTeleport(float x,float y);
	float teleX;
	float teleY;

	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};


