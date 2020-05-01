﻿#include <algorithm>
#include "Utils.h"

#include "Game.h"
#include "Portal.h"
#include "Brick.h"
#include "Simon.h"
#include "Item.h"
#include "StairBottom.h"
#include "StairTop.h"

CSimon::CSimon()
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(0));
	untouchable = 0;
	whip = new Whip();
	weapon = new Knife();
	subWeaponIsON = false;
	isHittingWhip = false;
	isDone = false;
	isTouchStair = false;
}

void CSimon::WalkLeft()
{
	if (isGrounded)
	{
	nx = -1;
	CSimon::SetState(SIMON_WALKING);
	}

}

void CSimon::WalkRight()
{
	if (isGrounded)
	{
	nx = 1;
	CSimon::SetState(SIMON_WALKING);
	}
	
}

void CSimon::Jump()
{
	if (GetState() == SIMON_STAND_HIT || GetState() == SIMON_SIT_HIT || GetState() == SIMON_SHOCK)
		return;
	if (isGrounded && !(state == SIMON_SIT)) 
	{
		isGrounded = false;
		ani = SIMON_JUMP;
		CSimon::SetState(SIMON_JUMP);
	}
	
}

void CSimon::Hit()
{
	whip->SetNx(nx);
	if (isHittingWhip == true)
		return;
	isHittingWhip = true;
	if (GetState() == SIMON_SHOCK)
		return;
	if ((GetState() == SIMON_IDLE || GetState() == SIMON_JUMP || GetState() == SIMON_WALKING))
	{
		if (isGrounded)
		{
			SetState(SIMON_STAND_HIT);
			vx = 0;
		}
		else
			SetState(SIMON_STAND_HIT);
	}
	else if (GetState() == SIMON_SIT)
		SetState(SIMON_SIT_HIT);

}

void CSimon::HitWeapon()
{
	if (GetState() == SIMON_SHOCK)
		return;
	if (!subWeaponIsON)
	{
		Hit();
		return;
	}
	if (weapon->isSubWeaponExist)
		return;
	Hit();
	weapon->SetNx(nx);
	weapon->isHittingSubWeapon = true;
	weapon->SetDirectionSubWeapon(nx);
}

void CSimon::WalkUpOnStair()
{
	nx = nxStair;
	SetState(SIMON_STAIR_UP);
}


void CSimon::WalkDownOnStair()
{

}

void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += GRAVITY*dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	//whip
	whip->SetHit(isHittingWhip);
	if (ani != SIMON_SIT_HIT)
	{
		whip->SetPosition(x - 90, y);
	}
	else
	{
		whip->SetPosition(x - 90, y + 15);
	}
	//
	if (weapon->isSubWeaponExist && subWeaponIsON)
	{
		weapon->Update(dt);
	}

	// turn off collision when die 
	
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size()==0)
	{
		x += dx;
		if(!isTouchStair)
			y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx*dx + nx*0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty*dy + ny*0.4f;
		
		if (nx!=0) vx = 0;
		if (ny!=0) vy = 0;

		// Collision logic with Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Brick *>(e->obj)) // if e->obj is Goomba 
			{
				if (e->ny < 0)
				{
					isGrounded = true;
				}
			}
			else if (dynamic_cast<CPortal *>(e->obj))
			{
				CPortal *p = dynamic_cast<CPortal *>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
				return;
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CSimon::Render()
{
	animation_set->at(ani)->Render(nx, x, y);
	if ((ani == SIMON_STAND_HIT || ani == SIMON_SIT_HIT) && !weapon->isHittingSubWeapon || (weapon->isHittingSubWeapon && !subWeaponIsON))
	{
		whip->Render(animation_set->at(ani)->GetCurrentFrame());
	}
	RenderBoundingBox();
	if (subWeaponIsON)
		weapon->Render();
	
	

	//RenderBoundingBox();
}

void CSimon::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case SIMON_WALKING:
		if (nx == 1)
			vx = SIMON_WALKING_SPEED;
		else
			vx = -SIMON_WALKING_SPEED;
		ani = SIMON_WALKING;
		break;
	case SIMON_JUMP:
		isGrounded = false;
		vy = -SIMON_JUMP_SPEED_Y;
		break;
	case SIMON_SIT:
		vx = 0;
		ani = SIMON_SIT;
		break;
	case SIMON_IDLE:
		vx = 0;
		ani = SIMON_IDLE;
		break;
	case SIMON_SHOCK:
		vx = 0;
		ani = SIMON_SHOCK;
		animation_set->at(state)->StartRenderAnimation();
		break;
	case SIMON_SIT_HIT:
		vx = 0;
		ani = SIMON_SIT_HIT;
		animation_set->at(ani)->SetCurrentFrame();
		animation_set->at(ani)->StartRenderAnimation();
		break;
	case SIMON_STAND_HIT:
		ani = SIMON_STAND_HIT;
		animation_set->at(ani)->SetCurrentFrame();
		animation_set->at(ani)->StartRenderAnimation();
		break;
	case SIMON_STAIR_UP:
		ani = SIMON_STAIR_UP;
		vx = SIMON_WALKING_SPEED;
		vy = SIMON_WALKING_UP;
		break;
	}
}

