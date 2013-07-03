#pragma	once
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include <iostream>
#include "input.h"
#include "linkedlist.h"
#include "texlist.h"
#include <sstream>
using namespace std;

extern double toDegrees(double radians);
extern double toRadians(double degrees);
extern const int SCREENW;
extern const int SCREENH;

#ifdef	_WIN32
typedef	LPDIRECT3DTEXTURE9  TexPtr;
typedef	LPDIRECT3DDEVICE9	DevPtr;
#else
typedef void*	TexPtr;
typedef void*	DevPtr;
#endif

extern enum objectType {
#define NULL	0
#define POWERUP 1
#define STATIC  2
};

extern enum objectPowerUps {
#define NULL	0
#define HEALTH	1
#define AMMO	2
};

extern enum objectRenderType {
#define NULL 0
#define TICK 1
#define POSITION 2
};

extern enum objectStatic
{
#define NULL 0
#define ROCK 1
};

// The Structure for single Objects contains all information including file names,
// Textures, types and information about the object.
struct objectElement {

	int objectNumber;
	bool inPlay;
	int type;
	int power;
	int renderType;
	int staticObject;
	bool render;

	string fileElement;
	LPDIRECT3DTEXTURE9 texElement;
	LPD3DXSPRITE spriteElement;
	D3DCOLOR colorElement;

	double currentTick;
	double aliveTime;
	double enterTick;
	double removeTick;
	double remainingTick;

	POINT globalStart;


	//Borders
	float stageWidth;
	float stageHeight;

	int frame, columns;
	int width, height;
	float scaling, rotation;
	int startframe, endframe;
	int starttime, delay;

	double velx, vely;


	//visbility	
	RECT target;
	float visRamp;		
	int visTime;		
	int visDelay;		
	float opacity;

	bool visible;
	double x,y;

	bool canMove;
	bool alive;

	// timers

};

class Objects {

	int level;

	// Device
	DevPtr objectDevice;

	// Array of Object Structures (Textures / File Names / Sprite Objects / Render Information)
	objectElement object[256];
	int objectsInLevel;
	int objectsUsed;
	int nextObject;


	D3DCOLOR color;

	POINT globalPosition;

	double playerX;
	double playerVelocity;



public:
	Objects(int inLevel, DevPtr inDevice);
	~Objects();

	// Add Objects
	void addObjectByTimer(  int inObjectType, int inPowerUpType, int inStatic, int tickIn, int ticksAlive, double posY);
	void addObjectByPosition( int inObjectType, int inPowerUpType, int inStatic, int inStartX, int inStartY);

	// Rendering
	void drawAll();
	void Sprite_Transform_Draw(objectElement inElement);
	void Sprite_Transform_DrawAll();

	// Converters
	string convertInt(int in);

	// Getters
	objectElement *getObjectElement(int inObjectNumber){return &object[inObjectNumber];};
	objectElement getNextObjectElement(){if (objectsUsed < 256) return object[objectsUsed++];};
	string getFile(objectElement inObject);
	string getAttributes(objectElement inObject, int inLevel);
	LPDIRECT3DTEXTURE9 getObjectTex(int inObjectNumber){return object[inObjectNumber].texElement;};
	int getLevel(){return level;};
	int getObjectsUsed(){return objectsUsed;};
	bool getRender(int objNumber){return object[objNumber].render;};

	//Updates
	void updateObject(bool inMove, double inPlayerPositionX, double inPlayerVelocity);
	void move();
	void move(int inObject);
	void checkRender();
};

class objectManager {

private:
	DevPtr ManagerDevice;
	int currentLevel;
	int globalPosition;

public:
	LPDIRECT3DDEVICE9 d3ddev;
	objectManager(LPDIRECT3DDEVICE9 d3ddev);
	virtual ~objectManager();

	void AddObjectTick(int inLevel, int inType, int inPowerUp, int inStatic, int tickIn, int ticksAlive, int inY);
	void AddObjectPosition(int inLevel, int inType, int inPowerUp, int inStatic, int inStartX, int inStartY);

	void drawObjects(int inLevel);
	void AddLevel(int inLevel);
	void update(bool inMove, int inLevel, double inX, double inPlayerVelocity);
	void collisionCheck();

	Objects getLevel(int inLevel);

	LinkedList < Objects > *m_Object; 
};

