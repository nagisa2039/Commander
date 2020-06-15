#pragma once
#include "UI.h"
#include "TimeLine.h"

class Charactor;

class StatusWindow :
	public UI
{
private:
	const Charactor& _charactor;

	std::unique_ptr<Track<float>> _animTrack;
	void(StatusWindow::* _updater)(const Input& input);

	void NormalUpdate(const Input& input);
	void ScaleUpdate(const Input& input);

	static int _windowH;

	Size _leftWindowSize;
	Size _rightWindowSize;

	void DrawToWindowScreen();
	void DrawIcon(Rect& iconRect);
	void DrawBaseInf(Rect& statusRect0);
	void DrawBattleStatus(Rect& statusRect1);
	void DrawStatus(Rect& statusRect2);

public:
	StatusWindow(std::deque<std::shared_ptr<UI>>& uiDeque, const Charactor& charactor);
	~StatusWindow();

	void Update(const Input& input)override;
	void Draw()override;
};

