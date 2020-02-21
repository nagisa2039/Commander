#include<DxLib.h>
#include <algorithm>
#include "PlayScene.h"
#include "../Utility/Input.h"
#include "../System/Application.h"
#include "../Utility/Geometry.h"
#include "../System/FileSystem.h"
#include "../System/ImageLoader.h"

using namespace std;

void PlayScene::PlayUpdate(const Input& input)
{
	// デバッグ
	if (input.GetButtonDown(0, "debug"))
	{
		debug = !debug;
	}
}
PlayScene::PlayScene(SceneController & ctrl):Scene(ctrl)
{
	_updater = &PlayScene::PlayUpdate;
	debug = false; 
}


PlayScene::~PlayScene()
{
}

void PlayScene::Update(const Input & input)
{
	(this->*_updater)(input);
}

void PlayScene::Draw(void)
{
	DrawBox(0, 0, 50, 50, 0x114514, true);

	if (debug)
	{
	}
}