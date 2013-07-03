#include "objects.h"

#include <iostream>
#include <fstream>
#include <string>
using namespace std;



//Constructor
Objects::Objects(int inLevel, DevPtr inDevice): objectsUsed(0), nextObject(0) // maybe use DevPtr to get the device?
{
	objectDevice = inDevice;
	level = inLevel;
}

//Destructor
Objects::~Objects()
{

}

//Add an object into the level that is Time based such as a powerup
void Objects::addObjectByTimer( int inObjectType, int inPowerUpType, int inStatic, int tickIn, int ticksAlive, double posY)
{
	// Converts the level to a string value.
	string l = convertInt(level);
	string type;
	string power;
	string objnum = convertInt(nextObject);

	bool created = false;
	if (objectsUsed <= 255)
	{


		object[nextObject].render = false;
		object[nextObject].renderType = TICK;
		object[nextObject].type = inObjectType;
		object[nextObject].power = inPowerUpType;
		object[nextObject].staticObject = inStatic;
		object[nextObject].currentTick = GetTickCount();
		object[nextObject].removeTick = ticksAlive + tickIn;

		string file = getAttributes(object[nextObject], level) + convertInt(nextObject) + ".txt";
		ifstream openObject (file);
		string info;


		// Scripting (Uses information from file)
		getline (openObject, info);
		object[nextObject].y = atoi(info.c_str());
		getline (openObject, info);
		object[nextObject].x = atoi(info.c_str());
		getline (openObject, info);
		object[nextObject].aliveTime = atoi(info.c_str());
		getline (openObject, info);
		object[nextObject].enterTick = atoi(info.c_str());

		// Sprite Render Information
		getline (openObject, info);
		object[nextObject].width = atoi(info.c_str());
		getline (openObject, info);
		object[nextObject].height = atoi(info.c_str());
		getline (openObject, info);
		object[nextObject].columns = atoi(info.c_str());
		getline (openObject, info);
		object[nextObject].frame = atoi(info.c_str());
		getline (openObject, info);
		object[nextObject].startframe = atoi(info.c_str());
		getline (openObject, info);
		object[nextObject].endframe = atoi(info.c_str());
		getline (openObject, info);
		object[nextObject].velx = atoi(info.c_str());
		getline (openObject, info);
		object[nextObject].vely = atoi(info.c_str());
		getline (openObject, info);
		object[nextObject].rotation = atoi(info.c_str());
		getline (openObject, info);
		object[nextObject].starttime = atoi(info.c_str());
		getline (openObject, info);
		object[nextObject].delay = atoi(info.c_str());
		getline (openObject, info);
		object[nextObject].scaling = atoi(info.c_str());


		// Sprite Texture
		object[nextObject].colorElement = color;
		object[nextObject].fileElement = getFile(object[nextObject]);
		object[nextObject].colorElement = D3DCOLOR_RGBA(255,255,255,255);
		D3DXCreateTextureFromFile(objectDevice, object[nextObject].fileElement.c_str(), &object[nextObject].texElement);
		D3DXCreateSprite(objectDevice, &object[nextObject].spriteElement);
		created = true;
	}
	if (created) objectsUsed += 1;
	nextObject = objectsUsed;
}

