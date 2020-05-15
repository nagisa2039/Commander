#pragma once
#include "UI.h"
#include "TimeLine.h"
#include <memory>
#include <list>

class MapCtrl;
class MapCursor;
class PlayerUI;

class StatusInfomation
	: public UI
{
private:
	const MapCtrl& _mapCtrl;
	const MapCursor& _cursor;
	const PlayerUI& _playerUI;

	std::unique_ptr<Track<float>> _moveAnimTrack;
	std::list<Vector2Int> _targetPosList;

public:
	StatusInfomation(std::deque<std::shared_ptr<UI>>& uiDeque, const MapCtrl& mapCtrl, const MapCursor& cursor, const PlayerUI& playerUI);
	~StatusInfomation();

	void Update(const Input& input)override;
	void Draw()override;
};

