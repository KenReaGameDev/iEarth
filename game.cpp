#include "game.h"
#include "D3DX10math.h"
#include <math.h>

double	mousedex = 0;
double	mousedey = 0;
int		movedu = 100;
int		prevDir = 2;
int		prevSound = 0;
int		prevSound2 = 0;
int		levelEnd = 99999;
bool	soundCreated = false;
bool	songPlaying = false;
bool	menuPlayed = false;
bool	startMenu = true;
bool	changeLevel = false;



// Timers	
float pauseTimer = 0;

enum GameState {State_MainMenu, State_Playing, State_Paused, State_LevelChange};
int CurrentGameState = State_MainMenu;

// Externs
extern const int lowestX;
extern const int highestX;
extern const int rangeX;
extern const int lowestY;;
extern const int highestY;
extern const int rangeY;
extern bool gameover;

char*	TextureListNames[] = 
{
	"city.png ",
	"ryanalpha.png",
	"darkClouds.png",
	"Zombie1Final.png",
	"Bullet.png",
	"Foreground.png",
	"main-menu.png",
	"startButton.png",
	"superJobs.png",
	"4ground.png"
};

Game::Game() : nextFrame(0), display(NULL), input(NULL), fps(30)
{


}

Game::~Game()
{
	EndGame();
}



bool Game::Game_Init(HWND window, int SCREENW, int SCREENH)
{

	thread0 = new ThreadX();
	thread1 = new ThreadX();

	srand((unsigned int) time(NULL));

	display = new Display(SCREENW, SCREENH);
	if (!display->Direct3D_Init(window, SCREENW, SCREENH, false)) return false;

	input = new Input();
	if (!input->DirectInput_Init(window)) return false;

	texlist = new TexList(display->getDevice(),TextureListNames, 10);
	if(!texlist) return false;

	mainMenu = new MainMenu(display->getDevice(), display->getWidth(), display->getHeight(), texlist->use(7));
	if(!mainMenu) return false;

	startButton = new MainMenu(display->getDevice(), display->getWidth(), display->getHeight(), texlist->use(8), true);
	if(!startButton) return false;

	Crowd = new MainMenu(display->getDevice(), display->getWidth(), display->getHeight(), texlist->use(8), true);
	if(!startButton) return false;

	// Testing new Level Manager.
	levelmang = new levelManager(display->getDevice());
	if (!levelmang) return false;

	objectmang = new objectManager(display->getDevice());
	if(!objectmang) return false;

	manager = new Manager();
	if(!manager) return false;

	manager->AddPlayer(display->getDevice(), texlist->use(2), 1, 200, 200, display->getWidth(), display->getHeight());
	manager->m_Player->GetFirst()->level = 1;

	engine = createIrrKlangDevice();
	if (!engine) return false;

	tracker = new Font(display->getDevice(), "System Bold", 24, 1, false, "Level 1");
	if(!tracker) return false;

	gameover = false;

	return true;

}


// Add level objects here this is pretty much loading levels and what not.

void Game::PrepLevel()
{
	engine->stopAllSounds();
	manager->m_Player->GetFirst()->x = 50;
	levelmang->m_Level->Empty();
	objectmang->m_Object->Empty();
	manager->m_Enemies->SetToOne();
}

