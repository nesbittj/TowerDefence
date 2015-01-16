#include "cCore.h"

cCore::cCore(Uint32 _x, Uint32 _y,Uint32 _grid_size) : cEntity(_x,_y,_grid_size)
{
}

cCore::~cCore()
{
}

bool cCore::Init(SDL_Texture* _bitmap)
{
	if(!cEntity::Init(_bitmap)) return false;
	if(!LoadCoreData()) return false;
	return true;
}

bool cCore::CleanUp()
{
	return cEntity::CleanUp();
}

void cCore::Update(cEnemy** const _enemies, int size_of_array)
{
	float2 l_this_pos = { x,y };
	for(int i = 0; i < size_of_array; i++)
	{
		if(_enemies[i] != NULL)
		{
			float2 l_target = { _enemies[i]->GetX(),_enemies[i]->GetY() };
			//TODO: set core range and damage properly
			if(cMaths::InRange(l_this_pos,l_target,30)) Damage(1);
		}
	}
}

bool cCore::LoadCoreData()
{
	XMLDocument doc;
	if(!doc.LoadFile("assets/core_data.xml"))
	{
		XMLElement* l_core = doc.FirstChildElement("core")->FirstChild()->ToElement();
		int i = 0;
		for( l_core; l_core; l_core = l_core->NextSiblingElement())
		{
			l_core->QueryIntAttribute("max_lives",&mMaxLives);
			l_core->QueryIntAttribute("start_lives",&mLives);
			mBitmap = mRen->LoadBitmap(l_core->Attribute("bitmap"));
			i++;
		}
	}
	else
	{
		mLog->LogError("cCore XML failed to load");
		return false;
	}
	return true;
}

void cCore::Heal()
{
	if(++mLives > mMaxLives) mLives = mMaxLives;
}

void cCore::Damage(int _damage)
{
	mLives -= _damage;
	if(mLives < 0) mLives = 0;
}