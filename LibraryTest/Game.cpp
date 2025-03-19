#include "Game.h"


Game::Game() {}

Game::~Game() {}
Game* Game::GetInstance()
{
	static Game game;
	return &game;
}


void Game::Run()
{

	//MelLib::ImguiManager::GetInstance()->SetReleaseDrawFlag(true);

	Initialize();

	while (1)
	{
		//MelLib::Library::LoopStartProcess();

		//if (MelLib::Input::KeyTrigger(DIK_ESCAPE))MelLib::Library::EndFlagTrue();
		//if (MelLib::Library::GetIsEnd())break;
		
		//ImGui::ShowDemoWindow();

		Update();
		Draw();
		//MelLib::Library::LoopEndProcess();

	}

	Finalize();
}

void Game::Initialize()
{

	//MelLib::Library::Initialize(1920, 1080, MelLib::Color(30,30,160,255),L"MELLib");
	//MelLib::Library::SetFramesPerSecond60(true);


}


void Game::Finalize()
{
	//MelLib::Library::Finalize();//ê‚ëŒÇ…ç≈å„Ç…èëÇ≠
}

void Game::Update()
{


}

void Game::Draw()
{
	

}