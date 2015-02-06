#include "cEnemyController.h"

cEnemyController::cEnemyController()
{
	mEnemyFileLocation = "assets/enemies/";
}

cEnemyController::~cEnemyController()
{	
}

bool cEnemyController::Init(cArena* _arena)
{
	mInput = cInput::Instance();
	mRen = cRenderer::Instance();
	mLog = cLogger::Instance();
	mArena = _arena;
	mEnemyStartPos = mArena->GetEnemyStartPos();
	mEnemyPath = mArena->BreadthFirst(
		make_pair((int)mEnemyStartPos.x,(int)mEnemyStartPos.y),
		make_pair((int)mArena->GetEnemyExitPos().x,(int)mArena->GetEnemyExitPos().y));
	mEnemyStartPos *= mArena->GetGridSize();
	if(!LoadEnemyData()) return false;
	for(int i = 0; i < mMaxEnemiesAlive; i++) mEnemiesAlive[i] = NULL;

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
	mArena = NULL;
	mInput = NULL;
	mRen = NULL;
	mLog = NULL;
	return true;
}

void cEnemyController::Update()
{
	if(mEnemySpawnTimer.GetTicks() > (1000))
	{
		//if(mEnemiesAlive[0] == NULL) //create only one enemy
		{
			AddEnemy(mEnemyStartPos.x,mEnemyStartPos.y, 0);
			mEnemySpawnTimer.Start();
		}
	}
	for(int i = 0; i < mMaxEnemiesAlive; i++)
	{
		if(mEnemiesAlive[i] !=  NULL)
		{
			//damage arena::core //could go after update, would need another != NULL
			mArena->GetCore()->Damage(mEnemiesAlive[i]->GetPos(),30,0.1);

			//update enemy
			mEnemiesAlive[i]->Update();
			if(mEnemiesAlive[i]->GetLives() <= 0) RemoveEnemy(i);

		}
	}
}

void cEnemyController::DrawEnemies()
{
	SDL_Color col = { 0,0,0,255 };
	int l_alive = 0;

	for(int i = 0; i < mMaxEnemiesAlive; i++)
	{
		if(mEnemiesAlive[i] != NULL)
		{
			mEnemiesAlive[i]->Draw();
			l_alive++;
		}
	}

	mRen->RenderText(l_alive,34,140,0,col,NULL,SCREEN_SPACE);
	
}

void cEnemyController::DrawEnemy(Uint32 _x, Uint32 _y, Uint32 _enemy, Uint32 _space)
{
}

void cEnemyController::DrawEnemyText(Uint32 _x, Uint32 _y, Uint32 _enemy, SDL_Color _col, Uint32 _space)
{
}

void cEnemyController::AddEnemy(Uint32 _x, Uint32 _y, Uint32 _enemy_type)
{
	for(int i = 0; i < mMaxEnemiesAlive; i++)
	{
		if(mEnemiesAlive[i] == NULL)
		{
			mEnemiesAlive[i] = new cEnemy(_x,_y);
			mEnemiesAlive[i]->Init(mEnemiesData[_enemy_type].mBitmap,mArena,&mEnemiesData[_enemy_type],mEnemyPath);
			return;
		}
	}
}

void cEnemyController::RemoveEnemy(Uint32 _enemy)
{
	mEnemiesAlive[_enemy]->CleanUp();
	delete mEnemiesAlive[_enemy];
	mEnemiesAlive[_enemy] = NULL;
}

bool cEnemyController::LoadEnemyData()
{
	int l_game_speed = 120;
	for(int i = 0; i < mMaxEnemyTypes; i++)
		mEnemiesData[i].mBitmap = NULL;

	XMLDocument doc;
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
			mEnemiesData[i].mSpeed *= l_game_speed;
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