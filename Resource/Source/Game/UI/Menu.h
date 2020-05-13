#pragma once
#include "UI.h"
#include <array>
#include <functional>
#include "Geometry.h"
#include "TimeLine.h"

class PlayerCommander;
class MapCtrl;

class Menu :
	public UI
{
private:
	PlayerCommander& _playerCommander;
	const MapCtrl& _mapCtrl;

	std::unique_ptr<Track<float>> _penAnimTrack;

	std::unique_ptr<Track<float>> _openAnimTrack;
	std::unique_ptr<Track<float>> _closeAnimTrack;

	enum class State
	{
		open,		// 開いている
		close,		// 閉じている
		openAnim,	// 開くアニメーション中
		closeAnim,	// 閉じるアニメーション中
		max,
	};

	std::array<std::function<Vector2Int(const int idx)>, static_cast<size_t>(State::max)> _drawCenterPosFuncs;

	enum class Content
	{
		situation,
		retreat,
		turnEnd,
		max
	};

	std::array<std::string, static_cast<size_t>(Content::max)> _contentNames;
	std::array<Vector2Int, static_cast<size_t>(Content::max)> _centerPoss;
	std::array<std::function<void()>, static_cast<size_t>(Content::max)> _contentFunc;

	Content _selectContent;

	void (Menu::* _updater)(const Input& input);
	void (Menu::* _drawer)();

	void OpenUpdate(const Input& input);
	void CloseUpdate(const Input& input);
	void OpenAnimUpdate(const Input& input);
	void CloseAnimUpdate(const Input& input);

	void OpenDraw();
	void CloseDraw();
	void OpenAnimDraw();
	void CloseAnimDraw();

	void DrawPen();
	void DrawContent(const Vector2Int& drawCenterPos, const unsigned int idx);

public:
	Menu(std::deque<std::shared_ptr<UI>>& uiDeque, PlayerCommander& playerCom, const MapCtrl& mapCtrl);
	~Menu();

	void Update(const Input& input)override;
	void Draw()override;

	void Open(bool animation = true);
	void Close(bool animation = true);
};

