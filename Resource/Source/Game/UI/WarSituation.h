#pragma once
#include "UI.h"
#include "TimeLine.h"
#include <memory>

class PlayScene;
class MapCtrl;

class WarSituation :
	public UI
{
private:
	const MapCtrl& _mapCtrl;
	std::unique_ptr<Track<float>> _exRate;
	void(WarSituation::* _updater)(const Input& input);
	bool _end;
	PlayScene* _playScene;

	void ScalingUpdate(const Input& input);
	void NormalUpdate(const Input& input);

public:
	WarSituation(std::deque<std::shared_ptr<UI>>* uiDeque, const MapCtrl& mapCtrl);
	~WarSituation();

	void Update(const Input& input)override;
	void Draw()override;
};

