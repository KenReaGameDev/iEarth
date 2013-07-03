#include <iostream>
#include "sprite.h"
#include "MultiThread.h"
using namespace std;


//MATH
const double PI = 3.1415926535;
const double PI_under_180 = 180.0f / PI;
const double PI_over180 = PI / 180.0f;

// Wouldn't actually use this global methodology but don't have enough time to make it scriptable
// Used for setting enemies when they respawn on the X Co-ordinate
extern const int lowestX = SCREENW - (SCREENW / 2);
extern const int highestX = (SCREENW * 2);
extern const int rangeX = (highestX - lowestX);

// Used for setting enemies when they respawn on the Y Co-ordinate
extern const int lowestY = (SCREENH - 500);
extern const int highestY = (SCREENH * 2);
extern const int rangeY = (highestX - lowestX);

double toRadians(double degrees)
{
	return degrees * PI_over180;
}

double toDegrees(double radians)
{
	return radians * PI_under_180;
}



// SPRITE FUNCTIONS //
void Sprite::Sprite_Draw_Frame()
{
	if (!visible) return;
	D3DXVECTOR3 position( (float)x, (float)y, 0 );
	D3DCOLOR color = D3DCOLOR_XRGB(255,255,255);

	RECT rect;
	rect.left = (frame % columns) * width;
	rect.top = (frame / columns) * height;
	rect.right = rect.left + width;
	rect.bottom = rect.top + height;


	spriteobj->Begin(D3DXSPRITE_ALPHABLEND);
	spriteobj->Draw( texture, &rect, NULL, &position, color);
	spriteobj->End();
}

void Player::Sprite_Transform_Draw()
{
	if (!visible) return;
	if (texture == NULL) return;

	//calc source frame rectangle in spritesheet

	D3DXVECTOR2 scale(scaling, scaling);

	if (!movingRight)
		scale.x *= -1;

	D3DXVECTOR2 center((scaling * width)/2, (scaling * height)/2); //finds center of CEL
	D3DXVECTOR2 trans(x, y);

	//create 2D transformation matrix
	D3DXMATRIX mat;
	D3DXMatrixTransformation2D(&mat, NULL, 0, &scale, &center, (float) toRadians((double)rotation), &trans);

	//tell sprite object to use the transform
	spriteobj->SetTransform( &mat );

	RECT rect;
	rect.left = (frame % columns) * width; //find wrap amount - calculating the col
	rect.top = (frame / columns) * height; //calculating the rows * height
	rect.right = rect.left + width;
	rect.bottom = rect.top + height;

	//draw the sprite frame
	spriteobj->Begin(D3DXSPRITE_ALPHABLEND);
	spriteobj->Draw( texture, &rect, NULL, NULL, color );

	D3DXMatrixIdentity( &mat );
	spriteobj->SetTransform( &mat );
	spriteobj->End();

}
void Sprite::Sprite_Transform_Draw()
{
	if (!visible) return;
	if (texture == NULL) return;

	//calc source frame rectangle in spritesheet


	D3DXVECTOR2 scale(scaling, scaling);
	D3DXVECTOR2 center((scaling * width)/2, (scaling * height)/2); //finds center of CEL
	D3DXVECTOR2 trans(x, y);

	//create 2D transformation matrix
	D3DXMATRIX mat;
	D3DXMatrixTransformation2D(&mat, NULL, 0, &scale, &center, (float) toRadians((double)rotation), &trans);

	//tell sprite object to use the transform
	spriteobj->SetTransform( &mat );

	RECT rect;
	rect.left = (frame % columns) * width; //find wrap amount - calculating the col
	rect.top = (frame / columns) * height; //calculating the rows * height
	rect.right = rect.left + width;
	rect.bottom = rect.top + height;

	//draw the sprite frame
	spriteobj->Begin(D3DXSPRITE_ALPHABLEND);
	spriteobj->Draw( texture, &rect, NULL, NULL, color );

	D3DXMatrixIdentity( &mat );
	spriteobj->SetTransform( &mat );
	spriteobj->End();
}

