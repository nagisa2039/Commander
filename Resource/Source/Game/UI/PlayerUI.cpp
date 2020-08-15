#include "PlayerUI.h"
#include "PlayerCommander.h"
#include "MapCtrl.h"
#include "Camera.h"
#include "DxLibUtility.h"
#include <Dxlib.h>
#include "Application.h"
#include "FileSystem.h"
#include "PlayerMenu.h"
#include "MoveMenu.h"
#include "Input.h"
#include "../UI/StatusWindow/StatusWindow.h"
#include "StatusInfomation.h"
#include "BattlePrediction.h"
#include "Charactor.h"
#include "TerrainInf.h"
#include "MenuTopInf.h"
#include "Map.h"

using namespace std;

PlayerUI::PlayerUI(PlayerCommander& playerCommander, const MapCtrl& mapCtrl, const unsigned char& turnCnt)
	: _playerCommander(playerCommander), _mapCtrl(mapCtrl)
{
	_playerMenuDeque.clear();
	_moveMenuDeque.clear();

	_playerMenu = make_shared<PlayerMenu>(&_playerMenuDeque, playerCommander, _mapCtrl, turnCnt);
	_playerMenuDeque.emplace_front(_playerMenu);

	_moveMenu = make_shared<MoveMenu>(&_playerMenuDeque, playerCommander, _mapCtrl);
	_moveMenuDeque.emplace_back(_moveMenu);

	_terrainInf = make_shared<TerrainInf>(nullptr, _mapCtrl, playerCommander.GetMapPos());
	_statusInf	= make_shared<StatusInf>	(nullptr, _mapCtrl, playerCommander.GetMapPos());

	_statusWindow.reset();
	_battlePre.reset();
}

PlayerUI::~PlayerUI()
{
}

void PlayerUI::Update(const Input& input)
{
	// ステータス確認
	if (input.GetButtonDown(0, "status") || input.GetButtonDown(1, "status"))
	{
		auto charactor = _mapCtrl.GetMapPosChar(_playerCommander.GetMapPos());
		if (!_statusWindow && charactor != nullptr)
		{
			_statusWindow = make_shared<StatusWindow>(nullptr, *charactor);
			return;
		}
	}

	if (GetUIIsOpen())
	{
		_terrainInf->Close();
		_statusInf->Close();
	}
	else
	{
		_terrainInf->Open();
		_statusInf->Open();
	}

	auto UpdateUI = [&input](std::shared_ptr<UI>& ui)
	{
		if (!ui)return;

		ui->Update(input);
		if (ui->GetDelete())
		{
			ui.reset();
		}
	};

	auto UpdateUIDeque = [&input](auto& deque)
	{
		if (deque.size() > 0)
		{
			(*deque.begin())->Update(input);
		}
	};
	
	_terrainInf->Update(input);
	_statusInf->Update(input);

	UpdateUI(_statusWindow);
	UpdateUI(_battlePre);

	UpdateUIDeque(_playerMenuDeque);
	UpdateUIDeque(_moveMenuDeque);

	if (_playerMenu->GetIsOpen())
	{
		if ((*_playerMenuDeque.begin())->GetDelete())
		{
			_playerMenuDeque.pop_front();
		}
	}
}

void PlayerUI::AddBattlePre()
{
	auto self = _playerCommander.GetSelectCharactor();
	if (self == nullptr)
	{
		_battlePre.reset();
		return;
	}

	auto targetMapPos = _playerCommander.GetMapPos();
	auto charactor = _mapCtrl.GetMapPosChar(targetMapPos);
	if (charactor == nullptr)
	{
		_battlePre.reset();
		return;
	}

	Vector2Int attackStartPos;
	if (!self->GetAttackStartPos(attackStartPos, targetMapPos))
	{
		_battlePre.reset();
		return;
	}

	_battlePre = make_shared<BattlePrediction>(*self, *charactor, nullptr, attackStartPos, 
		_mapCtrl.GetMap()->GetMapData(attackStartPos).mapChip);
}

void PlayerUI::ClearBattlePre()
{
	_battlePre.reset();
}

void PlayerUI::Draw()
{
	auto DequeDraw = [](auto deque)
	{
		for (auto ritr = deque.rbegin(); ritr != deque.rend(); ritr++)
		{
			(*ritr)->Draw();
		}
	};

	_terrainInf->Draw();
	_statusInf->Draw();

	if (_statusWindow)
	{
		_statusWindow->Draw();
	}
	if (_battlePre)
	{
		_battlePre->Draw();
	}
	DequeDraw(_playerMenuDeque);
	if (_moveMenuDeque.size() > 0)
	{
		(*_moveMenuDeque.begin())->Draw();
	}
}

bool PlayerUI::GetUIIsOpen() const
{
	return _playerMenu->GetIsOpen() || _statusWindow != nullptr || _moveMenu->GetIsOpen();
}

void PlayerUI::OpenPlayerMenu(bool animation)
{
	if (_playerMenu->GetIsOpen())return;
	_playerMenu->Open(animation);
}

void PlayerUI::ClosePlayerMenu(bool animation)
{
	if (!GetUIIsOpen()) return;
	_playerMenu->Close(animation);
}

std::shared_ptr<MoveMenu> PlayerUI::GetMoveMenu()
{
	return _moveMenu;
}