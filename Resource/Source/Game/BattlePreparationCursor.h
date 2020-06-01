#pragma once
#include "MapCursor.h"
#include <memory>
#include <deque>
#include "TimeLine.h"

class Charactor;

class BattlePreparationCursor :
	public MapCursor
{
private:
	Charactor* _selectChar;
	std::unique_ptr<Track<float>> _exRateTrack;
	bool _end;

	void Select();

public:
	BattlePreparationCursor(MapCtrl& mapCtrl, Camera& camera);
	~BattlePreparationCursor();

	void Update(const Input& input)override;
	void Draw()override;

	void DrawsSortieMass();

	bool GetEnd()const;

	void Start();
	void SetCursorStartPosition();
};