void Sprite::move(double fx, double fy)
{

	int timeNow = GetTickCount();

	//left
	if (fx > 0){x -= velx;}

	//right
	if (fx < 0) {x += velx;}

	//up
	if (fy < 0) {y -= vely;}

	//down
	if (fy > 0){y += vely;}

}


void Sprite::fade(float inRamp, int inVisDelay)
{
	visRamp = inRamp;
	visDelay = inVisDelay;
	visTime = GetTickCount();
}

bool Sprite::isDead()
{
	if (!alive) {return true;}
	else {return false;}
}

int Sprite::Collision(Sprite* sprite2)
{
	RECT rect1;
	rect1.left = (long)x;
	rect1.top = (long)y;
	rect1.right = (long)(x + width * scaling);
	rect1.bottom = (long)(y + height * scaling);

	RECT rect2;
	rect2.left = (long)sprite2->x;
	rect2.top = (long)sprite2->y;
	rect2.right = (long)(sprite2->x + sprite2->width * sprite2->scaling);
	rect2.bottom = (long)(sprite2->y + sprite2->height * sprite2->scaling);

	RECT dest; //ignored
	return IntersectRect(&dest, &rect1, &rect2);
}


bool Sprite::CollisionD(Sprite* sprite2)
{
	double radius1, radius2;

	//calculate radius 1
	if (width > height)
		radius1 = (width * scaling) / 2.0;
	else
		radius1 = (height * scaling) / 2.0;

	//center point 1
	double x1 = x + radius1;
	double y1 = y + radius1;
	D3DXVECTOR2 vector1( (float)x1, (float)y1);

	//calculate radius 2
	if (sprite2->width > sprite2->height)
		radius2 = (sprite2->width * sprite2->scaling) / 2.0;
	else
		radius2 = (sprite2->height * sprite2->scaling) / 2.0;

	//center point 2
	double x2 = sprite2->x + radius2;
	double y2 = sprite2->y + radius2;
	D3DXVECTOR2 vector2( (float)x2, (float)y2);

	//calculate distance
	double deltax = vector1.x - vector2.x;
	double deltay = vector2.y - vector1.y;
	double dist = sqrt((deltax * deltax) + (deltay * deltay));

	//return distance comparison
	return (dist < radius1 + radius2);
}

// ADDED 9/8/2011
void Sprite::CollisionD_Object(Objects* sprite2)
{
	canMove = true;
	for (int ndx = 0; ndx <= sprite2->getObjectsUsed(); ndx++)
	{
		bool collision = false;
		objectElement *tempElement = sprite2->getObjectElement(ndx);
		if (tempElement->alive == false){ break;}
		double radius1, radius2;

		//calculate radius 1
		if (width > height)
			radius1 = (width * scaling) / 2.0;
		else
			radius1 = (height * scaling) / 2.0;

		//center point 1
		double x1 = x + radius1;
		double y1 = y + radius1;
		D3DXVECTOR2 vector1( (float)x1, (float)y1);

		//calculate radius 2
		if (tempElement->width > tempElement->height)
			radius2 = (tempElement->width * tempElement->scaling) / 2.0;
		else
			radius2 = (tempElement->height * tempElement->scaling) / 2.0;

		//center point 2
		double x2 = tempElement->x + radius2;
		double y2 = tempElement->y + radius2;
		D3DXVECTOR2 vector2( (float)x2, (float)y2);

		//calculate distance
		double deltax = vector1.x - vector2.x;
		double deltay = vector2.y - vector1.y;
		double dist = sqrt((deltax * deltax) + (deltay * deltay));

		//return distance comparison
		if ((dist < radius1 + radius2) >= 1) collision = true;

		if (collision)
		{
			if (tempElement->type == STATIC)
			{ 
				canMove = false;
				tempElement->canMove = false;

			}

			if (tempElement->type == POWERUP)
			{
				if (respawning) break;
				if (tempElement->power == HEALTH) 
				{
					tempElement->alive = FALSE;
					playerHealth += 50;
				}

				if (tempElement->power == AMMO) {}
			}
		}
	}

}