void Game::levelChange(int inLevel)
{
	if (inLevel != currentLevel)
	{
		if (inLevel == 1)
		{
			PrepLevel();
			levelmang->AddLevel(1);	
			manager->m_Player->GetFirst()->currentLevel = inLevel;
			levelEnd = 500;
			currentLevel = inLevel;
			objectmang->AddLevel(1);

			// TICK (Level, STATIC/POWERUP, TYPE OF POWERUP, TYPE OF STATIC, TICK IT COMES INTO PLAY, TICK IT DIES AT, Y POSITION)
			// Adding item by tick will allow you to spawn it when ever you want.
			objectmang->AddObjectTick(1, POWERUP, HEALTH, NULL, 5000, 900000000, 20);

			// POSI (Level, STATIC/POWERUP, TYPE OF POWERUP, TYPE OF STATIC, START X, START Y)
			// Adding object by position allows you to place it anywhere on the map.
			objectmang->AddObjectPosition(1, POWERUP, HEALTH, NULL, 200, 300);

			// Adds Amount of enemies to level
			for (int ndx = 0; ndx <= 4; ndx++)
			{
				manager->AddEnemy(display->getDevice(), texlist->use(4), 1, 
					(lowestX+int(rangeX*rand()/(RAND_MAX + 1.0))), 
					(lowestY+int(rangeY*rand()/(RAND_MAX + 1.0))));
			}


			engine->play2D("..//Music//bondTheme.mp3");

		}

		if (inLevel == 2)
		{
			PrepLevel();
			currentLevel = inLevel;
			manager->m_Player->GetFirst()->currentLevel = inLevel;
			levelmang->AddLevel(2);				
			levelEnd = 500;	
			objectmang->AddLevel(2);
			objectmang->AddObjectTick(2, POWERUP, HEALTH, NULL, 5000, 900000000, 20);

			// POSI (Level, STATIC/POWERUP, TYPE OF POWERUP, TYPE OF STATIC, START X, START Y)
			objectmang->AddObjectPosition(2, POWERUP, HEALTH, NULL, 200, 300);

			for (int ndx = 0; ndx < 8; ndx++)
			{
				manager->AddEnemy(display->getDevice(), texlist->use(4), 1, (lowestX+int(rangeX*rand()/(RAND_MAX + 1.0))), (lowestY+int(rangeY*rand()/(RAND_MAX + 1.0))));
			}
			engine->play2D("..//Music//level2.mp3");
			CurrentGameState = State_Playing;

		}

		if (inLevel == 3)
		{
			PrepLevel();
			currentLevel = inLevel;
			manager->m_Player->GetFirst()->currentLevel = inLevel;
			levelmang->AddLevel(3);				
			levelEnd = 500;			
			objectmang->AddLevel(3);
			objectmang->AddObjectTick(3, POWERUP, HEALTH, NULL, 5000, 900000000, 20);

			// POSI (Level, STATIC/POWERUP, TYPE OF POWERUP, TYPE OF STATIC, START X, START Y)
			objectmang->AddObjectPosition(3, POWERUP, HEALTH, NULL, 200, 300);

			for (int ndx = 0; ndx < 16; ndx++)
			{
				manager->AddEnemy(display->getDevice(), texlist->use(4), 1, (lowestX+int(rangeX*rand()/(RAND_MAX + 1.0))), (lowestY+int(rangeY*rand()/(RAND_MAX + 1.0))));
			}
			engine->play2D("..//Music//level3.mp3");
			CurrentGameState = State_Playing;
		}

		if (inLevel > 3)
		{
			engine->stopAllSounds();
			CurrentGameState = State_MainMenu;
		}
	}
}

void Game::Game_Run(HWND window)
{
	if (CurrentGameState == State_MainMenu)
		StartMenuCycle();

	if (CurrentGameState == State_Playing)
		InGameCycle();

	if (CurrentGameState == State_Paused)
		PausedCycle();

	if(input->Key_Down(DIK_ESCAPE))
	{
		CurrentGameState = State_MainMenu;
		StartMenuCycle();
		gameover = true;
	}
}

