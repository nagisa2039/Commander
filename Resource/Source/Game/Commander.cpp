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