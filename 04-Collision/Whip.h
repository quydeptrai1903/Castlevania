#pragma once
#include "GameObject.h"
#include "Enemy.h"
#include "HitEffect.h"

#define WHIP_LVL_1		0
#define WHIP_LVL_2		1
#define WHIP_LVL_3		2
#define ANIMATION_WHIP	2
#define TIME_RESET_HIT	200

#define DAME_WHIP_LVL1	1
#define DAME_WHIP_LVL23	2

class Whip : public CGameObject
{
public:
	vector<LPHITEFFECT> hitEffect;
	int dame;
	DWORD timeResetHit;
	bool isResetHit;
public:
	int CurrentFrame;
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	
	void Render(int currentID = -1);
	void CollideWithSecretObj(vector<LPGAMEOBJECT>* coObjects);
	void CollideWithSecretEnemies(vector<LPENEMY>* coObjects);
	void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	Whip();
	~Whip();
};

