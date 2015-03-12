#include "cEnemyController.h"

cEnemyController::cEnemyController()
{
	mEnemyFileLocation = "assets/enemies/";
}

cEnemyController::~cEnemyController()
{	
}

bool cEnemyController::Init(cArena* _arena, cPlayer* _player)
{
	mInput = cInput::Instance();
	mRen = cRenderer::Instance();
	mLog = cLogger::Instance();

	mArena = _arena;
	mPlayer = _player;
	mEnemyStartPos = mArena->GetEnemyStartPos();
	mEnemyStartPos *= (float)mArena->GetGridSize();

	if(!LoadEnemyData()) return false;
	for(int i = 0; i < mMaxEnemiesAlive; i++) mEnemiesAlive[i] = NULL;
	mNumEnemiesAlive = 0;

	mEnemySpawnTimer = cTimer();
	mEnemySpawnTimer.Start();

	return true;
}

bool cEnemyController::CleanUp()
{
	mEnemySpawnTimer.Stop();

	for(int i = 0; i < mMaxEnemiesAlive; i++)
	{
		if(mEnemiesAlive[i])
		{
			mEnemiesAlive[i]->CleanUp();
			delete mEnemiesAlive[i];
		}
		mEnemiesAlive[i] = NULL;
	}
	for(int i = 0; i < mMaxEnemyTypes; i++)
	{
		mRen->UnloadBitmap(mEnemiesData[i].mBitmap);
	}
	mPlayer = NULL;
	mArena = NULL;
	mInput = NULL;
	mRen = NULL;
	mLog = NULL;
	return true;
}

void cEnemyController::Update()
{
	if(mEnemySpawnTimer.GetTicks() >= (3000))
	{
		//if(mEnemiesAlive[0] == NULL) //create only one enemy at one time
		{
			AddEnemy((Uint32)mEnemyStartPos.x,(Uint32)mEnemyStartPos.y, rand() % 3); //TODO: better rand, needs a seed
			mEnemySpawnTimer.Start();
		}
	}
	for(int i = 0; i < mMaxEnemiesAlive; i++)
	{
		if(mEnemiesAlive[i] !=  NULL)
		{
			mEnemiesAlive[i]->Update();
			if(mEnemiesAlive[i]->GetLives() <= 0)
			{
				if(mEnemiesAlive[i]->GetLives() < 0)
					mPlayer->IncScore(-1);
				else
					mPlayer->IncScore(1);
				RemoveEnemy(i);
			}
		}
	}
}

void cEnemyController::DrawEnemies()
{
	SDL_Color col = { 0,0,0,255 };

	for(int i = 0; i < mMaxEnemiesAlive; i++)
	{
		if(mEnemiesAlive[i] != NULL)
		{
			mEnemiesAlive[i]->Draw();
		}
	}
}

void cEnemyController::AddEnemy(Uint32 _x, Uint32 _y, Uint32 _enemy_type)
{
	for(int i = 0; i < mMaxEnemiesAlive; i++)
	{
		if(mEnemiesAlive[i] == NULL)
		{
			mEnemiesAlive[i] = new cEnemy(_x,_y);
			mEnemiesAlive[i]->Init(mEnemiesData[_enemy_type].mBitmap,mArena,&mEnemiesData[_enemy_type]);
			mNumEnemiesAlive++;
			return;
		}
	}
}

void cEnemyController::RemoveEnemy(Uint32 _enemy)
{
	if(_enemy < 0 || _enemy > mMaxEnemiesAlive) return;
	mEnemiesAlive[_enemy]->CleanUp();
	delete mEnemiesAlive[_enemy];
	mEnemiesAlive[_enemy] = NULL;
	mNumEnemiesAlive--;
}

bool cEnemyController::LoadEnemyData()
{
	for(int i = 0; i < mMaxEnemyTypes; i++)
		mEnemiesData[i].mBitmap = NULL;

	tinyxml2::XMLDocument doc;
	if(!doc.LoadFile("assets/enemies/enemies_data.xml"))
	{
		XMLElement* l_enemy = doc.FirstChildElement("enemies")->FirstChild()->ToElement();
		int i = 0;
		for( l_enemy; l_enemy; l_enemy=l_enemy->NextSiblingElement())
		{
			mEnemiesData[i].mName = l_enemy->Attribute("name");
			mEnemiesData[i].mBitmap = mRen->LoadBitmap( std::string(mEnemyFileLocation + l_enemy->Attribute("bitmap")).c_str() );
			l_enemy->QueryIntAttribute("lives",&mEnemiesData[i].mStartingLives);
			l_enemy->QueryFloatAttribute("speed",&mEnemiesData[i].mSpeed);
			i++;
		}
	}
	else
	{
		mLog->LogError("cEnemyController XML failed to load");
		return false;
	}
	return true;
}