void Game::InGameCycle()
{
	DWORD screentimer = timeGetTime();
	if (!display->getDevice()) return;

	input->DirectInput_Update();
	display->clear();

	if (screentimer > nextFrame && display->getDevice()->BeginScene())
	{

		// Level Functions -- Both of these calls need to be here for the levels to run. 
		levelmang->drawLevel(manager->m_Player->GetFirst()->currentLevel);
		levelmang->update(manager->m_Player->GetFirst()->getX(), manager->m_Player->GetFirst()->getX());

		////Object Functions
		if (objectmang->m_Object->GetFirst() != NULL) 
		{
			objectmang->update(manager->m_Player->GetFirst()->getMove(),manager->m_Player->GetFirst()->currentLevel, manager->m_Player->GetFirst()->getX(), 10);
			objectmang->m_Object->GetFirst()->Sprite_Transform_DrawAll();
		}


		// Update checks collisons, moves characters, redraws frames.
		manager->Update();

		CollisionDetection();
		ParseKeys();
		hudUpdates();
		RenderGame();

		LevelChecks();

		nextFrame = screentimer + (1000/fps);
	}

}
void Game::StartMenuCycle()
{
	DWORD screentimer = timeGetTime();
	if (!display->getDevice()) return;
	input->DirectInput_Update();
	display->clear();
	if (!menuPlayed) 
	{
		engine->play2D("..//Music//menuRap.wav");
		menuPlayed = true;
		engine->stopAllSounds();
	}
	if (screentimer > nextFrame && display->getDevice()->BeginScene()) 
	{
		mainMenu->Sprite_Transform_Draw();
		startButton->Sprite_Transform_Draw();


		if (input->Key_Down(DIK_RETURN)/*startButton->mouseCollision(display->getWindow(), input) && input->Mouse_Button(0)*/)
		{
			CurrentGameState = State_Playing;
			levelChange(1);
		}

		if (display->getDevice()->BeginScene())
		{

			//stop rendering
			display->getDevice()->EndScene();
			display->getDevice()->Present(NULL, NULL, NULL, NULL);

		}		
		nextFrame = screentimer + (1000/fps);
	}

}
void Game::PausedCycle()
{

	pauseTimer += 1;

	if ( pauseTimer > 1000000)
	{
		if(input->Key_Down(DIK_P))
		{
			CurrentGameState = State_Playing;
			pauseTimer = 0;
		}
	}

}
void Game::RenderGame()
{

	//start rendering
	if (display->getDevice()->BeginScene())
	{
		//stop rendering
		display->getDevice()->EndScene();
		display->getDevice()->Present(NULL, NULL, NULL, NULL);

	}
}
void Game::hudUpdates()
{
	//manager->AddProjectile(display->getDevice(), texlist->use(5), 1, manager->m_Enemies->GetFirst()->getX(), manager->m_Enemies->GetFirst()->getY(), 10, 0, 4, 1, 3, -1, 0, 2);
	tracker->covSay(manager->projectilesInGame);

	// Mouse Tracking for Debugging
	mousedex += input->Mouse_X();
	mousedey += input->Mouse_Y();

	// Player Tracking for Debugging
	tracker->covSay(manager->m_Player->GetFirst()->getX());
	tracker->sayNow(1, 300, 50, tracker->getText());

	tracker->covSay(manager->m_Player->GetFirst()->getY());
	tracker->sayNow(1, 400, 50, tracker->getText());

	// Score Display
	tracker->covSay(manager->score);
	tracker->sayNow(1, 720, 50, tracker->getText());

	// Health Display (Numerical)
	tracker->covSay(manager->m_Player->GetFirst()->playerHealth);
	tracker->sayNow(1, 50, 50, tracker->getText());

	if (!manager->m_Player->GetFirst()->isAlive()) 
	{
		tracker->say("PRESS 'R' TO RESPAWN!");
		tracker->sayNow(1, 900, 50, tracker->getText());
	}

}
void Game::LevelChecks()
{
	if (manager->m_Player->GetFirst()->currentLevel == 3)
	{
		startMenu = true;
	}

	// Checks if the player Position is farther than the Levels End.
	if (manager->m_Player->GetFirst()->globalX > levelEnd) 
	{
		NextLevel();
	}

	if (changeLevel)
	{
		NextLevel();
		changeLevel = false;
	}

}
void Game::NextLevel()
{
	CurrentGameState = State_LevelChange;
	manager->m_Player->GetFirst()->globalX = 0;
	int i = manager->m_Player->GetFirst()->currentLevel + 1;
	levelChange(i);	
}
void Game::EndGame()
{
	// Code here will be used to safetly delete all objects so game does not error out.
	engine->stopAllSounds();
	engine->drop();

	delete tracker;
//	delete engine;
	delete zombie;
	delete test;
	delete texlist;
	// delete input;
	//delete display;	

	delete levelmang;
	delete objectmang;
	//manager->~Manager();
	manager->m_Enemies->ClearPointers();
	manager->m_Enemies->Empty();
	manager->m_Player->ClearPointers();
	manager->m_Player->Empty();
	manager->m_Projectile->ClearPointers();
	manager->m_Projectile->Empty();
	delete manager;	
	int i = 0;
}

