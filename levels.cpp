#include "levels.h"

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

extern const int SCREENW;
extern const int SCREENH;

// Reason it wont load texture is because 

// Needs Texture as Texture
// Object needs D3DXCreateSprite(d3ddev, &spriteobj)

Levels::Levels(int inLevel, string inBackground, string inForeground, string inClouds, DevPtr inDevice): globalPosition(0) // maybe use DevPtr to get the device?
{
	string s = convertInt(inLevel);

	// Script
	string line;
	ifstream backgroundfile ("..//backgrounds//" + s + ".txt");
	if (backgroundfile.is_open())
	{
		while ( backgroundfile.good() )
		{
			getline (backgroundfile,line);
			background.width = atoi(line.c_str());
			getline (backgroundfile,line);
			background.height = atoi(line.c_str());
			getline (backgroundfile,line);
			background.x = atoi(line.c_str());
			getline (backgroundfile,line);
			background.y = atoi(line.c_str());
			getline (backgroundfile,line);
			background.columns = atoi(line.c_str());
			getline (backgroundfile,line);
			background.frame = atoi(line.c_str());
			getline (backgroundfile,line);
			background.startframe = atoi(line.c_str());
			getline (backgroundfile,line);
			background.endframe = atoi(line.c_str());
			getline (backgroundfile,line);
			background.velx = atoi(line.c_str());
			getline (backgroundfile,line);
			background.vely = atoi(line.c_str());
			getline (backgroundfile,line);
			background.rotation = atoi(line.c_str());
			getline (backgroundfile,line);
			background.starttime = atoi(line.c_str());
			getline (backgroundfile,line);
			background.delay = atoi(line.c_str());
			getline (backgroundfile,line);
			background.scaling = atoi(line.c_str());
			background.colorElement = D3DCOLOR_RGBA(255,255,255,255);
		}
		backgroundfile.close();
	}


	ifstream foregroundfile ("..//foregrounds//" + s + ".txt");
	if (foregroundfile.is_open())
	{
		while ( foregroundfile.good() )
		{
			getline (foregroundfile,line);
			foreground.width = atoi(line.c_str());
			getline (foregroundfile,line);
			foreground.height = atoi(line.c_str());
			getline (foregroundfile,line);
			foreground.x = atoi(line.c_str());
			getline (foregroundfile,line);
			foreground.y = atoi(line.c_str());
			getline (foregroundfile,line);
			foreground.columns = atoi(line.c_str());
			getline (foregroundfile,line);
			foreground.frame = atoi(line.c_str());
			getline (foregroundfile,line);
			foreground.startframe = atoi(line.c_str());
			getline (foregroundfile,line);
			foreground.endframe = atoi(line.c_str());
			getline (foregroundfile,line);
			foreground.velx = atoi(line.c_str());
			getline (foregroundfile,line);
			foreground.vely = atoi(line.c_str());
			getline (foregroundfile,line);
			foreground.rotation = atoi(line.c_str());
			getline (foregroundfile,line);
			foreground.starttime = atoi(line.c_str());
			getline (foregroundfile,line);
			foreground.delay = atoi(line.c_str());
			getline (foregroundfile,line);
			foreground.scaling = atoi(line.c_str());
			foreground.colorElement = D3DCOLOR_RGBA(255,255,255,255);
		}
		foregroundfile.close();
	}

	ifstream cloudfile ("..//clouds//" + s + ".txt");
	if (cloudfile.is_open())
	{
		while ( cloudfile.good() )
		{
			getline (cloudfile,line);
			clouds.width = atoi(line.c_str());
			getline (cloudfile,line);
			clouds.height = atoi(line.c_str());
			getline (cloudfile,line);
			clouds.x = atoi(line.c_str());
			getline (cloudfile,line);
			clouds.y = atoi(line.c_str());
			getline (cloudfile,line);
			clouds.columns = atoi(line.c_str());
			getline (cloudfile,line);
			clouds.frame = atoi(line.c_str());
			getline (cloudfile,line);
			clouds.startframe = atoi(line.c_str());
			getline (cloudfile,line);
			clouds.endframe = atoi(line.c_str());
			getline (cloudfile,line);
			clouds.velx = atoi(line.c_str());
			getline (cloudfile,line);
			clouds.vely = atoi(line.c_str());
			getline (cloudfile,line);
			clouds.rotation = atoi(line.c_str());
			getline (cloudfile,line);
			clouds.starttime = atoi(line.c_str());
			getline (cloudfile,line);
			clouds.delay = atoi(line.c_str());
			getline (cloudfile,line);
			clouds.scaling = atoi(line.c_str());
			clouds.colorElement = D3DCOLOR_RGBA(255,255,255,255);
		}
		cloudfile.close();
	}
	

	level = inLevel;
	background.fileElement = inBackground;
	foreground.fileElement = inForeground;
	clouds.fileElement = inClouds;

	D3DXCreateTextureFromFile(inDevice, background.fileElement.c_str(), &background.texElement);
	D3DXCreateTextureFromFile(inDevice, foreground.fileElement.c_str(), &foreground.texElement);
	D3DXCreateTextureFromFile(inDevice, clouds.fileElement.c_str(), &clouds.texElement);

	D3DXCreateSprite(inDevice, &background.spriteElement);
	D3DXCreateSprite(inDevice, &foreground.spriteElement);
	D3DXCreateSprite(inDevice, &clouds.spriteElement);



}

