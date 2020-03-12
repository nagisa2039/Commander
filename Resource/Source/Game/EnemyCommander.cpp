#include "EnemyCommander.h"
#include "Charactor.h"
#include "Input.h"
#include "Camera.h"
#include "MapCtrl.h"

EnemyCommander::EnemyCommander(std::vector<std::shared_ptr<Charactor>>& charactors, MapCtrl& mapCtrl, const Team ctrlTeam):
	Commander(charactors, mapCtrl, ctrlTeam)
{
}

EnemyCommander::~EnemyCommander()
{
}

void EnemyCommander::Update(const Input& input)
{
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
	}

	// 移動可能なキャラを探す
	for (auto charactor : _charactors)
	{
		if (charactor->GetTeam() == _ctrlTeam 
			&& charactor->GetCanMove())
		{
			_selectChar = &*charactor;
  			_selectChar->SearchAndMove();
			break;
		}
	}

	// 移動可能なｷｬﾗがいないので終了
	if (_selectChar == nullptr)
	{
		return;
	}


	return;
}

void EnemyCommander::Draw(const Camera& camera)
{
}