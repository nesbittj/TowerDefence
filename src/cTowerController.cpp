#include "cTowerController.h"

cTowerController::cTowerController()
{
	mTowersFileLocation = "assets/towers/";
	mTowerSelected = 0;
	mTowerEditMode = true;
}

cTowerController::~cTowerController()
{	
}

bool cTowerController::Init(cArena* _arena, cPlayer* _player)
{
	mInput = cInput::Instance();
	mRen = cRenderer::Instance();
	mLog = cLogger::Instance();
	mArena = _arena;
	mPlayer = _player;
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
		Mix_FreeChunk(mTowersData[i].mFireSound);
		mTowersData[i].mFireSound = NULL;
	}
	mPlayer = NULL;
	mArena = NULL;
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
	if(_tower < mMaxTowerTypes
	&& mTowersData[_tower].mName != "")
	{
		mTowerSelected = _tower;
		mTowerEditMode = true;
	}
	else
	{
		mTowerEditMode = false;
	}
}

void cTowerController::Update(cEnemy** const _enemies, int size_of_array)
{
	if(mInput->GetKeyDownNotRepeat(SDLK_1)) SetTowerSelected(0);
	if(mInput->GetKeyDownNotRepeat(SDLK_2)) SetTowerSelected(1);
	if(mInput->GetKeyDownNotRepeat(SDLK_3)) SetTowerSelected(2);
	if(mInput->GetKeyDownNotRepeat(SDLK_4)) SetTowerSelected(3);
	if(mInput->GetKeyDownNotRepeat(SDLK_5)) SetTowerSelected(4);
	if(mInput->GetKeyDownNotRepeat(SDLK_6)) SetTowerSelected(5);
	if(mInput->GetKeyDownNotRepeat(SDLK_7)) SetTowerSelected(6);

	if(mTowerEditMode)
	{
		if(mInput->GetMouseButtonDownNotRepeat(LEFT_MOUSE_BUTTON))
		{
			if(*mPlayer->GetMoney() >= mTowersData[GetTowerSelected()].mCost)
			{
				if(AddTower(mRen->mCamera->GetCursorX(),mRen->mCamera->GetCursorY(),GetTowerSelected()))
					mPlayer->SpendMoney(mTowersData[GetTowerSelected()].mCost);
			}
		}
		else if(mInput->GetMouseButtonDownNotRepeat(RIGHT_MOUSE_BUTTON))
			RemoveTower(mRen->mCamera->GetCursorX(),mRen->mCamera->GetCursorY());
	}

	//TODO: consider indexing/sorting to top towers in use to avoid looping through all
	//TODO: also consider binary search / bsp
	for(int i = 0; i < mMaxTowersInUse; i++)
	{
		if(mTowersInUse[i] != NULL) mTowersInUse[i]->Update(_enemies,size_of_array);
	}
}

void cTowerController::DrawTowersInUse()
{
	for(int i = 0; i < mMaxTowersInUse; i++)
		if(mTowersInUse[i] != NULL) mTowersInUse[i]->Draw();
}

void cTowerController::DrawTower(float _x, float _y, Uint32 _tower, Uint32 _space)
{
	if(_tower < mMaxTowerTypes)
		mRen->RenderTexture(mTowersData[_tower].mBitmap,_x,_y,NULL,_space);
}

void cTowerController::DrawTowerText(float _x, float _y, Uint32 _tower, SDL_Color _col, Uint32 _space)
{
	if(_tower < mMaxTowerTypes)
		mRen->RenderText(mTowersData[_tower].mName.c_str(),_x,_y,0,_col,NULL,_space);
}

/*
adds tower at world position _x,_y, of type _tower.
uses cArena::SetTowerType() with TILE_TOWER.
return true on success.
*/
bool cTowerController::AddTower(Uint32 _x, Uint32 _y, Uint32 _tower)
{
	for(int i = 0; i < mMaxTowersInUse; i++)
	{
		if(mTowersInUse[i] == NULL)
		{
			JVector2 l_world_pos((float)_x,(float)_y);
			for(int j = 0; j < mMaxTowersInUse; j++)	//TODO: use a more appropriate contriner, to be able to search by key
			{
				if(mTowersInUse[j] != NULL
				&& mTowersInUse[j]->GetX() == l_world_pos.x && mTowersInUse[j]->GetY() == l_world_pos.y) 
					return false;
			}
			//TODO: check bounds  of l_world_pos before creatingnew tower
			mTowersInUse[i] = new cTower((Uint32)l_world_pos.x,(Uint32)l_world_pos.y);
			mTowersInUse[i]->Init(mTowersData[_tower].mBitmap,&mTowersData[_tower]);
			mArena->SetTileType(l_world_pos/(float)mArena->GetGridSize(),TILE_TOWER);
			return true;
		}
	}
}

