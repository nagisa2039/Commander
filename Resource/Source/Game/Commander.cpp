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
}

Commander::~Commander()
{
}

bool Commander::CheckEnd() const
{
	bool end = true;
	for (auto& charactor : _charactors)
	{
		if (_ctrlTeam == charactor->GetTeam())
		{
			end = end && !charactor->GetCanMove();
		}
	}

	return end;
}

void Commander::TurnReset()
{
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