void CSimon::CollideWithItem(vector<LPGAMEOBJECT> *listItems)
{
	if (listItems->size() == 0)
		return;
	float left_a, top_a, right_a, bottom_a, left_b, top_b, right_b, bottom_b;
	GetBoundingBox(left_a, top_a, right_a, bottom_a);
	for (int i = 0; i < listItems->size(); i++)
	{
		LPGAMEOBJECT obj = listItems->at(i);
		Item* e = dynamic_cast<Item*>(obj);
		e->GetBoundingBox(left_b, top_b, right_b, bottom_b);
		if (AABBCollision(left_a, top_a, right_a, bottom_a, left_b, top_b, right_b, bottom_b))
		{
			if (e->GetState() == ITEM_UPGRADE_WHIP)
			{
				SetState(SIMON_SHOCK);
				e->isDone = true;
				if (whip->GetState() == WHIP_LVL_1)
					whip->SetState(WHIP_LVL_2);
				else if (whip->GetState() == WHIP_LVL_2)
					whip->SetState(WHIP_LVL_3);
			}
			else if (e->GetState() == ITEM_HEART)
			{
				e->isDone = true;
			}
			else if (e->GetState() == ITEM_KNIFE)
			{
				subWeaponIsON = true;
				e->isDone = true;
			}
			vector<LPGAMEOBJECT>::iterator it;
			it = listItems->begin();
			listItems->erase(it);
		}
	}
}

bool CSimon::CollideWithPortal(vector<LPGAMEOBJECT>* portal)
{
	if (portal->size() == 0)
		return false;
	LPGAMEOBJECT obj = portal->at(0);
	CPortal* e = dynamic_cast<CPortal*>(obj);
	float left_a, top_a, right_a, bottom_a, left_b, top_b, right_b, bottom_b;
	GetBoundingBox(left_a, top_a, right_a, bottom_a);
	e->GetBoundingBox(left_b, top_b, right_b, bottom_b);
	if (AABBCollision(left_a, top_a, right_a, bottom_a, left_b, top_b, right_b, bottom_b))
	{
		return true;
	}
	return false;
}

void CSimon::TouchStair(vector<LPGAMEOBJECT>* stair)
{
	float left_a, top_a, right_a, bottom_a, left_b, top_b, right_b, bottom_b;
	GetBoundingBox(left_a, top_a, right_a, bottom_a);
	for (int i = 0; i < stair->size(); i++)
	{
		LPGAMEOBJECT obj = stair->at(i);
		
		obj->GetBoundingBox(left_b, top_b, right_b, bottom_b);
		if(dynamic_cast<StairBottom*>(obj))
		{	
			if (AABBCollision(left_a, top_a, right_a, bottom_a, left_b, top_b, right_b, bottom_b))
			{

				isTouchStair = true;
				nxStair = obj->nx;
			}
			else
				isTouchStair = false;
		}
		else
		{

		
		}
	}
}

void CSimon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x + 15;
	top = y; 
	right = x + SIMON_BOX_WIDTH + 15;
	bottom = y + SIMON_BOX_HEIGHT;
}