/*
remove any tower at grid world pos x,y.
sets arena tile to TILE_TYPE_RESET to set it back to its origional type
returns true on success.
*/
bool cTowerController::RemoveTower(Uint32 _x, Uint32 _y)
{
	JVector2 l_world_pos((float)_x,(float)_y);
	for(int i = 0; i < mMaxTowersInUse; i++)
	{
		if(mTowersInUse[i] != NULL
		&& (mTowersInUse[i]->GetX() == l_world_pos.x && mTowersInUse[i]->GetY() == l_world_pos.y))	
		{
			mTowersInUse[i]->CleanUp();
			delete mTowersInUse[i];
			mTowersInUse[i] = NULL;
			mArena->SetTileType(l_world_pos/(float)mArena->GetGridSize(),TILE_TYPE_RESET);
			return true;
		}
	}
	return false;
}

/*
return tower at world position _pos.
retunrs NULL if no tower.
*/
cTower* cTowerController::GetTower(JVector2 _pos)
{
	for(unsigned int i = 0; i < mMaxTowersInUse; i++)
	{
		if(mTowersInUse[i] != NULL)
		{
			if(mTowersInUse[i]->GetPos() == _pos) return mTowersInUse[i];
		}
	}
	return NULL;
}

bool cTowerController::LoadTowersData()
{
	//TODO: cosolidate into file load
	for(int i = 0; i < mMaxTowerTypes; i++)
	{
		mTowersData[i].mBitmap = NULL;
		mTowersData[i].mFireSound = NULL;
	}

	tinyxml2::XMLDocument doc;
	if(!doc.LoadFile("assets/towers/towers_data.xml"))
	{
		XMLElement* l_tower = doc.FirstChildElement("towers")->FirstChild()->ToElement();
		int i = 0;
		for( l_tower; l_tower; l_tower=l_tower->NextSiblingElement())
		{
			mTowersData[i].mName = l_tower->Attribute("name");

			mTowersData[i].mBitmap = mRen->LoadBitmap( std::string(mTowersFileLocation + l_tower->Attribute("bitmap")).c_str() );

			l_tower->QueryFloatAttribute("damage",&mTowersData[i].mDamage);
			l_tower->QueryUnsignedAttribute("range",&mTowersData[i].mRange);
			l_tower->QueryUnsignedAttribute("firefreq",&mTowersData[i].mFireFreq);
			l_tower->QueryUnsignedAttribute("firedur",&mTowersData[i].mFireDuration);
			l_tower->QueryUnsignedAttribute("cost",&mTowersData[i].mCost);

			string l_sound_location(mTowersFileLocation + l_tower->Attribute("sound"));
			if(l_sound_location != mTowersFileLocation)
			{
				SDL_RWops l_file = *SDL_RWFromFile(l_sound_location.c_str(),"rb");
				mTowersData[i].mFireSound = Mix_LoadWAV_RW(&l_file,1);
				if(!mTowersData[i].mFireSound) mLog->LogSDLError("failed to load tower sound");
			}
			else
			{
				//TODO: this could become NULL / silence if no sound file is loaded,
				//rather than sharing sound file
				if(mTowersData[0].mFireSound) mTowersData[i].mFireSound = mTowersData[0].mFireSound;
			}

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

void cTowerController::Pause()
{
	for(unsigned int i = 0; i < mMaxTowersInUse; i++)
		if(mTowersInUse[i]) mTowersInUse[i]->Pause();
}

void cTowerController::UnPause()
{
	for(unsigned int i = 0; i < mMaxTowersInUse; i++)
		if(mTowersInUse[i]) mTowersInUse[i]->UnPause();
}