bool Sprite::mouseCollision(HWND hwindow ,Input* input)
{

	POINT pointCursor;
	GetCursorPos( &pointCursor );
	ScreenToClient( hwindow, &pointCursor );

	double radius1, radius2;

	//calculate radius 1
	if (width > height)
		radius1 = (width * scaling) / 2.0;
	else
		radius1 = (height * scaling) / 2.0;

	//center point 1
	double x1 = x + radius1;
	double y1 = y + radius1;
	D3DXVECTOR2 vector1( (float)x1, (float)y1);

	//calculate radius 2
	radius2 = (20 * 1.0f) / 2.0;

	//center point 2
	double x2 = pointCursor.x + radius2;
	double y2 = pointCursor.y + radius2;
	D3DXVECTOR2 vector2( (float)x2, (float)y2);

	//calculate distance
	double deltax = vector1.x - vector2.x;
	double deltay = vector2.y - vector1.y;
	double dist = sqrt((deltax * deltax) + (deltay * deltay));

	//return distance comparison
	return (dist < radius1 + radius2);
}

// PLAYER FUNCTIONS //
void Player::Shoot()
{
	frame=3;
}


void Player::snap(int movex, int movey)
{
	int timeNow = GetTickCount();

	if((frame != 0) && (timeNow > starttime))
	{
		if (frame > 0 && movex == 0 && movey == 0) frame = 0;
		starttime = timeNow + delay;
	}
}

void Player::move(int fx, int fy)
{
	if (canMove == false) return;


	int h = fx;
	int v = fy;
	int timeNow = GetTickCount();

	//left
	if (h < 0)
	{

		frame++;
		if(frame>2)
		{
			frame=0;
		};
		x -= velx;
	}

	//right
	if (h > 0) 
	{
		frame++;
		if(frame>2)
		{
			frame=0;
		};
		x += velx;
	}

	//up
	if (v < 0) 
	{
		frame++;
		if(frame>2)
		{
			frame=0;
		};
		y -= vely;
	}

	//down
	if (v > 0)
	{
		frame++;
		if(frame>2)
		{
			frame=0;
		};
		y += vely;
	}

	snap(h, v);

}

bool Player::checkCanMove()
{
	return canMove;
}

void Player::checkcanfire()
{
	int tick = GetTickCount();
	int tickcheck =  timefired + firetime;
	if (tick > tickcheck){ canfire = true;}
	if (tick < tickcheck){ canfire = false;}
}

bool Player::checkCanDamage()
{
	int tick = GetTickCount();
	if (tick > lastDamaged + damageTime) {return true;}
	return false;
}

void Player::setRight(bool inRight)
{
	movingRight = inRight;
}

// PROJECTILE FUNCTIONS //
void Projectile::update()
{

}

void Projectile::move(double inVelX, double inVelY)
{
	x += inVelX;
	y += inVelY;

}

void Projectile::projectileDrop()
{
	y -= gravity;
}


// MANAGER FUCTIONS //

// Adds projectile to linked list.
void Manager::AddProjectile(LPDIRECT3DDEVICE9 d3ddev, LPDIRECT3DTEXTURE9 tex, int inID, int inBulletType, double inScale, int inX, int inY, int inVelX, int inVelY, int inDamage, bool inAlive, float inScaling, double inDirectionX, double inDirectionY, int inType)
{	
	projectilesInGame++;
	projectilesFired++;
	m_Projectile->Add(new Projectile(d3ddev, tex, inID, inBulletType, inScale, inX, inY, inVelX, inVelY, inDamage, inAlive, inScaling, inDirectionX, inDirectionY, inType, projectilesFired));
}

