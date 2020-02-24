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
#include "Effect.h"

using namespace std;

void PlayScene::PlayUpdate(const Input& input)
{
	// デバッグ
	if (input.GetButtonDown(0, "debug"))
	{
		debug = !debug;
	}

	_camera->Update();
	int cnt = 0;
	for (auto& charactor : _charactors)
	{
		charactor->Update(input);
		if (_turn == charactor->GetTeam() 
		&& charactor->GetCanMove())
		{
			cnt++;
		}
	}
	if (cnt <= 0)
	{
		TurnReset(Team::Player);
		_playerCursor->TurnReset();
	}
	_playerCursor->Update(input);

	for (auto& effect : _effects)
	{
		effect->Update(input);
	}
	auto newEnd = remove_if(_effects.begin(), _effects.end(), 
		[](const std::shared_ptr<Effect>& effect){ return effect->GetDelete(); });
	_effects.erase(newEnd, _effects.end());

}
PlayScene::PlayScene(SceneController & ctrl):Scene(ctrl)
{
	_effects.clear();
	_charactors.clear();
	_charactors.reserve(30);
	_updater = &PlayScene::PlayUpdate;
	debug = false; 

	_mapCtrl = make_shared<MapCtrl>(_charactors);
	_camera = make_shared<Camera>(Rect(Vector2Int(), Application::Instance().GetWindowSize()));
	_playerCursor = make_shared<PlayerCursor>(_charactors, *_mapCtrl, Team::Player);

	_charactors.emplace_back(make_shared<Swordsman>(Vector2Int(0,0),	Team::Player, *_mapCtrl, _controller, _effects));
	_charactors.emplace_back(make_shared<Swordsman>(Vector2Int(1,3),	Team::Player, *_mapCtrl, _controller, _effects));
	_charactors.emplace_back(make_shared<Swordsman>(Vector2Int(4, 5),	Team::Enemy,  *_mapCtrl, _controller, _effects));
	_charactors.emplace_back(make_shared<Swordsman>(Vector2Int(4, 7),	Team::Enemy,  *_mapCtrl, _controller, _effects));

	_camera->AddTargetActor(_playerCursor);

	auto mapSize = _mapCtrl->GetMapCnt() * _mapCtrl->GetChipSize();
	_camera->SetLimitRect(Rect(mapSize.ToVector2Int() * 0.5, mapSize));

	_mapCtrl->LoadMap("map0");

	TurnReset(Team::Player);

}



PlayScene::~PlayScene()
{
}

void PlayScene::TurnReset(const Team turn)
{
	_turn = turn;
	for (auto charactor : _charactors)
	{
		if (_turn == charactor->GetTeam())
		{
			charactor->TurnReset();
		}
	}
}

void PlayScene::Update(const Input & input)
{
	(this->*_updater)(input);
}

void PlayScene::Draw(void)
{
	_mapCtrl->Draw(*_camera);
	for (auto& charactor : _charactors)
	{
		charactor->Draw(*_camera);
	}
	for (auto& effect : _effects)
	{
		effect->Draw(*_camera);
	}
	_playerCursor->Draw(*_camera);

	if (debug)
	{
	}
}