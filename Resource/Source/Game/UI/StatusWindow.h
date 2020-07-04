#pragma once
#include "UI.h"
#include "TimeLine.h"

class Charactor;
class WeaponStatusWindow;

class StatusWindow :
	public UI
{
private:
	const Charactor& _charactor;

	std::unique_ptr<WeaponStatusWindow> _weaponStatusWindow;
	std::unique_ptr<Track<float>> _animTrack;
	void(StatusWindow::* _updater)(const Input& input);

	void NormalUpdate(const Input& input);
	void ScaleUpdate(const Input& input);

	static int _windowH;

	Size _sideWindowSize;
	Size _centerWindowSize;

	void DrawToWindowScreen();
	void DrawIcon(const Rect& iconRect);
	void DrawBaseInf(const Rect& statusRect0);
	void DrawBattleStatus(const Rect& statusRect1);
	void DrawStatus(const Rect& statusRect2);
	void DrawItems(const Rect& itemRect);

public:
	StatusWindow(std::deque<std::shared_ptr<UI>>* uiDeque, const Charactor& charactor);
	~StatusWindow();

	void Update(const Input& input)override;
	void Draw()override;
};

