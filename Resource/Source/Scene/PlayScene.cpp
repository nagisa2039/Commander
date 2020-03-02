#include<DxLib.h>
#include <algorithm>
#include "PlayScene.h"
#include "../Utility/Input.h"
#include "../System/Application.h"
#include "../System/FileSystem.h"
#include "../Game/Camera.h"
#include "../Game/MapCtrl.h"
#include "../Game/Swordsman.h"
#include "PlayerCommander.h"
#include "Effect.h"
#include "FlyText.h"

using namespace std;

PlayScene::PlayScene(SceneController & ctrl):Scene(ctrl)
{
	_effects.clear();
	_charactors.clear();
	_charactors.reserve(30);
	_turnUpdater = &PlayScene::PlayerTurn;
	debug = false; 

	_mapCtrl = make_shared<MapCtrl>(_charactors);
	_camera = make_shared<Camera>(Rect(Vector2Int(), Application::Instance().GetWindowSize()));
	_playerCommander = make_shared<PlayerCommander>(_charactors, *_mapCtrl, Team::player);

	_charactors.emplace_back(make_shared<Swordsman>(Vector2Int(0,0),	Team::player, *_mapCtrl, _controller, _effects));
	_charactors.emplace_back(make_shared<Swordsman>(Vector2Int(1,3),	Team::player, *_mapCtrl, _controller, _effects));
	_charactors.emplace_back(make_shared<Swordsman>(Vector2Int(4, 5),	Team::enemy,  *_mapCtrl, _controller, _effects));
	_charactors.emplace_back(make_shared<Swordsman>(Vector2Int(4, 7),	Team::enemy,  *_mapCtrl, _controller, _effects));

	_camera->AddTargetActor(_playerCommander);

	auto mapSize = _mapCtrl->GetMapCnt() * _mapCtrl->GetChipSize();
	//_camera->SetLimitRect(Rect(mapSize.ToVector2Int() * 0.5, mapSize));

	_mapCtrl->LoadMap("map0");

	_playerCommander->TurnReset();

	Application::Instance().GetFileSystem()->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin", 40, 1, true, true);
}

PlayScene::~PlayScene()
{
}

void PlayScene::Update(const Input & input)
{
	// デバッグ
	if (input.GetButtonDown(0, "debug"))
	{
		debug = !debug;
	}

	if (debug)
	{
		if (input.GetButtonDown(0, "ok"))
		{
			
		}
	}

	(this->*_turnUpdater)(input);

	_camera->Update();
	for (auto& effect : _effects)
	{
		effect->Update(input);
	}
	auto newEnd = remove_if(_effects.begin(), _effects.end(),
		[](const std::shared_ptr<Effect>& effect) { return effect->GetDelete(); });
	_effects.erase(newEnd, _effects.end());
}

void PlayScene::PlayerTurn(const Input& input)
{
	bool end = true;
	for (auto& charactor : _charactors)
	{
		charactor->Update(input);
		if (Team::player == charactor->GetTeam())
		{
			end = end && !charactor->GetCanMove();
		}
		
	}
	if (end)
	{
		_playerCommander->TurnReset();
	}
	_playerCommander->Update(input);
}

void PlayScene::EnemyTurn(const Input& input)
{
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
	_playerCommander->Draw(*_camera);


	if (debug)
	{
	}
}