#pragma once
#include "Geometry.h"
#include<array>
#include "TimeLine.h"
#include <memory>
#include <functional>

class PlayerCommander;
class MapCtrl;
class Input;

class PlayerUI
{
private:
	const MapCtrl& _mapCtrl;
	PlayerCommander& _playerCommander;

	std::unique_ptr<Track<float>> _penAnimTrack;

	std::unique_ptr<Track<float>> _menuOpenAnimTrack;
	std::unique_ptr<Track<float>> _menuCloseAnimTrack;

	bool _isMenuOpen;

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

	MenuContent _selectMenuContent;

	void (PlayerUI::* _menuUpdater)(const Input& input);
	void (PlayerUI::* _menuDrawer)();

	void DrawTerrainInf();

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
	PlayerUI(PlayerCommander& playerCommander, const MapCtrl& mapCtrl);
	~PlayerUI();

	void Update(const Input& input);
	void Draw();

	bool GetIsOpen()const;
	void OpenMenu();
	void CloseMenu();
};

