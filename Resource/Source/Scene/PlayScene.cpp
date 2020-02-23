#include<DxLib.h>
#include <algorithm>
#include "PlayScene.h"
#include "../Utility/Input.h"
#include "../System/Application.h"
#include "../System/FileSystem.h"
#include "../Game/Camera.h"
#include "../Game/MapCtrl.h"
#include "../Game/Swordsman.h"
#include "../Game/PlayerCursor.h"

using namespace std;

void PlayScene::PlayUpdate(const Input& input)
{
	// デバッグ
	if (input.GetButtonDown(0, "debug"))
	{
		debug = !debug;
	}

	_camera->Update();
	for (auto& charactor : _charactors)
	{
		charactor->Update(input);
	}
	_playerCursor->Update(input);
}
PlayScene::PlayScene(SceneController & ctrl):Scene(ctrl)
{
	_charactors.clear();
	_charactors.reserve(30);
	_updater = &PlayScene::PlayUpdate;
	debug = false; 

	_mapCtrl = make_shared<MapCtrl>(_charactors);
	_camera = make_shared<Camera>(Rect(Vector2Int(), Application::Instance().GetWindowSize()));
	_playerCursor = make_shared<PlayerCursor>(_charactors, *_mapCtrl);

	_charactors.emplace_back(make_shared<Swordsman>(Vector2Int(), Charactor::Team::Team_Blue, *_mapCtrl));

	_camera->AddTargetActor(_charactors[0]);
	_camera->RemoveTargetActor(_charactors[0]);
	_camera->AddTargetActor(_playerCursor);
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

	_mapCtrl->Draw(*_camera);
	for (auto& charactor : _charactors)
	{
		charactor->Draw(*_camera);
	}
	_playerCursor->Draw(*_camera);

	if (debug)
	{
	}
}