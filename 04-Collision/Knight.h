#pragma once
#include "GameObject.h"

#define	KNIGHT_IS_HIT	1
#define KNIGHT_WALK		0
#define KNIGHT_DEAD		2

#define KNIGHT_ANI		0

#define KNIGHT_SPEED_WALK 0.1f

class Knight : public CGameObject
{
	bool isTurning;
	int hp;
	DWORD timeDelay;
public:
	void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	void Render();
	void SetState(int state);
	int GetHP() { return hp; }
	void SetHP(int HP) { hp = HP; }
	Knight();
	~Knight();
};

