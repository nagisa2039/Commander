#include "EnemyCommander.h"
#include "Charactor.h"
#include "Input.h"
#include "Camera.h"
#include "MapCtrl.h"
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

	// 移動可能なキャラを探す
	for (auto charactor : _charactors)
	{
		if (charactor->GetTeam() == _ctrlTeam
			&& charactor->GetCanMove())
		{
			_selectChar = &*charactor;
			_selectChar->SetMoveStandby(15);
			_camera.AddTargetActor(_selectChar);
			_mapPos = _selectChar->GetMapPos();
			_pos = (_mapPos * _mapCtrl.GetChipSize().ToVector2Int()).ToVector2();
			return;
		}
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
		_uniqueUpdater = &EnemyCommander::NormalUpdate;
	}
}

EnemyCommander::EnemyCommander(std::vector<std::shared_ptr<Charactor>>& charactors, MapCtrl& mapCtrl, const Team ctrlTeam, Camera& camera):
	Commander(charactors, mapCtrl, ctrlTeam, camera)
{
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