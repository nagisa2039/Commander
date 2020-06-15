#pragma once
#include "Geometry.h"
#include <memory>
#include <deque>
#include "TimeLine.h"

class PlayerCommander;
class MapCtrl;
class Input;
class UI;
class PlayerMenu;
class MoveMenu;
class StatusInf;
class TerrainInf;
class StatusInf;

class PlayerUI
{
private:
	const MapCtrl& _mapCtrl;
	PlayerCommander& _playerCommander;

	std::deque<std::shared_ptr<UI>> _playerMenuDeque;
	std::shared_ptr<PlayerMenu> _playerMenu;

	std::deque<std::shared_ptr<UI>> _moveMenuDeque;
	std::shared_ptr<MoveMenu> _moveMenu;

	std::deque<std::shared_ptr<UI>> _terrainInfDeque;
	std::shared_ptr<TerrainInf> _terrainInf;

	std::deque<std::shared_ptr<UI>> _statusDeque;
	std::shared_ptr<StatusInf> _statusInf;

	std::deque<std::shared_ptr<UI>> _statusInfDeque;
	std::deque<std::shared_ptr<UI>> _battlePreDeque;

public:
	PlayerUI(PlayerCommander& playerCommander, const MapCtrl& mapCtrl, const unsigned char& turnCnt);
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

