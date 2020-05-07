#pragma once
#include "Geometry.h"
#include<array>
#include <memory>
#include <functional>
#include <deque>
#include "TimeLine.h"

class PlayerCommander;
class MapCtrl;
class Input;
class UI;
class Menu;

class PlayerUI
{
private:
	const MapCtrl& _mapCtrl;
	PlayerCommander& _playerCommander;

	std::deque<std::shared_ptr<UI>> _menuDeque;
	std::shared_ptr<Menu> _menu;

	std::unique_ptr<Track<float>> _terrainInfTrack;
	void DrawTerrainInf();

public:
	PlayerUI(PlayerCommander& playerCommander, const MapCtrl& mapCtrl);
	~PlayerUI();

	void Update(const Input& input);
	void Draw();

	bool GetMenuIsOpen()const;
	void OpenMenu(bool animation = true);
	void CloseMenu(bool animation = true);
};