void Manager::AddEnemy(LPDIRECT3DDEVICE9 d3ddev, LPDIRECT3DTEXTURE9 tex, int inID, int inX, int inY)
{

	m_Enemies->Add(new Zombie(d3ddev, tex, enemiesCreated, inX, inY));
	enemiesCreated++;
}

void Manager::AddPlayer(LPDIRECT3DDEVICE9 d3ddev, LPDIRECT3DTEXTURE9 tex, int inID, int inX, int inY, int inStageWidth, int inStageHeight)
{
	playersCreated++;
	m_Player->Add(new Player(d3ddev, tex, playersCreated, inX, inY, inStageWidth, inStageHeight));

}

void Manager::playerMove(int fx, int fy, int inID)
{
	Player* playerTemp = m_Player->GetFirst();
	if (playerTemp->checkCanMove() == false) {return;}
	bool moving = true;
	int playersPlayered = 0;
	// Right
	if (playerTemp->x >= (SCREENW * 0.60)) {playerTemp->x = (SCREENW * 0.6);}
	// Bottom
	if (playerTemp->y >= (SCREENH - 145)) {playerTemp->y = (SCREENH-146);}
	// Left
	if (playerTemp->x <= 0) {playerTemp->x = 1;}
	// Top
	if (playerTemp->y <= (SCREENH - 500)) {playerTemp->y = (SCREENH-499);}
	while (moving)
	{
		if (playerTemp->getID() == inID)
		{
			playerTemp->move(fx, fy);
			playerTemp->globalX += fx;
			moving = false;
		}
		else
		{
			playerTemp = m_Player->GetNext(playerTemp);
			playersPlayered++;
		}
		if (playersPlayered >= playersCreated) moving = false;
	}

}

//void Manager::enviromentMove(Background *inBack, Foreground *inFore)
//{
//	inBack->backMove(m_Player->GetFirst()->getX());
//	inFore->foreMove(m_Player->GetFirst()->getX());
//
//}

// Rechecks the entire Linked List for this Manager.
// Removes any objects that are dead.
// Draws all alive objects for this frame.
void Manager::Update()
{

	Projectile* temp = m_Projectile->GetFirst();
	Player* playerTemp = m_Player->GetFirst();
	Zombie* enemyTemp = m_Enemies->GetFirst();



	// PROJECTILE MOVEMENT // Projectiles need movement calculations to work correctly and not throw
	// bullets in random directions when you get to close.
	while (temp != NULL)
	{
		double projspeed = temp->getSpeed();
		doublePoint dir;
		dir = temp->getDirection();
		double dirTotal = dir.x + dir.y;
		if (dirTotal < 1){ dirTotal *= -1;}
		double x = dir.x/dirTotal;
		double y = dir.y/dirTotal;
		x = projspeed * x;
		y = projspeed * y;
		temp->move(x, y);
		temp->Sprite_Transform_Draw();    
		if (m_Projectile->GetNext(temp) == NULL) {break;}
		if (m_Projectile->GetNext(temp) != NULL) {temp = m_Projectile->GetNext(temp);}

	}

	while (playerTemp != NULL){

		playerTemp->checkcanfire();
		playerTemp->checkCanMove();
		playerTemp->Sprite_Transform_Draw();
		playerTemp = m_Player->GetNext(playerTemp);
	}

	while (enemyTemp != NULL)
	{	
		enemyTemp->Sprite_Draw_Frame();		
		enemyTemp = m_Enemies->GetNext(enemyTemp);
	}

	collisionChecks();
	enemyMove();
	checkDead();


}

