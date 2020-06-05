#pragma once
#include "Enemy.h"

#define GHOST_SPEED 0.085f
#define VY_GHOST	0.022f;

class Ghost : public Enemy
{
	
public:
	void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL, LPGAMEOBJECT simon = NULL);
	void Render();
	void SetState(int state);
	void ChasingSimon(int x, int y);
	int GetHP() { return hp; }
	void SetHP(int HP) { hp = HP; }
	Ghost();
	~Ghost();
};

