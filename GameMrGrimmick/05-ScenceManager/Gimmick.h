	#pragma once
#include "GameObject.h"

#define GIMMICK_WALKING_SPEED		0.08f 
#define GIMMICK_WALKING_SPEED_SLIDE_TRUE		0.1f 
#define GIMMICK_WALKING_SPEED_SLIDE_FALSE		0.05f 

#define GIMMICK_JUMP_SPEED_Y		0.2f
#define GIMMICK_JUMP_STATE_SPEED_Y		0.00001f
#define GIMMICK_DOUBLE_JUMP_SPEED	0.245f
#define GIMMICK_JUMP_DEFLECT_SPEED 0.2f
#define GIMMICK_GRAVITY			0.002f
#define GIMMICK_DIE_DEFLECT_SPEED	 0.5f
#define GIMMICK_INCLINE_UP_SPEED_X_1		0.05f
#define GIMMICK_INCLINE_UP_SPEED_X_2		0.075f
#define GIMMICK_INCLINE_DOWN_SPEED_X_1		0.125f
#define GIMMICK_INCLINE_DOWN_SPEED_X_2		0.1f

#define GIMMICK_INCLINE_UP_SPEED_Y_1		0.065
#define GIMMICK_INCLINE_UP_SPEED_Y_2		0.04f
#define GIMMICK_INCLINE_DOWN_SPEED_Y_1		0.125f
#define GIMMICK_INCLINE_DOWN_SPEED_Y_2		0.05f

#define GIMMICK_STATE_IDLE			0
#define GIMMICK_STATE_WALKING_RIGHT	100
#define GIMMICK_STATE_WALKING_LEFT	200
#define GIMMICK_STATE_JUMP			300
#define GIMMICK_STATE_DIE				400
#define GIMMICK_STATE_HOLD_JUMP				500
#define GIMMICK_STATE_INCLINE_DOWN	600
#define GIMMICK_STATE_INCLINE_UP		700
#define GIMMICK_STATE_AUTO_GO		1000
#define GIMMICK_STATE_AUTO_GO_SLIDE_RIGHT		1001
#define GIMMICK_STATE_AUTO_GO_SLIDE_LEFT		1002


#define GIMMICK_ANI_IDLE_RIGHT		1
#define GIMMICK_ANI_IDLE_LEFT			0

#define GIMMICK_ANI_WALKING_LEFT			2
#define GIMMICK_ANI_WALKING_RIGHT			3
#define GIMMICK_ANI_JUMPING_RIGHT		4
#define GIMMICK_ANI_JUMPING_LEFT		5

#define GIMMICK_TREND_INCLINE_RIGHT	1
#define GIMMICK_TREND_INCLINE_LEFT	-1



#define GIMMICK_BIG_BBOX_WIDTH  16
#define GIMMICK_BIG_BBOX_HEIGHT 19



#define GIMMICK_UNTOUCHABLE_TIME 5000


class CGimmick : public CGameObject
{

public: 
	int jump = 0;

	int untouchable;
	DWORD untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y;
	bool isOnBridge = false;
public:
	//Incline
	bool isIncline = false;				// check for sliding
	int incline_size;						// size of slide
	int direct_incline;
	int direct_go;						// trend of slide
	int key_down = 0;

	// pipe
	bool isPiping = false;		// checking gimmick is piping
	float pipeVx, pipeVy;		// set speed when piping


	// silde
	bool isSlide = false;
	int isDirSlide = 0;
	int slideType = 0;

	float incline_vx, incline_vy;


	//
	int startJump = -1;
	int holdJump = 0;


public:


	CGimmick(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	int GetJumping() { return jump; };
	void SetState(int state);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	float isOnTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float x, float y);
	void SetJumping(int jump) { this->jump = jump; };
	void SetOnBridge(bool isOnBridge) { this->isOnBridge = isOnBridge; };
	void Reset();
	void KeyState(BYTE* state);
	void OnKeyDown(int keyCode);
	void OnKeyUp(int keyCode);

};