#pragma once
#include "UI.h"
#include <string>
#include "TimeLine.h"
#include <memory>

class PlayerCommander;

class CheckWindow :
	public UI
{
private:
	std::string _messageStr;
	std::unique_ptr<Track<float>> _exRateTrack;
	PlayerCommander& _playerCommander;

	void (CheckWindow::* _updater)(const Input& input);
	void NormalUpdate(const Input& input);
	void ScalingUpdate(const Input& input);

	static int _windowImageH;
	static Size _windowSize;

public:
	CheckWindow(const std::string& messageStr, std::deque<std::shared_ptr<UI>>& uiDeque, PlayerCommander& playerCom);
	~CheckWindow();

	void Update(const Input& input)override;
	void Draw()override;
};

