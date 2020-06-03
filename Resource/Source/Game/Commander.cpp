#include "Commander.h"
#include "Charactor.h"
#include "MapCtrl.h"
#include "Camera.h"

using namespace std;

Commander::Commander(std::vector<std::shared_ptr<Charactor>>& charactors, MapCtrl& mapCtrl, const Team ctrlTeam, Camera& camera)
	:_charactors(charactors), _ctrlTeam(ctrlTeam), MapCursor(mapCtrl, camera)
{
	_selectChar = nullptr;
	_pos = Vector2();
	_animCnt = 0;
	_rigid = 0;
	_end = false;
}

Commander::~Commander()
{
}

bool Commander::ChaeckCharactor()
{
	_end = true;
	for (auto& charactor : _charactors)
	{
		if (_ctrlTeam == charactor->GetTeam())
		{
			_end = _end && !charactor->GetCanMove();
		}
	}
	return _end;
}

void Commander::DrawMovableMass()
{
}

bool Commander::GetBackMapSelect() const
{
	return false;
}

void Commander::End()
{
	_end = true;
}

bool Commander::CheckEnd() const
{
	return _end;
}

void Commander::TurnReset()
{
	_end = false;
	bool setMapPos = false;
	for (auto& charactor : _charactors)
	{
		if (charactor->GetTeam() == _ctrlTeam)
		{
			charactor->TurnReset();
			if (!setMapPos)
			{
				setMapPos = true;
				_mapPos = charactor->GetMapPos();
				_pos = (_mapPos * _mapCtrl.GetChipSize().ToVector2Int()).ToVector2();
			}
		}
	}

	if (_selectChar == nullptr)
	{
		return;
	}
	_selectChar->SetIsSelect(false);
	_selectChar = nullptr;
}

Charactor* Commander::GetSelectCharactor() const
{
	return _selectChar;
}

void Commander::StartTerrainEffect()
{
	bool addCamera = true;
	for (auto& charactor : _charactors)
	{
		if (charactor->GetTeam() != _ctrlTeam)continue;

		charactor->StartTerrainEffect();
	}
}