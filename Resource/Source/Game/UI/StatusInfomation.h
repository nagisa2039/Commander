#pragma once
#include "UI.h"
#include "TimeLine.h"
#include <memory>
#include <list>

class MapCtrl;
class MapCursor;
class PlayerUI;

class StatusInf
	: public UI
{
private:
	const MapCtrl& _mapCtrl;
	const Vector2Int& _mapPos;

	std::list<Vector2Int> _targetPosList;
	std::unique_ptr<Track<float>> _moveAnimTrack;

public:
	StatusInf(std::deque<std::shared_ptr<UI>>& uiDeque, const MapCtrl& mapCtrl, const Vector2Int& mapPos);
	~StatusInf();

	void Update(const Input& input)override;
	void Draw()override;
	void Open();
	void Close();
};

