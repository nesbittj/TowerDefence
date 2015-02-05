#include "cTowerController.h"

cTowerController::cTowerController()
{
	mTowersFileLocation = "assets/towers/";
	mTowerSelected = 0;
}

cTowerController::~cTowerController()
{	
}

bool cTowerController::Init(cArena* _arena, int* _cursor_x, int* _cursor_y)
{
	mInput = cInput::Instance();
	mRen = cRenderer::Instance();
	mLog = cLogger::Instance();
	mArena = _arena;
	mCursorX = _cursor_x; mCursorY = _cursor_y;
	if(!LoadTowersData()) return false;
	for(int i = 0; i < mMaxTowersInUse; i++) mTowersInUse[i] = NULL;
	return true;
}

bool cTowerController::CleanUp()
{
	for(int i = 0; i < mMaxTowersInUse; i++)
	{
		if(mTowersInUse[i])
		{
			mTowersInUse[i]->CleanUp();
			delete mTowersInUse[i];
		}
		mTowersInUse[i] = NULL;
	}
	for(int i = 0; i < mMaxTowerTypes; i++)
	{
		mRen->UnloadBitmap(mTowersData[i].mBitmap);
	}
	mInput = NULL;
	mRen = NULL;
	mLog = NULL;
	return true;
}

/*
set the currently selected tower from tower inventory.
*/
void cTowerController::SetTowerSelected(Uint32 _tower)
{
	if(_tower < mMaxTowerTypes)
	{
		if(mTowersData[_tower].mName != "")
			mTowerSelected = _tower;
	}
}

void cTowerController::Update(cEnemy** const _enemies_hit, int size_of_array)
{
	if(mInput->GetKeyDownRelease(SDLK_1)) SetTowerSelected(0);
	if(mInput->GetKeyDownRelease(SDLK_2)) SetTowerSelected(1);
	if(mInput->GetKeyDownRelease(SDLK_3)) SetTowerSelected(2);
	if(mInput->GetKeyDownRelease(SDLK_4)) SetTowerSelected(3);
	if(mInput->GetKeyDownRelease(SDLK_5)) SetTowerSelected(4);
	if(mInput->GetKeyDownRelease(SDLK_6)) SetTowerSelected(5);

	if(mInput->GetMouseButtonDownRelease(LEFT_MOUSE_BUTTON))	
		AddTower(*mCursorX,*mCursorY,GetTowerSelected());
	if(mInput->GetMouseButtonDownRelease(RIGHT_MOUSE_BUTTON))
		RemoveTower(*mCursorX,*mCursorY);

	for(int i = 0; i < mMaxTowersInUse; i++)
	{
		if(mTowersInUse[i] != NULL) mTowersInUse[i]->Update(_enemies_hit,size_of_array);
	}
}

void cTowerController::DrawTowersInUse()
{
	for(int i = 0; i < mMaxTowersInUse; i++)
		if(mTowersInUse[i] != NULL) mTowersInUse[i]->Draw();
}

void cTowerController::DrawTower(Uint32 _x, Uint32 _y, Uint32 _tower, Uint32 _space)
{
	if(_tower < mMaxTowerTypes)
		mRen->RenderTexture(mTowersData[_tower].mBitmap,_x,_y,NULL,_space);
}

void cTowerController::DrawTowerText(Uint32 _x, Uint32 _y, Uint32 _tower, SDL_Color _col, Uint32 _space)
{
	if(_tower < mMaxTowerTypes)
		mRen->RenderText(mTowersData[_tower].mName.c_str(),_x,_y,0,_col,NULL,_space);
}

void cTowerController::AddTower(Uint32 _x, Uint32 _y, Uint32 _tower)
{
	for(int i = 0; i < mMaxTowersInUse; i++)
	{
		if(mTowersInUse[i] == NULL)
		{
			JVector2 l_world_pos(_x,_y);
			for(int j = 0; j < mMaxTowersInUse; j++)
			{
				if(mTowersInUse[j] != NULL
				&& mTowersInUse[j]->GetX() == l_world_pos.x && mTowersInUse[j]->GetY() == l_world_pos.y) 
					return;
			}
			//TODO: check bounds  of l_world_pos before creatingnew tower
			mTowersInUse[i] = new cTower(l_world_pos.x,l_world_pos.y);
			mTowersInUse[i]->Init(mTowersData[_tower].mBitmap,&mTowersData[_tower]);
			return;
		}
	}
}

/*
remove any tower at grid world pos x,y
*/
void cTowerController::RemoveTower(Uint32 _x, Uint32 _y)
{
	JVector2 l_world_pos(_x,_y);
	for(int i = 0; i < mMaxTowersInUse; i++)
	{
		if(mTowersInUse[i] != NULL
		&& (mTowersInUse[i]->GetX() == l_world_pos.x && mTowersInUse[i]->GetY() == l_world_pos.y))	
		{
			mTowersInUse[i]->CleanUp();
			delete mTowersInUse[i];
			mTowersInUse[i] = NULL;
		}
	}
}

bool cTowerController::LoadTowersData()
{
	for(int i = 0; i < mMaxTowerTypes; i++)
		mTowersData[i].mBitmap = NULL;

	XMLDocument doc;
	if(!doc.LoadFile("assets/towers/towers_data.xml"))
	{
		XMLElement* l_tower = doc.FirstChildElement("towers")->FirstChild()->ToElement();
		int i = 0;
		for( l_tower; l_tower; l_tower=l_tower->NextSiblingElement())
		{
			mTowersData[i].mName = l_tower->Attribute("name");

			mTowersData[i].mBitmap = mRen->LoadBitmap( std::string(mTowersFileLocation + l_tower->Attribute("bitmap")).c_str() );

			l_tower->QueryFloatAttribute("damage",&mTowersData[i].mDamage);
			l_tower->QueryIntAttribute("range",&mTowersData[i].mRange);
			l_tower->QueryIntAttribute("firefreq",&mTowersData[i].mFireFreq);
			l_tower->QueryIntAttribute("firedur",&mTowersData[i].mFireDuration);
			i++;
		}
	}
	else
	{
		mLog->LogError("cTowerController XML failed to load");
		return false;
	}
	return true;
}