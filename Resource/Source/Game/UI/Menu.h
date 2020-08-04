#pragma once
#include "UI.h"
#include <array>
#include <functional>
#include <vector>
#include "Geometry.h"
#include "TimeLine.h"

class SelectPen;
class PlayerCommander;
class MapCtrl;

class Menu :
	public UI
{
protected:
	PlayerCommander& _playerCommander;
	const MapCtrl& _mapCtrl;

	Size _contentSize;
	size_t _selectContent;

	std::unique_ptr<Track<float>> _openAnimTrack;
	std::unique_ptr<Track<float>> _closeAnimTrack;

	std::unique_ptr<SelectPen> _selectPen;

	enum class State
	{
		open,		// 開いている
		close,		// 閉じている
		openAnim,	// 開くアニメーション中
		closeAnim,	// 閉じるアニメーション中
		max,
	};

	std::array<std::function<Vector2Int(const int idx)>, static_cast<size_t>(State::max)> _drawCenterPosFuncs;

	struct ContentInf
	{
		std::string name;
		Vector2Int centerPos;
		std::function<void()> func;
	};

	std::vector<ContentInf> _contentInfs;
	std::vector<unsigned int> _contentList;

	void (Menu::* _updater)(const Input& input);
	void (Menu::* _drawer)();

	virtual void OpenUpdate(const Input& input);
	void CloseUpdate(const Input& input);
	void OpenAnimUpdate(const Input& input);
	void CloseAnimUpdate(const Input& input);

	void OpenDraw();
	void CloseDraw();
	void OpenAnimDraw();
	void CloseAnimDraw();

	void DrawContent(const Vector2Int& drawCenterPos, const unsigned int idx);

	void Init(const size_t contentNum, const int frameH);

	virtual void Back();
	virtual void Decision();

public:
	Menu(std::deque<std::shared_ptr<UI>>* uiDeque, PlayerCommander& playerCom, const MapCtrl& mapCtrl);
	~Menu();

	void Update(const Input& input)override;
	virtual void Draw()override;

	void Open(bool animation = true);
	void Close(bool animation = true);
};

