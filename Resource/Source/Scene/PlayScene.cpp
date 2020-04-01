#include<DxLib.h>
#include <algorithm>
#include "PlayScene.h"
#include "../Utility/Input.h"
#include "../System/Application.h"
#include "../System/FileSystem.h"
#include "../Game/Camera.h"
#include "../Game/MapCtrl.h"

#include "Swordsman.h"
#include "Soldier.h"
#include "Warrior.h"

#include "PlayerCommander.h"
#include "EnemyCommander.h"
#include "Effect.h"
#include "FlyText.h"
#include "TurnChangeAnim.h"

using namespace std;

PlayScene::PlayScene(SceneController & ctrl):Scene(ctrl)
{
	_effects.clear();
	_charactors.clear();
	_charactors.reserve(30);


	debug = true;

	_mapCtrl = make_shared<MapCtrl>(_charactors);
	_camera = make_shared<Camera>(Rect(Vector2Int(), Application::Instance().GetWindowSize()));

	_turnChangeAnim = make_shared<TurnChangeAnim>();

	_playerCommander = make_shared<PlayerCommander>(_charactors, *_mapCtrl, Team::player);
	_enemyCommander = make_shared<EnemyCommander>(_charactors, *_mapCtrl, Team::enemy);

	_charactors.emplace_back(make_shared<Swordsman>(Vector2Int(0,0),	Team::player, *_mapCtrl, _controller, _effects));
	_charactors.emplace_back(make_shared<Swordsman>(Vector2Int(1,3),	Team::player, *_mapCtrl, _controller, _effects));
	_charactors.emplace_back(make_shared<Swordsman>(Vector2Int(5, 5),	Team::enemy,  *_mapCtrl, _controller, _effects));
	_charactors.emplace_back(make_shared<Swordsman>(Vector2Int(5, 3),	Team::enemy,  *_mapCtrl, _controller, _effects));

	_camera->AddTargetActor(_playerCommander);

	auto mapSize = _mapCtrl->GetMapCnt() * _mapCtrl->GetChipSize();
	_camera->SetLimitRect(Rect(mapSize.ToVector2Int() * 0.5, mapSize));

	_mapCtrl->LoadMap("map0");

	_playerCommander->TurnReset();

	Application::Instance().GetFileSystem()->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin", 40, 1, true, true);
	Application::Instance().GetFileSystem()->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin100", 100, 1, true, true);

	// プレイヤーターンを開始
	StartPlayerTurn();
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
	}

	for (auto& charactor : _charactors)
	{
		charactor->Update(input);
	}

	(this->*_uniqueUpdater)(input);

	_camera->Update();
	for (auto& effect : _effects)
	{
		effect->Update(input);
	}
	auto newEnd = remove_if(_effects.begin(), _effects.end(),
		[](const std::shared_ptr<Effect>& effect) { return effect->GetDelete(); });
	_effects.erase(newEnd, _effects.end());
}

void PlayScene::PlayerTurnUpdate(const Input& input)
{
	_turnChangeAnim->Update(input);
	if (!_turnChangeAnim->GetAnimEnd()) return;

	if (_playerCommander->CheckEnd())
	{
		StartEnemyTurn();
		return;
	}
	_playerCommander->Update(input);
}

void PlayScene::StartPlayerTurn()
{
	_uniqueUpdater = &PlayScene::PlayerTurnUpdate;
	_uniqueDrawer = &PlayScene::PlayerTurnDraw;
	_turnChangeAnim->TurnStart(Team::player);
	_enemyCommander->TurnReset();
}

void PlayScene::StartEnemyTurn()
{
	_uniqueUpdater = &PlayScene::EnemyTurnUpdate;
	_uniqueDrawer = &PlayScene::EnemyTurnDraw;
	_turnChangeAnim->TurnStart(Team::enemy);
	_playerCommander->TurnReset();
}

void PlayScene::EnemyTurnUpdate(const Input& input)
{
	_turnChangeAnim->Update(input);
	if (!_turnChangeAnim->GetAnimEnd()) return;

	if (_enemyCommander->CheckEnd())
	{
		StartPlayerTurn();
		return;
	}
	_enemyCommander->Update(input);
}

void PlayScene::GameClearUpdate(const Input& input)
{
}

void PlayScene::GameOverUpdate(const Input& input)
{
}

void PlayScene::PlayerTurnDraw(const Camera& camera)
{
	// プレイヤーCursorの描画
	_playerCommander->Draw(*_camera);
}

void PlayScene::EnemyTurnDraw(const Camera& camera)
{
}

void PlayScene::GameOverDraw(const Camera& camera)
{
}

void PlayScene::GameClearDraw(const Camera& camera)
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

	// 場面ごとの描画
	(this->*_uniqueDrawer)(*_camera);

	// ターン交代のエフェクト描画
	_turnChangeAnim->Draw(*_camera);

	if (debug)
	{
	}
}