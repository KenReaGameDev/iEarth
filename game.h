#pragma	once
#include <time.h>
#include "input.h"
#include "display.h"
#include "sprite.h"
#include "layer.h"
#include "texlist.h"
#include "font.h"
#include "linkedlist.h"
#include "levels.h"
#include "objects.h"
#include <sstream>
#include <iostream>
#include <irrKlang.h>
#include "MultiThread.h"

using namespace irrklang;

#pragma comment(lib, "irrKlang.lib")



#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)


class Game
{

protected:
	Display* display;
	Input* input;
	TexList* texlist;
	Zombie* zombie;
	Player* test;
	MainMenu* mainMenu;
	MainMenu* startButton;
	MainMenu* Crowd;
	Objects* object;
	Manager* manager;
	Projectile* bullet;
	Font* tracker;
	levelManager* levelmang;
	objectManager* objectmang;
	ISoundEngine* engine;

	DWORD screentimer;
	int fps;
	int	nextFrame;
	int currentLevel;

	// Multi-threading
	ThreadX* thread0;
	HANDLE   hth1;
	unsigned  uiThread1ID;

	ThreadX* thread1;
	HANDLE   hth2;
	unsigned  uiThread2ID;

public:

	Game();
	~Game();
	bool Game_Init(HWND window, int SCREENW, int SCREENH);
	void Game_Run(HWND window);
	void player_objectCollisionDetect(Player* inPlayer, Objects* inObject);
	void levelChange(int inLevel);
	void ParseKeys();
	void LevelChecks();
	void hudUpdates();
	void RenderGame();
	void CollisionDetection();
	void StartMenuCycle();
	void InGameCycle();
	void PausedCycle();
	void NextLevel();
	void EndGame();
	void PrepLevel();

};