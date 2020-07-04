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
class StatusWindow;
class BattlePrediction;

class PlayerUI
{
private:
	const MapCtrl& _mapCtrl;
	PlayerCommander& _playerCommander;

	std::deque<std::shared_ptr<UI>> _playerMenuDeque;
	std::shared_ptr<PlayerMenu> _playerMenu;

	std::deque<std::shared_ptr<UI>> _moveMenuDeque;
	std::shared_ptr<MoveMenu> _moveMenu;

	std::shared_ptr<TerrainInf> _terrainInf;
	std::shared_ptr<StatusWindow> _statusWindow;
	std::shared_ptr<StatusInf> _statusInf;
	std::shared_ptr<BattlePrediction> _battlePre;

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