void Manager::checkDead()
{
	Projectile* projTemp = m_Projectile->GetFirst();
	Player* playerTemp = m_Player->GetFirst();
	Zombie* enemyTemp = m_Enemies->GetFirst();

	bool checking = true;

	// Checking to see if the bullets on screen are dead. If they are, remove them.
	while (checking)
	{
		// If there is no projectile, drop out of this loop.
		if (!projTemp) {break;}
		if (projTemp->isDead() == true)
		{


			if (projTemp == m_Projectile->GetFirst())
			{
				if (m_Projectile->GetNext(projTemp) != NULL){ m_Projectile->AppendFirst();}
			}


			Projectile* removing = projTemp;
			projTemp = m_Projectile->GetNext(projTemp);
			m_Projectile->Remove(&removing);

		} else {
			projTemp = m_Projectile->GetNext(projTemp);
		}
	}

	// Checking to see if the player is dead. If player is dead, initiate respawn button.
	while (checking)
	{
		if (!playerTemp) {break;}
		int ticknow = GetTickCount();
		if (playerTemp->respawning == true && (ticknow > playerTemp->respawnTime + 5000))
		{
			playerTemp->respawning = false;
		}
		if (playerTemp->isAlive() == false)
		{

			if (!m_Player->GetNext(m_Player->GetFirst()))
			{			
				playerTemp->x = NULL;
				playerTemp->y = NULL;
				playerTemp->visible = false;
				checking = false;
				break;
			}
			else
			{
				Player* removing = playerTemp;
				playerTemp = m_Player->GetNext(playerTemp);
				m_Player->Remove(&removing);
			}
		}
		else 
		{
			if (playerTemp != m_Player->GetNext(playerTemp))
			{
				playerTemp = m_Player->GetNext(playerTemp);
			} 
			else 
			{ 
				break;
			}
		}
		break;
	}
}


void Manager::enemyMove()
{
	Zombie* enemyTemp = m_Enemies->GetFirst();
	Player* playerTemp = m_Player->GetFirst();


	while (enemyTemp)
	{
		if (playerTemp->isAlive() == false) break;
		if (enemyTemp->getX() < playerTemp->getX()) enemyTemp->x += 5;
		if (enemyTemp->getX() > playerTemp->getX()) enemyTemp->x -= 5;
		if (enemyTemp->getY() < playerTemp->getY()) enemyTemp->y += 5;
		if (enemyTemp->getY() > playerTemp->getY()) enemyTemp->y -= 5;
		enemyTemp->ZombieAnimate();


		//if (enemyTemp->y >= (SCREENH - 145)) {enemyTemp->y = (SCREENH-146);}
		if (enemyTemp->y <= (SCREENH - 500)) {enemyTemp->y = (SCREENH-499);}

		enemyTemp = m_Enemies->GetNext(enemyTemp);
	}
}

//void Manager::enemyShoot()
//{
//	Manager.AddProjectile();
//}






