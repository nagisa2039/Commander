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
class PlayerMenu;
class MoveMenu;
class StatusInfomation;

class PlayerUI
{
private:
	const MapCtrl& _mapCtrl;
	PlayerCommander& _playerCommander;

	std::deque<std::shared_ptr<UI>> _playerMenuDeque;
	std::shared_ptr<PlayerMenu> _playerMenu;

	std::deque<std::shared_ptr<UI>> _moveMenuDeque;
	std::shared_ptr<MoveMenu> _moveMenu;

	std::deque<std::shared_ptr<UI>> _statusDeque;
	std::deque<std::shared_ptr<UI>> _statusInfDeque;
	std::deque<std::shared_ptr<UI>> _battlePreDeque;

	std::unique_ptr<Track<float>> _terrainInfTrack;
	void DrawTerrainInf();

public:
	PlayerUI(PlayerCommander& playerCommander, const MapCtrl& mapCtrl);
	~PlayerUI();

	void Update(const Input& input);
	void Draw();

	bool GetUIIsOpen()const;
	void OpenPlayerMenu(bool animation = true);
	void ClosePlayerMenu(bool animation = true);

	std::shared_ptr<MoveMenu> GetMoveMenu();

	void AddBattlePre();
	void ClearBattlePre();
};

