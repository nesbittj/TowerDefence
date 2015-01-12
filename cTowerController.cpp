#include "cTowerController.h"

#define forTowersInUse for(int i = 0; i < sizeof(mTowersInUse)/sizeof(mTowersInUse[0]); i++)
#define forTowersData for(int i = 0; i < sizeof(mTowersData)/sizeof(mTowersData[0]); i++)
#define ifInsideTowersData if(_tower < sizeof(mTowersData)/sizeof(mTowersData[0]))

cTowerController::cTowerController()
{
	mTowersFileLocation = "assets/towers/";
	mTowerSelected = 0;
	mInput = cInput::Instance();
	mRen = cRenderer::Instance();
	mLog = cLogger::Instance();

	forTowersInUse mTowersInUse[i] = NULL;
	forTowersData { mTowersData[i].mName = ""; mTowersData[i].mBitmap = NULL; }
}

cTowerController::~cTowerController()
{
	forTowersInUse
	{
		if(mTowersInUse[i] != NULL)
			delete mTowersInUse[i];
		mTowersInUse[i] = NULL;
	}
	forTowersData
	{
		if(mTowersData[i].mBitmap != NULL)
			SDL_DestroyTexture(mTowersData[i].mBitmap);
		mTowersData[i].mBitmap = NULL;
	}

	mInput = NULL;
}

bool cTowerController::Init(const Uint32* _grid_size, const cPlayer* _player)
{
	bool result = true;

	mGridSize = _grid_size;
	mPlayer = _player;
	result = LoadTowersData();

	return result;
}

/*
set the currently selected tower from tower inventory.
*/
void cTowerController::SetTowerSelected(Uint32 _tower)
{
	ifInsideTowersData
		if(mTowersData[_tower].mName != "")
			mTowerSelected = _tower;
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

	forTowersInUse  if(mTowersInUse[i] != NULL) mTowersInUse[i]->Update();
}

void cTowerController::DrawTowersInUse()
{
	forTowersInUse if(mTowersInUse[i] != NULL) mTowersInUse[i]->Draw();
}

void cTowerController::DrawTower(Uint32 _x, Uint32 _y, Uint32 _tower, Uint32 _space)
{
	ifInsideTowersData mRen->RenderTexture(mTowersData[_tower].mBitmap,_x,_y,NULL,_space);
}

void cTowerController::DrawTowerText(Uint32 _x, Uint32 _y, Uint32 _tower, SDL_Color _col, Uint32 _space)
{
	ifInsideTowersData mRen->RenderText(mTowersData[_tower].mName.c_str(),_x,_y,0,_col,NULL,_space);
}

void cTowerController::AddTower(Uint32 _x, Uint32 _y, Uint32 _tower)
{
	forTowersInUse
	{
		if(mTowersInUse[i] == NULL)
		{
			forTowersInUse
				if(mTowersInUse[i] != NULL && mTowersInUse[i]->GetX() == _x && mTowersInUse[i]->GetY() == _y) return;
			mTowersInUse[i] = new cTower(
				_x - mRen->GetCamera()->GetPos().x,
				_y - mRen->GetCamera()->GetPos().y,
				mTowersData[_tower].mBitmap,*mGridSize,&mTowersData[_tower]);
			return;
		}
	}
}

bool cTowerController::LoadTowersData()
{
	int l_game_speed = 120;
	XMLDocument doc;
	if(!doc.LoadFile("assets/towers/towers_data.xml"))
	{
		XMLElement* l_tower = doc.FirstChildElement("towers")->FirstChild()->ToElement();
		int i = 0;
		for( l_tower; l_tower; l_tower=l_tower->NextSiblingElement())
		{
			mTowersData[i].mName = l_tower->Attribute("name");
			mTowersData[i].mBitmap = mRen->LoadTextureFromBMP(
				std::string(mTowersFileLocation + l_tower->Attribute("bitmap")).c_str(),NULL);
			l_tower->QueryIntAttribute("damage",&mTowersData[i].mDamage);
			l_tower->QueryIntAttribute("range",&mTowersData[i].mRange);
			l_tower->QueryIntAttribute("firefreq",&mTowersData[i].mFireFreq) * l_game_speed;
			l_tower->QueryIntAttribute("firedur",&mTowersData[i].mFireDuration) * l_game_speed;
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