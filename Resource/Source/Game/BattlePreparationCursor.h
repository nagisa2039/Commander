#pragma once
#include "MapCursor.h"
#include <memory>
#include <deque>
#include "TimeLine.h"

class Charactor;
class TerrainInf;
class StatusInf;
class StatusWindow;
class UI;

class BattlePreparationCursor :
	public MapCursor
{
private:
	Vector2Int _selectMapPos;	// 選択中のマップ座標  xが負の値の時は未選択
	std::unique_ptr<Track<float>> _exRateTrack;
	bool _end;

	void Select();

	std::shared_ptr<TerrainInf> _terrainInf;
	std::shared_ptr<StatusInf> _statusInf;
	std::shared_ptr<StatusWindow> _statusWindow;

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

