#include "EnemyCommander.h"
#include "Charactor.h"
#include "Input.h"
#include "Camera.h"
#include "MapCtrl.h"
#include "RouteManager.h"
#include <algorithm>

using namespace std;

void EnemyCommander::NormalUpdate(const Input& input)
{
	// 硬直
	_rigid = max(_rigid - 1, 0);
	if (_rigid > 0)  return;

	// 選択中のキャラが移動中なら選択を無効にする
	if (_selectChar != nullptr)
	{
		if (_selectChar->GetIsMoveAnim() || _selectChar->GetMoveStandby())
		{
			return;
		}
		// 移動が終わったので待機にする
		_selectChar->MoveEnd(false);
		_selectChar = nullptr;
		return;
	}

	auto setSelectCharactor = [this](std::shared_ptr<Charactor> charactor)
	{
		_selectChar = &*charactor;
		_selectChar->SetMoveStandby(15);
		_camera.AddTargetActor(_selectChar);
		_mapPos = _selectChar->GetMapPos();
		_pos = (_mapPos * _mapCtrl.GetChipSize().ToVector2Int()).ToVector2();
	};
	
	// 移動可能且つ行動範囲内にターゲットがいるキャラを探す
	std::list<std::shared_ptr<Charactor>> _movableCharList;
	for (auto charactor : _charactors)
	{
		if (charactor->GetTeam() != _ctrlTeam)continue;

		if (!charactor->GetCanMove())continue;

		if(charactor->GetRouteManager()->CheckInRageTarget())
		{
			setSelectCharactor(charactor);
			return;
		}
		else
		{
			_movableCharList.emplace_back(charactor);
		}
	}

	// 移動可能なキャラクターをセットする
	if (_movableCharList.size() > 0)
	{
		setSelectCharactor(_movableCharList.front());
		return;
	}

	// 移動可能なｷｬﾗがいないので終了
	_end = true;

	return;
}

void EnemyCommander::TerrainEffectUpdate(const Input& input)
{
	bool end = true;
	for (auto& charactor : _charactors)
	{
		if (charactor->GetTeam() != _ctrlTeam)continue;

		bool effectEnd = charactor->GetTerrainEffectEnd();
		end = end && effectEnd;
	}

	if (end)
	{
		_camera.ClearTargetActor();
		_camera.AddTargetActor(this);
		_uniqueUpdater = &EnemyCommander::NormalUpdate;
	}
}

EnemyCommander::EnemyCommander(std::vector<std::shared_ptr<Charactor>>& charactors, MapCtrl& mapCtrl, const Team ctrlTeam, Camera& camera):
	Commander(charactors, mapCtrl, ctrlTeam, camera)
{
	if (ctrlTeam == Team::player)
	{
		for (auto charactor : charactors)
		{
			if (ctrlTeam != charactor->GetTeam())continue;
			charactor->SetMoveActive(true);
		}
	}
	_uniqueUpdater = &EnemyCommander::TerrainEffectUpdate;
}

EnemyCommander::~EnemyCommander()
{
}

void EnemyCommander::Update(const Input& input)
{
	(this->*_uniqueUpdater)(input);
}

void EnemyCommander::Draw()
{
}

void EnemyCommander::StartTerrainEffectUpdate()
{
	StartTerrainEffect();
	_uniqueUpdater = &EnemyCommander::TerrainEffectUpdate;
}

void EnemyCommander::StartNormalUpdate()
{
	_uniqueUpdater = &EnemyCommander::NormalUpdate;
}