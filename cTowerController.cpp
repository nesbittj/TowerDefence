#include "cTowerController.h"

cTowerController::cTowerController()
{
	mTowersFileLocation = "assets/towers/";
	mTowerSelected = 0;
}

cTowerController::~cTowerController()
{	
}

bool cTowerController::Init(const Uint32 _grid_size, const cPlayer* _player)
{
	mInput = cInput::Instance();
	mRen = cRenderer::Instance();
	mLog = cLogger::Instance();
	mGridSize = _grid_size;
	mPlayer = _player;
	if(!LoadTowersData()) return false;
	for(int i = 0; i < sizeof(mTowersInUse)/sizeof(mTowersInUse[0]); i++) mTowersInUse[i] = NULL;
	return true;
}

bool cTowerController::CleanUp()
{
	for(int i = 0; i < sizeof(mTowersInUse)/sizeof(mTowersInUse[0]); i++)
	{
		if(mTowersInUse[i])
		{
			mTowersInUse[i]->CleanUp();
			delete mTowersInUse[i];
		}
		mTowersInUse[i] = NULL;
	}
	for(int i = 0; i < sizeof(mTowersData)/sizeof(mTowersData[0]); i++)
	{
		UnloadBitmap(mTowersData[i].mBitmap);
	}
	mInput = NULL;
	mRen = NULL;
	mLog = NULL;
	mPlayer = NULL;
	return true;
}

/*
set the currently selected tower from tower inventory.
*/
void cTowerController::SetTowerSelected(Uint32 _tower)
{
	if(_tower < sizeof(mTowersData)/sizeof(mTowersData[0]))
	{
		if(mTowersData[_tower].mName != "")
			mTowerSelected = _tower;
	}
}

void cTowerController::Update()
{
	if(mInput->GetKeyDownRelease(SDLK_1)) SetTowerSelected(0);
	if(mInput->GetKeyDownRelease(SDLK_2)) SetTowerSelected(1);
	if(mInput->GetKeyDownRelease(SDLK_3)) SetTowerSelected(2);
	if(mInput->GetKeyDownRelease(SDLK_4)) SetTowerSelected(3);
	if(mInput->GetKeyDownRelease(SDLK_5)) SetTowerSelected(4);
	if(mInput->GetKeyDownRelease(SDLK_6)) SetTowerSelected(5);

	if(mInput->GetMouseButtonDownRelease(1))		
		AddTower(mPlayer->GetCurserX(),mPlayer->GetCurserY(),GetTowerSelected());

	for(int i = 0; i < sizeof(mTowersInUse)/sizeof(mTowersInUse[0]); i++)
		if(mTowersInUse[i] != NULL) mTowersInUse[i]->Update();
}

void cTowerController::DrawTowersInUse()
{
	for(int i = 0; i < sizeof(mTowersInUse)/sizeof(mTowersInUse[0]); i++)
		if(mTowersInUse[i] != NULL) mTowersInUse[i]->Draw();
}

void cTowerController::DrawTower(Uint32 _x, Uint32 _y, Uint32 _tower, Uint32 _space)
{
	if(_tower < sizeof(mTowersData)/sizeof(mTowersData[0]))
		mRen->RenderTexture(mTowersData[_tower].mBitmap,_x,_y,NULL,_space);
}

void cTowerController::DrawTowerText(Uint32 _x, Uint32 _y, Uint32 _tower, SDL_Color _col, Uint32 _space)
{
	if(_tower < sizeof(mTowersData)/sizeof(mTowersData[0]))
		mRen->RenderText(mTowersData[_tower].mName.c_str(),_x,_y,0,_col,NULL,_space);
}

void cTowerController::AddTower(Uint32 _x, Uint32 _y, Uint32 _tower)
{
	for(int i = 0; i < sizeof(mTowersInUse)/sizeof(mTowersInUse[0]); i++)
	{
		if(mTowersInUse[i] == NULL)
		{
			for(int i = 0; i < sizeof(mTowersInUse)/sizeof(mTowersInUse[0]); i++)
				if(mTowersInUse[i] != NULL && mTowersInUse[i]->GetX() == _x && mTowersInUse[i]->GetY() == _y) return;
			mTowersInUse[i] = new cTower(_x - mRen->GetCamera()->GetPos().x,_y - mRen->GetCamera()->GetPos().y,mGridSize);
			mTowersInUse[i]->Init(mTowersData[_tower].mBitmap,&mTowersData[_tower]);
			return;
		}
	}
}

bool cTowerController::LoadTowersData()
{
	int l_game_speed = 120;
	for(int i = 0; i < sizeof(mTowersData)/sizeof(mTowersData[0]); i++)
		mTowersData[i].mBitmap = NULL;

	XMLDocument doc;
	if(!doc.LoadFile("assets/towers/towers_data.xml"))
	{
		XMLElement* l_tower = doc.FirstChildElement("towers")->FirstChild()->ToElement();
		int i = 0;
		for( l_tower; l_tower; l_tower=l_tower->NextSiblingElement())
		{
			mTowersData[i].mName = l_tower->Attribute("name");

			mTowersData[i].mBitmap = LoadBitmap( std::string(mTowersFileLocation + l_tower->Attribute("bitmap")).c_str() );

			l_tower->QueryIntAttribute("damage",&mTowersData[i].mDamage);
			l_tower->QueryIntAttribute("range",&mTowersData[i].mRange);
			l_tower->QueryIntAttribute("firefreq",&mTowersData[i].mFireFreq);
			l_tower->QueryIntAttribute("firedur",&mTowersData[i].mFireDuration);
			mTowersData[i].mFireFreq *= l_game_speed;
			mTowersData[i].mFireDuration *= l_game_speed;
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

SDL_Texture* cTowerController::LoadBitmap(const char* filename)
{
	SDL_Texture* result = mRen->LoadTextureFromBMP(filename,NULL);
	if(!result) 
	mLog->LogSDLError("cEntity::LoadBitmap() Error Loading Bitmap");
	return result;
}

void cTowerController::UnloadBitmap(SDL_Texture* _bitmap)
{
	if(_bitmap) SDL_DestroyTexture(_bitmap);
	_bitmap = NULL;
}