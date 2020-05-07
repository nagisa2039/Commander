#pragma once
#include "UI.h"
#include <array>
#include <functional>
#include "Geometry.h"
#include "TimeLine.h"

class PlayerCommander;

class Menu :
	public UI
{
private:
	PlayerCommander& _playerCommander;

	std::unique_ptr<Track<float>> _penAnimTrack;

	std::unique_ptr<Track<float>> _menuOpenAnimTrack;
	std::unique_ptr<Track<float>> _menuCloseAnimTrack;

	enum class MenuState
	{
		open,		// 開いている
		close,		// 閉じている
		openAnim,	// 開くアニメーション中
		closeAnim,	// 閉じるアニメーション中
		max,
	};

	std::array<std::function<Vector2Int(const int idx)>, static_cast<size_t>(MenuState::max)> _menuDrawCenterPosFuncs;

	enum class MenuContent
	{
		situation,
		retreat,
		turnEnd,
		max
	};

	std::array<std::string, static_cast<size_t>(MenuContent::max)> _menuContentNames;
	std::array<Vector2Int, static_cast<size_t>(MenuContent::max)> _menuCenterPoss;
	std::array<std::function<void()>, static_cast<size_t>(MenuContent::max)> _menuContentFunc;

	MenuContent _selectMenuContent;

	void (Menu::* _menuUpdater)(const Input& input);
	void (Menu::* _menuDrawer)();

	void MenuOpenUpdate(const Input& input);
	void MenuCloseUpdate(const Input& input);
	void MenuOpenAnimUpdate(const Input& input);
	void MenuCloseAnimUpdate(const Input& input);

	void MenuOpenDraw();
	void MenuCloseDraw();
	void MenuOpenAnimDraw();
	void MenuCloseAnimDraw();


	void DrawPen();
	void DrawMenuContent(const Vector2Int& drawCenterPos, const unsigned int idx);

public:
	Menu(std::deque<std::shared_ptr<UI>>& uiDeque, PlayerCommander& playerCom);
	~Menu();

	void Update(const Input& input)override;
	void Draw()override;

	void Open(bool animation = true);
	void Close(bool animation = true);

	bool GetIsOpen()const;
};