// Object is created using scripting engine that pulls line by line information about the object.
// Type is Static or Powerup
// Power is Type of Powerup
// StaticObject is which object if is static
void Objects::addObjectByPosition( int inObjectType, int inPowerUpType, int inStatic, int inStartX, int inStartY)
{

	bool created = false;
	if (objectsUsed <= 255)
	{


		object[nextObject].render = false;
		object[nextObject].renderType = POSITION;
		object[nextObject].type = inObjectType;
		object[nextObject].power = inPowerUpType;
		object[nextObject].staticObject = inStatic;
		object[nextObject].currentTick = GetTickCount();

		// Placement Information
		string file = getAttributes(object[nextObject], level) + convertInt(nextObject) + ".txt";
		ifstream openObject (file);
		string info;

		object[nextObject].x = inStartX;
		getline (openObject, info);
		object[nextObject].y = atoi(info.c_str());

		// Sprite Render Information
		getline (openObject, info);
		object[nextObject].width = atoi(info.c_str());
		getline (openObject, info);
		object[nextObject].height = atoi(info.c_str());
		getline (openObject, info);
		object[nextObject].columns = atoi(info.c_str());
		getline (openObject, info);
		object[nextObject].frame = atoi(info.c_str());
		getline (openObject, info);
		object[nextObject].startframe = atoi(info.c_str());
		getline (openObject, info);
		object[nextObject].endframe = atoi(info.c_str());
		getline (openObject, info);
		object[nextObject].velx = atoi(info.c_str());
		getline (openObject, info);
		object[nextObject].vely = atoi(info.c_str());
		getline (openObject, info);
		object[nextObject].rotation = atoi(info.c_str());
		getline (openObject, info);
		object[nextObject].starttime = atoi(info.c_str());
		getline (openObject, info);
		object[nextObject].delay = atoi(info.c_str());
		getline (openObject, info);
		object[nextObject].scaling = atoi(info.c_str());
		getline (openObject, info);

		// Sprite Texture
		object[nextObject].colorElement = color;
		object[nextObject].fileElement = getFile(object[nextObject]);
		object[nextObject].colorElement = D3DCOLOR_RGBA(255,255,255,255);
		D3DXCreateTextureFromFile(objectDevice, object[nextObject].fileElement.c_str(), &object[nextObject].texElement);
		D3DXCreateSprite(objectDevice, &object[nextObject].spriteElement);
		created = true;
	}
	if (created) objectsUsed += 1;
	nextObject = objectsUsed;

}

string Objects::getFile(objectElement inObject)
{
	int ndx = 0;
	string file;
	if (inObject.type == NULL) return file;
	if (inObject.type == STATIC)
	{
		ifstream objectfile ("..//objects//static//ObjectFileNames.txt");

		if (objectfile.is_open())
		{
			while (objectfile.good())
			{		
				while (ndx <= inObject.staticObject)
				{
					getline (objectfile, file);
					ndx++;
				}						
				objectfile.close();
			}
		}		
	}

	if (inObject.type == POWERUP)
	{
		if (inObject.power == NULL) return file;

		// Opens Text File for Powerups, then depending on the Powerups #DEFINE number / name it cycles through the lines
		// in the text file. Each line will correspond to a file for the sprite so it can load the picture.
		ifstream powerfile ("..//objects//powerup//PowerupFileNames.txt");

		if (powerfile.is_open())
		{
			while (powerfile.good())
			{	
				while (ndx <= inObject.power)
				{
					getline (powerfile, file);
					ndx++;
				}						
				powerfile.close();
			}
		}
	}

	return file;
}

// Gets the attribues by importing information from text files about the objects
// (Essentially a scripting engine)
string Objects::getAttributes(objectElement inObject, int inLevel)
{

	int ndx = 0;
	string levelstring = convertInt(inLevel);
	string file;

	// Gets type of object and branches out via tree like structure to select file.
	// Then returns the file by using the level folder, type and object.
	if (inObject.type == NULL) return file;
	if (inObject.type == STATIC) file = "..//objects//static//" + levelstring + "//";


	if (inObject.type == POWERUP)
	{
		if (inObject.power == NULL) return file;
		if (inObject.power == HEALTH) file = "..//objects//powerup//" + levelstring + "//";
		if (inObject.power == AMMO)   file = "..//objects//powerup//" + levelstring + "//";
	}

	return file;
}