Levels::~Levels()
{

}

void Levels::Sprite_Transform_Draw(enviromentElement inElement)
{

	enviromentElement tempElement = inElement;


	if (!background.visible) return;

	//calc source frame rectangle in spritesheet


	D3DXVECTOR2 scale(tempElement.scaling, tempElement.scaling);
	D3DXVECTOR2 center((tempElement.scaling * tempElement.width)/2, (tempElement.scaling * tempElement.height)/2); //finds center of CEL
	D3DXVECTOR2 trans(tempElement.x, tempElement.y);

	//create 2D transformation matrix
	D3DXMATRIX mat;
	D3DXMatrixTransformation2D(&mat, NULL, 0, &scale, &center, (float) toRadians((double)tempElement.rotation), &trans);

	//tell sprite object to use the transform
	tempElement.spriteElement->SetTransform( &mat );

	// For scaling to work properly, the rect has to be below the transformation. This is because the Matrix tranforms it then you 
	// create the drawing boundries (rectangle) with the new information.
	RECT rect;
	rect.left = (tempElement.frame % tempElement.columns) * tempElement.width; //find wrap amount - calculating the col
	rect.top = (tempElement.frame / tempElement.columns) * tempElement.height; //calculating the rows * height
	rect.right = rect.left + tempElement.width;
	rect.bottom = rect.top + tempElement.height;

	//draw the sprite frame
	tempElement.spriteElement->Begin(D3DXSPRITE_ALPHABLEND);
	tempElement.spriteElement->Draw( tempElement.texElement, &rect, NULL, NULL, tempElement.colorElement );

	D3DXMatrixIdentity( &mat );
	tempElement.spriteElement->SetTransform( &mat );
	tempElement.spriteElement->End();
}


// Converts Int To String
string Levels::convertInt(int in)
{
	std::string s;
	std::stringstream out;
	out << in;
	s = out.str();
	string levelNumber = s;

	return s;
}

void Levels::updateLevel(double inPlayerPositionX, double inPlayerPositionY)
{
	playerX = inPlayerPositionX;
	move();
}

void Levels::move()
{
	if (level == 4) {return;}
	if (playerX > (SCREENW * 0.6))
	{
		background.x -= 5;
		foreground.x -= 5;
	}
}


// Level Manager ------------------------------------------------


levelManager::levelManager(LPDIRECT3DDEVICE9 d3ddev): currentLevel(0)
{
	m_Level = new LinkedList < Levels >;
	ManagerDevice = d3ddev;
}

levelManager::~levelManager()
{
	delete m_Level;
}


// TEST drawing of background next. Impliment first.
void levelManager::AddLevel(int inLevel)
{
	int level = inLevel;
	string s = m_Level->GetFirst()->convertInt(level);
	string background = "..//backgrounds//" + s + ".png";
	string foreground = "..//foregrounds//" + s + ".png";
	string clouds =		"..//clouds//" + s + ".png";

	m_Level->Add(new Levels(level, background, foreground, clouds, ManagerDevice));
}

void levelManager::drawLevel(int inLevel)
{

		Levels* temp = m_Level->GetFirst();
		while (temp->getLevel() != inLevel){temp = m_Level->GetNext(temp);}
		m_Level->GetFirst()->Sprite_Transform_Draw(temp->getCloudsElement());
		m_Level->GetFirst()->Sprite_Transform_Draw(temp->getBackgroundElement());
		m_Level->GetFirst()->Sprite_Transform_Draw(temp->getForegroundElement());

		currentLevel = inLevel;
}

void levelManager::update(double inX, double inY)
{
	Levels* temp = m_Level->GetFirst();
	while (temp->getLevel() != currentLevel){temp = m_Level->GetNext(temp);}
	m_Level->GetThis(temp)->updateLevel(inX, inY);
	
}

