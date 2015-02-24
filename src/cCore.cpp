#include "cCore.h"

cCore::cCore(Uint32 _x, Uint32 _y) : cEntity(_x,_y)
{
}

cCore::~cCore()
{
}

//TODO: cCore may not neet pointer to cArena
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

void cCore::Update()
{
	if(mInput->GetKeyDown(SDLK_h)) Heal();
}

bool cCore::LoadCoreData()
{
	tinyxml2::XMLDocument doc;
	if(!doc.LoadFile("assets/arena/core_data.xml"))
	{
		XMLElement* l_core = doc.FirstChildElement("core")->FirstChild()->ToElement();
		int i = 0;
		for( l_core; l_core; l_core = l_core->NextSiblingElement())
		{
			l_core->QueryIntAttribute("max_lives",&mMaxLives);
			l_core->QueryFloatAttribute("start_lives",&mLives);
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
	if(++mLives > mMaxLives) mLives = (float)mMaxLives;
}

void cCore::Damage(float _damage)
{
	mLives -= _damage;
	if(mLives < 0) mLives = 0;
}

void cCore::Damage(JVector2 _origin, int _range, float _damage)
{
	if(cMaths::InRange(_origin,GetPos(),(float)_range))
		Damage(_damage);
}
