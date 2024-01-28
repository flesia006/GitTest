#include "Framework.h"
#include "GameScene.h"

GameScene::GameScene()
{
	//naruto = new Naruto();
	player = new Player();

//	boss = new ModelAnimator("boss");
//	boss->Pos().z -= 2200;
//	boss->Rot().y += XM_PI;
//	boss->ReadClip("boss2");

	ground = new Model("forest");
	ground->Scale() *= 10.0f;
	ground->UpdateWorld();

	sky = new SkyBox(L"Textures/Landscape/SpaceSky.dds");
	sky->Scale() *= 100.0f;
	sky->UpdateWorld();
	//RobotManager::Get()->SetTarget(naruto);

	//CAM->SetTarget(player);
	//KunaiManager::Get();
	/////
}

GameScene::~GameScene()
{
	//delete naruto;

	//RobotManager::Delete();
	//KunaiManager::Delete();
}

void GameScene::Update()
{
//	naruto->Update();
//	RobotManager::Get()->Update();
//	KunaiManager::Get()->Update();
//	boss->Update();
	//
	player->Update();
}

void GameScene::PreRender()
{
}

void GameScene::Render()
{
	// naruto->Render();
	// RobotManager::Get()->Render();
	// KunaiManager::Get()->Render();
	sky->Render();
	//boss->Render();
	//
	ground->Render();
	player->Render();
}

void GameScene::PostRender()
{
	//	naruto->PostRender();
	player->PostRender();
}

void GameScene::GUIRender()
{
	//   naruto->GUIRender();
	player->GUIRender();
//	boss->GUIRender();
}