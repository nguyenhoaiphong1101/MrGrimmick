#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Gimmick.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Turtle.h"
#include "BulletBigCannon.h"
#include "Map.h"
#include "MovingBrick.h"
#include "Grid.h"
#include "BlackBird.h"
#include "Quadtree.h"
#include "Cannon.h"
#include "FloatBox.h"
#include "Item.h"
#include "Boat.h"
#include "NoCollisionObject.h"
#include "Fish.h"
#include "Slide.h"
#include "BlackEnemy.h"
#include "BoomBoss.h"
#include "Electricboom.h"
#include "Worm.h"
#include "Hud.h"
#include "Teleport.h"
#include "BigCannon.h"
#include "Window.h"
#include "SuspensionBridge.h"
#include "Rocket.h"
#include "AniBrick.h"
#include "Bullet.h"
#include "Thunder.h"
#include "Star.h"
#include "Pipes.h"
#include "Incline.h"

class CPlayScene: public CScene
{
protected: 
	Map* map;
	CGimmick* player;				// A play scene has to have player, right? 
	Star* star = nullptr;
	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> objectsNoColliMove;
	CHud* hud = new CHud();
	

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP(string line);

	
public: 
	Quadtree* quadtree;
	CPlayScene(int id, LPCWSTR filePath);

	vector<LPGAMEOBJECT> get_objects() { return objects; };
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	void SetCamPos();

	CGimmick* GetPlayer() { return player; }
	Quadtree* GetQuadtree() { return quadtree; }
	void addObject(LPGAMEOBJECT a) { objects.push_back(a); }
	Star* GetStar() { return star; };

	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public: 
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};

