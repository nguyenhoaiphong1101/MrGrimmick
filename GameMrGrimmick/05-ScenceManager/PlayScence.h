#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Gimmick.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Map.h"
#include "Grid.h"
#include "Quadtree.h"
#include "Cannon.h"
#include "FloatBox.h"
#include "Item.h"
#include "NoCollisionObject.h"
#include "Fish.h"
#include "Slide.h"



class CPlayScene: public CScene
{
protected: 
	Map* map;
	CGimmick* player;					// A play scene has to have player, right? 
	vector<LPGAMEOBJECT> objects;

	

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP(string line);

	
public: 
	Quadtree* quadtree;
	CPlayScene(int id, LPCWSTR filePath);


	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	void SetCamPos();

	CGimmick* GetPlayer() { return player; }

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

