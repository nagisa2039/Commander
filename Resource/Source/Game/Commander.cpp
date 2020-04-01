#include "Commander.h"
#include "Charactor.h"
#include "MapCtrl.h"

using namespace std;

Commander::Commander(std::vector<std::shared_ptr<Charactor>>& charactors, MapCtrl& mapCtrl, const Team ctrlTeam)
	:_charactors(charactors), _ctrlTeam(ctrlTeam), MapCursor(mapCtrl)
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

bool Commander::CheckEnd() const
{
	return _end;
}

void Commander::TurnReset()
{
	_end = false;
	for (auto& charactor : _charactors)
	{
		if (charactor->GetTeam() == _ctrlTeam)
		{
			charactor->TurnReset();
		}
	}

	if (_selectChar == nullptr)
	{
		return;
	}
	_selectChar->SetIsSelect(false);
	_selectChar = nullptr;
}
