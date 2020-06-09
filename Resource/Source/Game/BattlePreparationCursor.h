#pragma once
#include "MapCursor.h"
#include <memory>
#include <deque>
#include "TimeLine.h"

class Charactor;
class TerrainInf;
class StatusInf;
class UI;

class BattlePreparationCursor :
	public MapCursor
{
private:
	Charactor* _selectChar;
	std::unique_ptr<Track<float>> _exRateTrack;
	bool _end;

	void Select();

	std::shared_ptr<TerrainInf> _terrainInf;
	std::shared_ptr<StatusInf> _statusInf;

	std::deque<std::shared_ptr<UI>> _terrainInfDeque;
	std::deque<std::shared_ptr<UI>> _statusInfDeque;
	std::deque<std::shared_ptr<UI>> _statusWindowDeque;

public:
	BattlePreparationCursor(MapCtrl& mapCtrl, Camera& camera);
	~BattlePreparationCursor();

	void Update(const Input& input)override;
	void Draw()override;

	void DrawSortieMass();
	void DrawMovableMass();

	bool GetEnd()const;

	void Start();
	void SetCursorStartPosition();
};

