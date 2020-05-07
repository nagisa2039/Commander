#pragma once
#include "UI.h"
#include <string>
#include "TimeLine.h"
#include <functional>
#include <memory>

class PlayerCommander;

class CheckWindow :
	public UI
{
private:
	std::string _messageStr;
	std::unique_ptr<Track<float>> _exRateTrack;
	std::unique_ptr<Track<float>> _selectEcRateTrack;
	std::function<void()> _func;

	void (CheckWindow::* _updater)(const Input& input);
	void NormalUpdate(const Input& input);
	void ScalingUpdate(const Input& input);

	static int _windowImageH;

	enum class Select
	{
		yes,
		no,
		max
	};
	Select _select;

public:
	CheckWindow(const std::string& messageStr, std::deque<std::shared_ptr<UI>>& uiDeque, const std::function<void()>& func);
	~CheckWindow();

	void Update(const Input& input)override;
	void Draw()override;
};