void Manager::collisionChecks()
{

	// Sets Player, Zombie, Projectile to test
	Projectile* projTemp = m_Projectile->GetFirst();
	Player* playerTemp = m_Player->GetFirst();
	Zombie* enemyTemp = m_Enemies->GetFirst();
	Zombie* enemyTemp2 = m_Enemies->GetFirst();

	// Sets testing requirements to true
	bool bulletVsPlayer = true;
	bool enemyVsPlayer = true;

/*
	ThreadX * o1 = new ThreadX();
	HANDLE   hth1;
	unsigned  uiThread1ID;

	ThreadX * o2 = new ThreadX();
	HANDLE   hth2;
	unsigned  uiThread2ID;


		// Thread 1

		o1->PassInClass_Manager(this);
		// 3rd Parameter allows the thread to start inside a function. Only beginthreadex allows this.
		hth1 = (HANDLE)_beginthreadex( NULL,         // security
			0,            // stack size
			ThreadX::ThreadStaticEntryPointEnemy_Player,
			o1,           // arg list
			CREATE_SUSPENDED,  // so we can later call ResumeThread()
			&uiThread1ID );
		if ( hth1 == 0 )
			printf("Failed to create thread 1\n");
		DWORD   dwExitCode;
		GetExitCodeThread( hth1, &dwExitCode );  // should be STILL_ACTIVE = 0x00000103 = 259
		printf( "initial thread 1 exit code = %u\n", dwExitCode );
		o1->threadName = "t1";


		// Thread 2

		o2->PassInClass_Manager(this);
		// 3rd Parameter allows the thread to start inside a function. Only beginthreadex allows this.
		hth2 = (HANDLE)_beginthreadex( NULL,         // security
			0,            // stack size
			ThreadX::ThreadStaticEntryPointEnemy_Enemy,
			o2,           // arg list
			CREATE_SUSPENDED,  // so we can later call ResumeThread()
			&uiThread2ID );
		if ( hth2 == 0 )
			printf("Failed to create thread 1\n");
		GetExitCodeThread( hth2, &dwExitCode );  // should be STILL_ACTIVE = 0x00000103 = 259
		printf( "initial thread 1 exit code = %u\n", dwExitCode );
		o2->threadName = "t2";
		threaded = true;

		ResumeThread( hth1 );
		ResumeThread( hth2 );

*/

ProjectileCollision:
	// reset enemies and players to first.
	enemyTemp = m_Enemies->GetFirst();
	playerTemp = m_Player->GetFirst();

	LinkedList < Zombie > *zombieSplit = new LinkedList < Zombie > ();
	//zombieSplit->Add(enemyTemp);
	
	m_Enemies->Split();

	//m_Enemies->Split( zombieSplit );

	// PROJECTILE COLLISION //
	while (bulletVsPlayer)
	{
		if (!projTemp) 
		{
			enemyTemp = m_Enemies->GetNext(enemyTemp);
			projTemp = m_Projectile->GetFirst();
		}
		if (!enemyTemp){break;}
		if (projTemp != NULL){

			// Players Projectiles vs Enemies
			if (projTemp->CollisionD(enemyTemp) && (projTemp->type != enemyTemp->type) )
			{
				if (projTemp->bulletType == 1) 
				{
					//enemyTemp->setAlive(false);
					//projTemp->setAlive(false);
					m_Projectile->Remove(&projTemp);  // Line Causes issues when trying to get X value.
					//m_Enemies->Remove(&enemyTemp);
					enemyTemp->x += 20;
					score += 10;
					enemyTemp->x = lowestX+int(rangeX*rand()/(RAND_MAX + 1.0));
					enemyTemp->y = lowestY+int(rangeY*rand()/(RAND_MAX + 1.0));
					if (enemyTemp->x < SCREENW && enemyTemp->y < SCREENH) enemyTemp->y = SCREENW + 100;
					//AddEnemy(enemyTemp->holdd3ddev, enemyTemp->texture, 10, 300, 300);
					projectilesInGame--;
				} 
				else if (projTemp->bulletType == 2)
				{

					enemyTemp->x += 20;
					score += 10;
					enemyTemp->x = lowestX+int(rangeX*rand()/(RAND_MAX + 1.0));
					enemyTemp->y = lowestY+int(rangeY*rand()/(RAND_MAX + 1.0));
					if (enemyTemp->x < SCREENW && enemyTemp->y < SCREENH) enemyTemp->y = SCREENW + 100;

				}
			}
			else if (projTemp->CollisionD(playerTemp) && (projTemp->type != playerTemp->type))
			{

				// ADDED 9/8/2011
				if (playerTemp->checkCanDamage())
				{
					playerTemp->playerHealth -= 10;
					if (playerTemp->playerHealth < 0) 
					{
						playerTemp->setAlive(false); 
						playerTemp->x = NULL;
						playerTemp->y = NULL;
					}
					m_Projectile->Remove(&projTemp);
					projectilesInGame--;
					playerTemp->lastDamaged = GetTickCount();
				}
			}
			else if (projTemp->getX() > (SCREENW + 50)){m_Projectile->Remove(&projTemp); projectilesInGame--;}
		}
		projTemp = m_Projectile->GetNext(projTemp);

	}


	/*
	WaitForSingleObject( hth1, 5000 );
	
	WaitForSingleObject( hth2, 500 );



	GetExitCodeThread( hth1, &dwExitCode );
	printf( "thread 1 exited with code %u\n", dwExitCode );

	if(GetExitCodeThread(hth1, &dwExitCode) != 0)
		TerminateThread(hth1, dwExitCode);

	GetExitCodeThread( hth2, &dwExitCode );
	printf( "thread 2 exited with code %u\n", dwExitCode );

	if(GetExitCodeThread(hth2, &dwExitCode) != 0)
		TerminateThread(hth2, dwExitCode);

	CloseHandle( hth1 );
	CloseHandle( hth2 );
	
	delete o1;
	o1 = NULL;
	   

	delete o2;
	o2 = NULL;
	*/
}


