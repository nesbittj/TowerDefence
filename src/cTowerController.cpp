#include "cTowerController.h"

cTowerController::cTowerController()
{
	mTowersFileLocation = "assets/towers/";
	//mTowerSelected = 0;
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

	mGUI = cGUInamepsace::cGUI::Instance();
	mEditTowerPanel = mGUI->AddElementPanel(0,0,100,100);
	cGUInamepsace::button<cTowerController>::AddButtonToPanel<cTowerController>(mEditTowerPanel,2,5	,100,20,"Tower Upgrade 1",  this,&cTowerController::UpgradeTower,1);
	cGUInamepsace::button<cTowerController>::AddButtonToPanel<cTowerController>(mEditTowerPanel,2,30,100,20,"Tower Upgrade 2",  this,&cTowerController::UpgradeTower,2);
	cGUInamepsace::button<cTowerController>::AddButtonToPanel<cTowerController>(mEditTowerPanel,2,55,100,20,"Cancel", NULL,NULL,0);
	mAddTowerPanel = mGUI->AddElementPanel(0,0,100,100);
	cGUInamepsace::button<cTowerController>::AddButtonToPanel<cTowerController>(mAddTowerPanel,2,5	,100,20,"Tower 1",  this,&cTowerController::AddTowerCallback,0);
	cGUInamepsace::button<cTowerController>::AddButtonToPanel<cTowerController>(mAddTowerPanel,2,30	,100,20,"Tower 2",  this,&cTowerController::AddTowerCallback,1);
	cGUInamepsace::button<cTowerController>::AddButtonToPanel<cTowerController>(mAddTowerPanel,2,55	,100,20,"Cancel", NULL,NULL,0);

	mAddTowerX = 0;
	mAddTowerY = 0;
	mSelectedTower = NULL;

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
	mSelectedTower = NULL;
	mPlayer = NULL;
	mArena = NULL;
	mGUI = NULL;
	mInput = NULL;
	mRen = NULL;
	mLog = NULL;
	return true;
}

/*
set the currently selected tower from tower inventory.
*//*
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
*/

void cTowerController::Update(cEnemy** const _enemies, int size_of_array)
{

	Sint32 _x = mRen->mCamera->GetCursorX();
	Sint32 _y = mRen->mCamera->GetCursorY();
	if(mInput->GetMouseButtonReleased(LEFT_MOUSE_BUTTON))
	{
		mSelectedTower = GetTower(_x,_y);
		//if(mAddTowerPanel->GetFocus() == cGUInamepsace::GUI_FOCUS_NONE && mEditTowerPanel->GetFocus() == cGUInamepsace::GUI_FOCUS_NONE)
		{
			if(mSelectedTower)
			{
				//upgrade tower
				mEditTowerPanel->SetPos(_x,_y);
				mEditTowerPanel->SetFocus(cGUInamepsace::GUI_FOCUS_NO_MOUSE);
				mAddTowerPanel->SetFocus(cGUInamepsace::GUI_FOCUS_NONE);
			}
			else //if(IsTileClear(_x,_y)) //only need to check bounds because we already know _get_tower == NULL
			{
				//add new tower
				mAddTowerX = _x;
				mAddTowerY = _y;
				mAddTowerPanel->SetPos(mAddTowerX,mAddTowerY);
				mAddTowerPanel->SetFocus(cGUInamepsace::GUI_FOCUS_NO_MOUSE);
				mEditTowerPanel->SetFocus(cGUInamepsace::GUI_FOCUS_NONE);
				/*
				//use get money to set avaliability of new towers
				*mPlayer->GetMoney() >= mTowersData[GetTowerSelected()].mCost
				*/
				//if(AddTower(_x,_y,GetTowerSelected()))
				//	mPlayer->SpendMoney(mTowersData[GetTowerSelected()].mCost);
			}
		}
	}
	else if(mInput->GetMouseButtonDownNotRepeat(RIGHT_MOUSE_BUTTON))
		RemoveTower(_x,_y);

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

bool cTowerController::UpgradeTower(int _upgrade)
{
	mSelectedTower->UpgradeTower(_upgrade);
	mEditTowerPanel->SetFocus(cGUInamepsace::GUI_FOCUS_NONE);
	return true;
}

/*
adds tower at world position _x,_y, of type _tower.
does NOT check position validity, use IsTileClear() first.
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
			mTowersInUse[i] = new cTower((Uint32)l_world_pos.x,(Uint32)l_world_pos.y);
			mTowersInUse[i]->Init(mTowersData[_tower].mBitmap,&mTowersData[_tower]);
			mArena->SetTileType(l_world_pos/(float)mArena->GetGridSize(),TILE_TOWER);
			return true;
		}
	}
	return false;
}

bool cTowerController::AddTowerCallback(int _tower)
{
	AddTower(mAddTowerX,mAddTowerY,_tower);
	mAddTowerPanel->SetFocus(cGUInamepsace::GUI_FOCUS_NONE);
	return true;
}

/*
check validity of tile for tower placement.
checks for existing tower at position _x,_y,
checks position is in bounds.
returns true it tile is clear, false if tile not clear.
*/
bool cTowerController::IsTileClear(Uint32 _x, Uint32 _y)
{
	if(GetTower(_x,_y)) return false;
	//TODO: check bounds
	return true;
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
returns NULL if no tower.
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
				//rather than sharing single sound file
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