void Game::ParseKeys()
{
	int horz = 0;
	int vert = 0;
	int ID = 0;
	bool moving = false;

	POINT ptCursor;
	GetCursorPos( &ptCursor );
	ScreenToClient( display->getWindow(), &ptCursor );

	POINT playerPos;
	playerPos.x = manager->m_Player->GetFirst()->getX();
	playerPos.y = manager->m_Player->GetFirst()->getY();

	if(input->Key_Down(DIK_P))
	{
		pauseTimer = 0;
		CurrentGameState = State_Paused;
	}

	if(input->Mouse_Button(1))
	{
		if (manager->m_Player->GetFirst()->canfire)
		{

			manager->m_Player->GetFirst()->timefired = GetTickCount();

			if (ptCursor.x < 0 | ptCursor.y < 0)
				return;

			// Checks where the mouse is to shoot in that Direction
			double shootDirX = manager->m_Player->GetFirst()->getX() - ptCursor.x;
			double shootDirY = manager->m_Player->GetFirst()->getY() - ptCursor.y;
			double hypot = sqrt((shootDirX*shootDirX) + (shootDirY*shootDirY));
			double angle = sin(shootDirY / hypot);

			// Quadrent Check
			// TOP LEFT IS 0,0  --- DO NOT FORGET THIS. THIS IS CAUSING THE BUG.
			if (ptCursor.y < playerPos.y)
			{
				if (ptCursor.y > playerPos.y){ angle += 180;}
				if (ptCursor.y < playerPos.y)
				{
					angle += 90;
					shootDirY -= (shootDirY * 2);
				}
			}

			if (ptCursor.x < playerPos.x)
			{
				if (ptCursor.y > manager->m_Player->GetFirst()->getY()){ angle += 270;}
				if (ptCursor.x < playerPos.x)
				{
					angle += 0;
					shootDirX -= (shootDirX * 2);
				}
			}

			if (ptCursor.y > playerPos.y)
			{
				//if (ptCursor.y > playerPos.y){ angle += 180;}
				if (ptCursor.y > playerPos.y)
				{
					angle += 90;
					shootDirY += (shootDirY * -2);
				}
			}

			if (ptCursor.x > playerPos.x)
			{
				//if (ptCursor.y > manager->m_Player->GetFirst()->getY()){ angle += 270;}
				if (ptCursor.x > playerPos.x)
				{
					angle += 0;
					shootDirX += (shootDirX * -2);
				}
			}

			tracker->covSay(angle);
			tracker->sayNow(1, 600, 50, tracker->getText());

			// Makes sure bullets don't go out of control because of division.
			if (shootDirX <= 0 && shootDirX > -1) shootDirX = -1;
			if (shootDirX >= 0 && shootDirX < -1) shootDirX = 1;
			if (shootDirY <= 0 && shootDirY > -1) shootDirX = -1;
			if (shootDirY >= 0 && shootDirY < -1) shootDirX = 1;

			ID = 1;   
			if (GetTickCount() > (prevSound2 + 50000)) {
				if (manager->m_Player->GetFirst()->canfire) {
					if (manager->m_Player->GetFirst()->isAlive()){
						manager->AddProjectile(display->getDevice(), texlist->use(5), 1, 2, 15.0f, manager->getPlayerLocationX(ID) + 70, manager->getPlayerLocationY(ID) +25, 8, 0, 4, 1, 3, shootDirX, shootDirY, 1);
						tracker->covSay(manager->projectilesInGame);
						tracker->sayNow(1, manager->getPlayerLocationX(ID), manager->getPlayerLocationY(ID), tracker->getText());
						manager->m_Player->GetFirst()->Shoot();
						ISound* music = engine->play2D("shouryuken.wav", false);
						prevSound2 = GetTickCount();
					}
				}
			}
		}
	}

	if(manager->m_Player->GetFirst()->getMove())
	{
		if(input->Key_Down(DIK_A) || input->controllers[0].sThumbLX < -2000) {horz = -1; ID = 1; manager->m_Player->GetFirst()->setRight(false);}
		if(input->Key_Down(DIK_D) || input->controllers[0].sThumbLX > 2000) {horz = 1; ID = 1; moving = true; manager->m_Player->GetFirst()->setRight(true);}
		if(input->Key_Down(DIK_W) || input->controllers[0].sThumbLY > 2000) {vert = -1; ID = 1;}
		if(input->Key_Down(DIK_S) || input->controllers[0].sThumbLY< -2000) {vert = 1; ID = 1;}
		if (input->Key_Down(DIK_Q)) 
		{ 
			if(prevDir == 1) prevDir = 2;
			else prevDir = 1;
		}

		manager->playerMove(horz, vert, ID);
	}

	bool movingRightTemp = manager->m_Player->GetFirst()->movingRight;
	// N to change level!
	if(input->Key_Down(DIK_N))
		changeLevel = true;

	// Shooting with Space Key, Straight Only
	if(input->Key_Down(DIK_SPACE))
	{
		if (GetTickCount() > (prevSound + 400)) {

			ID = 1;	
			if (manager->m_Player->GetFirst()->isAlive()){

				if (movingRightTemp) manager->AddProjectile(display->getDevice(), texlist->use(5), 1, 1, 1.0f, manager->getPlayerLocationX(ID)+70, manager->getPlayerLocationY(ID)+25, 30, 0, 4, 1, 3, 5, 0, 1);
				if (!movingRightTemp) manager->AddProjectile(display->getDevice(), texlist->use(5), 1, 1, 1.0f, manager->getPlayerLocationX(ID)+70, manager->getPlayerLocationY(ID)+25, -30, 0, 4, 1, 3, 5, 0, 1);
				tracker->covSay(manager->projectilesInGame);
				tracker->sayNow(1, manager->getPlayerLocationX(ID), manager->getPlayerLocationY(ID), tracker->getText());
				manager->m_Player->GetFirst()->Shoot();

				ISound* music = engine->play2D("hadouken.wav", false);

				prevSound = GetTickCount();
			}
		}
	}

	// Respawn Key
	if(input->Key_Down(DIK_R) && manager->m_Player->GetFirst()->isAlive() == false)
	{
		//ISoundEngine* engine = createIrrKlangDevice();
		engine->play2D("gobble.wav", false);
		manager->m_Player->GetFirst()->visible = true;
		manager->m_Player->GetFirst()->x = 100;
		manager->m_Player->GetFirst()->y = 100;
		manager->m_Player->GetFirst()->respawning = true;
		manager->m_Player->GetFirst()->respawnTime = GetTickCount();
	}

	if (input->Key_Down(DIK_G)) gameover = true;
	if (input->controllers[0].wButtons & XINPUT_GAMEPAD_BACK) gameover = true;

}

void Game::CollisionDetection()
{
	// Checks collisions against all objects from the player class.
	manager->m_Player->GetFirst()->CollisionD_Object(objectmang->m_Object->GetFirst());

}