// Checks if the object should be rendered
void Objects::checkRender()
{
	for (int ndx = 0; ndx <= objectsUsed; ndx++)
	{
		// If the render type is Tick it checks if the current tick is greater than the tick
		// when the object is supposed to enter.
		if (object[ndx].renderType == TICK) {
			//if (object[ndx].currentTick >= object[ndx].enterTick){ object[ndx].render = true;}

			//if (object[ndx].currentTick < object[ndx].enterTick){ object[ndx].render = false; break;}
			//if (object[ndx].x > object[ndx].stageWidth) {object[ndx].render = false; break;}
			//if (object[ndx].y > object[ndx].stageHeight) {object[ndx].render = false; break;}
			//if (object[ndx].x < 0) {object[ndx].render = false; break;}
			//if (object[ndx].y < 0) {object[ndx].render = false; break;}
			//if (object[ndx].currentTick >= object[ndx].removeTick) { object[ndx].render = false; break;}

		}

		// Checks if the Player is at the relative position to the objects.
		if (object[ndx].render == POSITION)
		{
			//if (object[ndx].globalStart.x < (globalPosition.x + object[ndx].stageWidth)){object[ndx].render = true;}

			if (object[ndx].x > object[ndx].stageWidth) {object[ndx].render = false; break;}
			if (object[ndx].y > object[ndx].stageHeight) {object[ndx].render = false; break;}
			if (object[ndx].x < 0) {object[ndx].render = false; break;}
			if (object[ndx].y < 0) {object[ndx].render = false; break;}
		}

	}
}



void Objects::Sprite_Transform_DrawAll()
{
	// Objects Used starts at 0
	// Iterates through all the objects used to draw them
	for (int ndx = 0; ndx < objectsUsed; ndx++){

		// If the Object is set to not be rendered it will not render and skip this entire part
		if (object[ndx].render)
		{

			if (!object[ndx].visible) return;

			//calc source frame rectangle in spritesheet
			RECT rect;
			rect.left = (object[ndx].frame % object[ndx].columns) * object[ndx].width; //find wrap amount - calculating the col
			rect.top = (object[ndx].frame / object[ndx].columns) * object[ndx].height; //calculating the rows * height
			rect.right = rect.left + object[ndx].width;
			rect.bottom = rect.top + object[ndx].height;

			D3DXVECTOR2 scale(object[ndx].scaling, object[ndx].scaling);
			D3DXVECTOR2 center((object[ndx].scaling * object[ndx].width)/2, (object[ndx].scaling * object[ndx].height)/2); //finds center of CEL
			D3DXVECTOR2 trans(object[ndx].x, object[ndx].y);

			//create 2D transformation matrix
			D3DXMATRIX mat;
			D3DXMatrixTransformation2D(&mat, NULL, 0, &scale, &center, (float) toRadians((double)object[ndx].rotation), &trans);

			//tell sprite object to use the transform
			object[ndx].spriteElement->SetTransform( &mat );


			//draw the sprite frame
			object[ndx].spriteElement->Begin(D3DXSPRITE_ALPHABLEND);
			object[ndx].spriteElement->Draw( object[ndx].texElement, &rect, NULL, NULL, object[ndx].colorElement );

			D3DXMatrixIdentity( &mat );
			object[ndx].spriteElement->SetTransform( &mat );
			object[ndx].spriteElement->End();
		}
	}
}

void Objects::Sprite_Transform_Draw(objectElement inElement)
{
	

	objectElement tempElement = inElement;
	if (tempElement.render) {

	if (!tempElement.visible) return;

	//calc source frame rectangle in spritesheet
	RECT rect;
	rect.left = (tempElement.frame % tempElement.columns) * tempElement.width; //find wrap amount - calculating the col
	rect.top = (tempElement.frame / tempElement.columns) * tempElement.height; //calculating the rows * height
	rect.right = rect.left + tempElement.width;
	rect.bottom = rect.top + tempElement.height;

	D3DXVECTOR2 scale(tempElement.scaling, tempElement.scaling);
	D3DXVECTOR2 center((tempElement.scaling * tempElement.width)/2, (tempElement.scaling * tempElement.height)/2); //finds center of CEL
	D3DXVECTOR2 trans(tempElement.x, tempElement.y);

	//create 2D transformation matrix
	D3DXMATRIX mat;
	D3DXMatrixTransformation2D(&mat, NULL, 0, &scale, &center, (float) toRadians((double)tempElement.rotation), &trans);

	//tell sprite object to use the transform
	tempElement.spriteElement->SetTransform( &mat );


	//draw the sprite frame
	tempElement.spriteElement->Begin(D3DXSPRITE_ALPHABLEND);
	tempElement.spriteElement->Draw( tempElement.texElement, &rect, NULL, NULL, tempElement.colorElement );

	D3DXMatrixIdentity( &mat );
	tempElement.spriteElement->SetTransform( &mat );
	tempElement.spriteElement->End();
	}
}


