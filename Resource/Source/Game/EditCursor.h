#pragma once
#include "MapChip.h"
#include "Team.h"
#include "MapCursor.h"
#include "CharactorType.h"

class MapCtrl;

class EditCursor :
	public MapCursor
{
private:

	Map_Chip _mapChip;

	CharactorChipInf _charactorChipInf;

	int _animCnt;

	void(EditCursor::* _uniqueUpdater)(const Input& input);
	void(EditCursor::* _uniqueDrawer)(const Camera& camera);

	void MapEidtUpdate(const Input& input);
	void CharactorEditUpdate(const Input& input);

	void MapEditDraw(const Camera& camera);
	void CharactorEditDraw(const Camera& camera);

public:
	EditCursor(MapCtrl& mapCtrl);
	~EditCursor();

	void Update(const Input& input)override;
	void Draw(const Camera& camera)override;
};