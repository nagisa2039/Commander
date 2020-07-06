#pragma once
#include <deque>
#include "MapChip.h"
#include "Team.h"
#include "MapCursor.h"
#include "CharactorType.h"

class UI;
class MapCtrl;

class EditCursor :
	public MapCursor
{
private:
	std::deque<std::shared_ptr<UI>> _uiDeque;

	Map_Chip _mapChip;
	CharactorChipInf _charactorChipInf;

	int _animCnt;

	void(EditCursor::* _uniqueUpdater)(const Input& input);
	void(EditCursor::* _uniqueDrawer)();

	void MapEidtUpdate(const Input& input);
	void CharactorEditUpdate(const Input& input);
	void WeaponSelectUpdate(const Input& input);

	void MapEditDraw();
	void CharactorEditDraw();
	void WeapponSelectDraw();

public:
	EditCursor(MapCtrl& mapCtrl, Camera& camera);
	~EditCursor();

	void Update(const Input& input)override;
	void CursorUpdate(const Input& input);
	void Draw()override;
};