// Converts Int To String
string Objects::convertInt(int in)
{
	std::string s;
	std::stringstream out;
	out << in;
	s = out.str();
	string objectNumber = s;

	return s;
}



void Objects::updateObject(bool inMove, double inPlayerPositionX, double inPlayerPositionY)
{
	playerX = inPlayerPositionX;
	for (int ndx = 0; ndx <= objectsUsed; ndx++)
	{
		if (object[ndx].x <= SCREENW) {object[ndx].render = true;}

		if (object[ndx].alive == false) 
		{
			object[ndx].render = false; 
			object[ndx].canMove = false;
		}

		object[ndx].currentTick = GetTickCount();


		if (object[ndx].render) {		

			if (object[ndx].renderType == TICK) 
			{
				object[ndx].remainingTick = object[ndx].removeTick - object[ndx].currentTick;
			}
			if (inMove) move(ndx);
		}

		if (object[ndx].renderType == POSITION) 
		{
			move(ndx);
		}
	}
	checkRender();
}

void Objects::move(int inObject)
{
	if (playerX > (400))
	{

		object[inObject].x -= 5;
	}
}

void Objects::drawAll()
{
	for (int ndx = 0; ndx == objectsUsed; ndx++)
	{
		Sprite_Transform_Draw(object[ndx]);
	}
}

// Level Manager ------------------------------------------------


objectManager::objectManager(LPDIRECT3DDEVICE9 d3ddev): currentLevel(0)
{
	m_Object = new LinkedList < Objects >;
	ManagerDevice = d3ddev;
}

objectManager::~objectManager()
{
	delete m_Object;
}


// TEST drawing of object next. Impliment first.
void objectManager::AddLevel(int inLevel)
{
	int level = inLevel;

	m_Object->Add(new Objects(level, ManagerDevice));
}

void objectManager::AddObjectTick(int inLevel, int inType, int inPowerUp, int inStatic, int tickIn, int ticksAlive, int inY)
{
	Objects* temp = m_Object->GetFirst();
	while (temp->getLevel() != inLevel) {temp = m_Object->GetNext(temp);}

	temp->addObjectByTimer(inType, inPowerUp, inStatic, tickIn, ticksAlive, inY);	
}

void objectManager::AddObjectPosition(int inLevel, int inType, int inPowerUp, int inStatic, int inStartX, int inStartY)
{
	Objects* temp = m_Object->GetFirst();
	while (temp->getLevel() != inLevel) {temp = m_Object->GetNext(temp);}

	temp->addObjectByPosition(inType, inPowerUp, inStatic, inStartX, inStartY);

}
//void objectManager::AddObjectPosition(Objects* inObject, int inLevel,  int inPowerup, int inRenderType, int inStartX, int inStartY)
//{



//}

void objectManager::drawObjects(int inLevel)
{

	Objects* temp = m_Object->GetFirst();
	while (temp)
	{
		m_Object->GetFirst()->drawAll();
	}
	currentLevel = inLevel;
}

void objectManager::update(bool inMove, int inLevel, double inX, double inPlayerVelocity)
{
	Objects* temp = m_Object->GetFirst();
	while (temp->getLevel() != inLevel) {temp = m_Object->GetNext(temp);}
	temp->updateObject(inMove, inX, inX);

}

Objects objectManager::getLevel(int inLevel)
{
	Objects* temp = m_Object->GetFirst();
	while (temp->getLevel() != inLevel){ temp = m_Object->GetNext(temp);}
	return *temp;
}