int Manager::getPlayerLocationX(int inID)
{
	Player* playerTemp = m_Player->GetFirst();
	while (playerTemp->getID() != inID)
	{
		playerTemp = m_Player->GetNext(playerTemp);
	}

	return playerTemp->getX();
}

int Manager::getPlayerLocationY(int inID)
{
	Player* playerTemp = m_Player->GetFirst();
	while (playerTemp->getID() != inID)
	{
		playerTemp = m_Player->GetNext(playerTemp);
	}

	return playerTemp->getY();
}

#pragma region Constructor.Deconstructor
//////////////////////////////////
// Constructors // Destructors //
////////////////////////////////

//SPRITE CONSTRUCTOR
Sprite::Sprite() : next(NULL), canMove(true), playerHealth(100)
{
	visible = true;
	life = 100;
	x = y = 0.0f;
	frame = 0;
	columns = 1;
	width = height = 0;
	scaling = 1.0f;
	rotation = 0.0f;
	startframe = endframe = 0;
	//	direction = 1;
	starttime = delay = 0;
	velx = vely = 0.0f;
	color = D3DCOLOR_XRGB(255,255,255);

	path = new Point[30];
}

Sprite::Sprite(Identity inID, int inLife, int z): next(NULL), path(NULL), canMove(true), playerHealth(100)
{
	ID = inID;
	life = inLife;
}


Sprite::~Sprite()
{
	delete path;
}



// PLAYER CONSTRUCTOR
Player::Player(LPDIRECT3DDEVICE9 d3ddev, LPDIRECT3DTEXTURE9 tex, int inID, int inX, int inY, int inStageWidth, int inStageHeight): currentLevel(0), globalX(0)
{

	// Identifications
	type = 1;
	ID = inID;

	// Tick Settings
	damageTime = 600;
	lastDamaged = 0;
	timefired = 0;
	firetime = 500;
	respawning = false;

	movingRight = true;
	// 
	alive = true;
	texture = tex;
	stageWidth =  inStageWidth;
	stageHeight = inStageHeight;
	x = inX;
	y = inY;
	boundries.x = inStageWidth/2;
	boundries.y = inStageHeight/2;
	width = 80;
	height = 140;
	columns = 4;
	frame = 0;
	startframe = 0;
	endframe = 4;
	velx = 20.0f;
	vely = 20.0f;
	rotation = 0.0f;
	starttime = 0;
	delay = 0;
	scaling = 1.0f;
	color = D3DCOLOR_RGBA(255,255,255,255);

	D3DXCreateSprite(d3ddev, &spriteobj);
}

Player::~Player()
{
	delete spriteobj;
}

// MANAGER CONSTRUCTOR
Manager::Manager(): projectilesInGame(0), projectilesFired(0), playersCreated(0), enemiesCreated(0), score(0)
{
	m_Projectile = new LinkedList < Projectile >;
	m_Player = new LinkedList < Player >;
	m_Enemies = new LinkedList <Zombie>;
	spriteEngine = createIrrKlangDevice();
	threaded = false;
}

Manager::~Manager()
{
	delete m_Projectile;
	delete m_Player;
	delete m_Enemies;
}

