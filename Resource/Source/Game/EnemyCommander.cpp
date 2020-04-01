#include "EnemyCommander.h"
#include "Charactor.h"
#include "Input.h"
#include "Camera.h"
#include "MapCtrl.h"
#include <algorithm>

using namespace std;

EnemyCommander::EnemyCommander(std::vector<std::shared_ptr<Charactor>>& charactors, MapCtrl& mapCtrl, const Team ctrlTeam):
	Commander(charactors, mapCtrl, ctrlTeam)
{
}

EnemyCommander::~EnemyCommander()
{
}

void EnemyCommander::Update(const Input& input)
{
	// 硬直
	_rigid = std::max(_rigid - 1, 0);
	if (_rigid > 0)  return;

	// 選択中のキャラが移動中なら選択を無効にする
	if (_selectChar != nullptr)
	{
		if (_selectChar->GetIsMoveAnim())
		{
			return;
		}
		// 移動が終わったので待機にする
		_selectChar->MoveEnd();
		_selectChar = nullptr;
		_rigid = 30;
		return;
	}

	// 移動可能なキャラを探す
	for (auto charactor : _charactors)
	{
		if (charactor->GetTeam() == _ctrlTeam 
			&& charactor->GetCanMove())
		{
			_selectChar = &*charactor;
  			_selectChar->SearchAndMove();
			return;
		}
	}

	// 移動可能なｷｬﾗがいないので終了
	_end = true;

	return;
}

void EnemyCommander::Draw(const Camera& camera)
{
}