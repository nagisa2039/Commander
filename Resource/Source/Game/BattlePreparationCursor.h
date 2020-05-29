#pragma once
#include "MapCursor.h"

class Charactor;

class BattlePreparationCursor :
	public MapCursor
{
private:
	Charactor* _selectChar;
	bool _end;

	void Select();

public:
	BattlePreparationCursor(MapCtrl& mapCtrl, Camera& camera);
	~BattlePreparationCursor();

	void Update(const Input& input)override;
	void Draw()override;

	void DrawsSortieMass();

	bool GetEnd()const;
};