// PROJECTILE CONSTRUCTOR //
Projectile::Projectile(LPDIRECT3DDEVICE9 d3ddev, LPDIRECT3DTEXTURE9 tex, int inID, int inBulletType, double inScale, int inX, int inY, int inVelX, int inVelY, int inDamage, bool inAlive, float inScaling, double inDirectionX, double inDirectionY, int inType, int inProjectileNumber)
{
	ID = inID;
	bulletType = inBulletType;
	number = inProjectileNumber;
	direction.x = inDirectionX;
	direction.y = inDirectionY;
	speed = inVelX;
	type = inType;
	texture = tex;
	alive = true;
	x = inX;
	y = inY;
	if (inScale == 1.0f)
	{
		width = 10;
		height = 10;
	}
	else
	{
		width = (10*(inScale/7.5f));
		height = (10*(inScale/7.5f));
	}
	columns = 1;
	frame = 0;
	startframe = 0;
	endframe = 4;
	velx = 6;	
	vely = 6;
	rotation = 0.0f;
	starttime = 0;
	delay = 0;
	scaling = inScale;
	color = D3DCOLOR_RGBA(255,255,255,255);

	D3DXCreateSprite(d3ddev, &spriteobj);


}

Projectile::~Projectile()
{

}

// ZOMBIE CONSTRUCTOR
Zombie::Zombie(LPDIRECT3DDEVICE9 d3ddev, LPDIRECT3DTEXTURE9 tex, int inID, int inX, int inY)
{
	type = 2;
	LPDIRECT3DDEVICE9 holdd3d3v = d3ddev;
	ID = inID;
	texture = tex;
	x = inX;
	y = inY;
	width = 366/6;
	height = 100;
	columns = 6;
	frame = 0;
	startframe = 0;
	endframe = 5;
	velx = 20.0f;
	vely = 20.0f;
	rotation = 0.0f;
	starttime = 0;
	delay = 0;
	scaling = 1.0f;
	color = D3DCOLOR_RGBA(255,255,255,255);

	D3DXCreateSprite(d3ddev, &spriteobj);
}
void Zombie::ZombieAnimate()
{
	frame++;
	if(frame>5)
	{
		frame=0;
	};
}
Zombie::~Zombie()
{
	delete spriteobj;
}

MainMenu::MainMenu(LPDIRECT3DDEVICE9 d3ddev, int inStageWidth, int inStageHeight, LPDIRECT3DTEXTURE9 tex)
{
	stageWidth = inStageWidth;
	stageHeight = inStageHeight;
	texture = tex;
	x = 0.0f;
	y = 0.0f;
	width = SCREENW;
	height = SCREENH;
	columns = 1;
	frame = 0;
	startframe = 1;
	endframe = 1;
	velx = 0.0f;
	vely = 0.0f;
	rotation = 0.0f;
	starttime = 0;
	delay = 0;
	scaling = 1.0f;
	color = D3DCOLOR_RGBA(255,255,255,255);
	D3DXCreateSprite(d3ddev, &spriteobj);
}



MainMenu::MainMenu(LPDIRECT3DDEVICE9 d3ddev, int inStageWidth, int inStageHeight, LPDIRECT3DTEXTURE9 tex, bool startButton)
{
	stageWidth = inStageWidth;
	stageHeight = inStageHeight;
	texture = tex;
	x = 275.0f;
	y = 450.0f;
	width = 123;
	height = 28;
	columns = 1;
	frame = 0;
	startframe = 1;
	endframe = 1;
	velx = 0.0f;
	vely = 0.0f;
	rotation = 0.0f;
	starttime = 0;
	delay = 0;
	scaling = 1.0f;
	color = D3DCOLOR_RGBA(255,255,255,255);
	D3DXCreateSprite(d3ddev, &spriteobj);
}



MainMenu::~MainMenu()
{
	delete spriteobj;
}
#pragma endregion Constructor.Deconstructor