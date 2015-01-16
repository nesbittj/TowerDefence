#include "cEnemyController.h"

cEnemyController::cEnemyController()
{
	mEnemyFileLocation = "assets/enemies/";
}

cEnemyController::~cEnemyController()
{	
}

bool cEnemyController::Init(const Uint32 _grid_size)
{
	mInput = cInput::Instance();
	mRen = cRenderer::Instance();
	mLog = cLogger::Instance();
	mGridSize = _grid_size;
	if(!LoadEnemyData()) return false;
	for(int i = 0; i < mMaxEnemiesAlive; i++) mEnemiesAlive[i] = NULL;

	mEnemySpawnTimer = cTimer();
	mEnemySpawnTimer.start();

	return true;
}

bool cEnemyController::CleanUp()
{
	mEnemySpawnTimer.stop();

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
	mInput = NULL;
	mRen = NULL;
	mLog = NULL;
	return true;
}

void cEnemyController::Update(float2 _target)
{
	if(mEnemySpawnTimer.getTicks() > (120 * 20))
	{
		//TODO: set up addenmemy spawn loacation properly
		AddEnemy(0,0,0);
		mEnemySpawnTimer.start();
	}
	for(int i = 0; i < mMaxEnemiesAlive; i++)
	{
		if(mEnemiesAlive[i] !=  NULL)
		{
			mEnemiesAlive[i]->Update(_target);
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

	//mRen->RenderText(l_alive,34,110,0,col,NULL,SCREEN_SPACE);
	
}

void cEnemyController::DrawEnemy(Uint32 _x, Uint32 _y, Uint32 _enemy, Uint32 _space)
{
}

void cEnemyController::DrawEnemyText(Uint32 _x, Uint32 _y, Uint32 _enemy, SDL_Color _col, Uint32 _space)
{
}

void cEnemyController::AddEnemy(Uint32 _x, Uint32 _y, Uint32 _enemy)
{
	for(int i = 0; i < mMaxEnemiesAlive; i++)
	{
		if(mEnemiesAlive[i] == NULL)
		{
			for(int j = 0; j < mMaxEnemiesAlive; j++)
				if(mEnemiesAlive[j] != NULL && mEnemiesAlive[j]->GetX() == _x && mEnemiesAlive[j]->GetY() == _y) return;
			mEnemiesAlive[i] = new cEnemy(_x,_y,mGridSize);
			mEnemiesAlive[i]->Init(mEnemiesData[_enemy].mBitmap,&mEnemiesData[_enemy]);
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
			l_enemy->QueryIntAttribute("speed",&mEnemiesData[i].mSpeed);
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