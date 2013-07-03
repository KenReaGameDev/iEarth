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
//extern const double PI = 3.1415926535;
//extern const double PI_under_180 = 180.0f / PI;
//extern const double PI_over180 = PI / 180.0f;

#ifdef	_WIN32
typedef	LPDIRECT3DTEXTURE9  TexPtr;
typedef	LPDIRECT3DDEVICE9	DevPtr;
#else
typedef void*	TexPtr;
typedef void*	DevPtr;
#endif

// Textures / File Names / Sprite Objects (container)
struct enviromentElement {

	string fileElement;
	LPDIRECT3DTEXTURE9 texElement;
	LPD3DXSPRITE spriteElement;
	D3DCOLOR colorElement;


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

};

class Levels {

	// Device
	DevPtr LevelDevice;

	// Textures / File Names / Sprite Objects
	enviromentElement background;
	enviromentElement foreground;
	enviromentElement clouds;

	// Level
	int level;
	double globalPosition;
	double playerX;
	double playerY;
//	Point BoundingPosition;

	// How long you've been in this level
	double levelTime;



public:
	Levels(int inLevel, string inBackground, string inForeground, string inClouds, DevPtr inDevice);
	// How it should be with const char* Levels(int inLevel, const char *background, string inForeground, string inClouds, DevPtr inDevice);
	~Levels();

	LPDIRECT3DTEXTURE9 getBackground(){return background.texElement;}; 


	// Rendering
	void Levels::Sprite_Transform_Draw(enviromentElement inElement);

	// Converters
	string convertInt(int in);

	// Getters
	enviromentElement getBackgroundElement(){return background;}
	enviromentElement getForegroundElement(){return foreground;}
	enviromentElement getCloudsElement(){return clouds;}
	
	int getLevel(){return level;};

	//Updates
	void updateLevel(double inPlayerPositionX, double inPlayerPositionY);
	void move();
};

class levelManager {

private:
	DevPtr ManagerDevice;
	int currentLevel;

public:
	LPDIRECT3DDEVICE9 d3ddev;
	levelManager(LPDIRECT3DDEVICE9 d3ddev);
	virtual ~levelManager();

	void drawLevel(int inLevel);
	void AddLevel(int inLevel);
	void update(double inX, double inY);

	LinkedList < Levels > *m_Level; 
};

