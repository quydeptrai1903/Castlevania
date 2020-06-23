#pragma once
#include "SubWeapon.h"

#define GRAVITY 0.002
#define VX_AXE	0.22
#define AXE_BBOX_WIDTH_WIDTH	32
#define AXE_ANIMATION_SET		17

class Axe : public SubWeapon
{

public:
	void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	void Render();
	void SetDirectionSubWeapon(int nx);
	//bool CheckExistKnife(float a);
	void TimeStart();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	Axe();
	~Axe();
};
