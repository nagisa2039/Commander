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
#include "StatusWindow.h"
#include "StatusInfomation.h"
#include "BattlePrediction.h"
#include "Charactor.h"
#include "TerrainInf.h"

using namespace std;

PlayerUI::PlayerUI(PlayerCommander& playerCommander, const MapCtrl& mapCtrl): _playerCommander(playerCommander), _mapCtrl(mapCtrl)
{
	_playerMenuDeque.clear();
	_moveMenuDeque.clear();
	_statusDeque.clear();
	_battlePreDeque.clear();
	_terrainInfDeque.clear();
	_statusInfDeque.clear();

	_playerMenu = make_shared<PlayerMenu>(_playerMenuDeque, playerCommander, _mapCtrl);
	_playerMenuDeque.emplace_front(_playerMenu);

	_moveMenu = make_shared<MoveMenu>(_playerMenuDeque, playerCommander, _mapCtrl);
	_moveMenuDeque.emplace_back(_moveMenu);

	_terrainInf = make_shared<TerrainInf>(_terrainInfDeque, _mapCtrl, playerCommander.GetMapPos());
	_terrainInfDeque.emplace_back(_terrainInf);

	_statusInf = make_shared<StatusInf>(_statusInfDeque, _mapCtrl, playerCommander.GetMapPos());
	_statusInfDeque.emplace_back(_statusInf);
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
		if (_statusDeque.size() <= 0 && charactor != nullptr)
		{
			_statusDeque.emplace_front(make_shared<StatusWindow>(_statusDeque, *charactor));
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

	auto UpdateDeque = [](auto deque, const Input& input)
	{
		if (deque.size() > 0)
		{
			(*deque.begin())->Update(input);
		}
	};

	UpdateDeque(_terrainInfDeque, input);
	UpdateDeque(_playerMenuDeque, input);
	UpdateDeque(_moveMenuDeque, input);
	UpdateDeque(_statusDeque, input);
	UpdateDeque(_statusInfDeque, input);
	UpdateDeque(_battlePreDeque, input);

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
		_battlePreDeque.clear();
		return;
	}

	auto charactor = _mapCtrl.GetMapPosChar(_playerCommander.GetMapPos());
	if (charactor == nullptr)
	{
		_battlePreDeque.clear();
		return;
	}

	auto resultPosList = self->GetResutlPosList(_playerCommander.GetMapPos());
	if(resultPosList.size() <= 0)
	{
		_battlePreDeque.clear();
		return;
	}

	auto itr = resultPosList.begin();
	for (; itr != resultPosList.end(); itr++)
	{
		if (itr->attack)break;
	}
	if (itr == resultPosList.end())
	{
		_battlePreDeque.clear();
		return;
	}

	auto sub = (itr->prev == nullptr ? self->GetMapPos() : itr->prev->mapPos) - charactor->GetMapPos();
	_battlePreDeque.emplace_front(make_shared<BattlePrediction>(*self, *charactor, _battlePreDeque, abs(sub.x) + abs(sub.y)));
}

void PlayerUI::ClearBattlePre()
{
	_battlePreDeque.clear();
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

	DequeDraw(_terrainInfDeque);
	DequeDraw(_statusInfDeque);
	DequeDraw(_statusDeque);
	DequeDraw(_playerMenuDeque);
	if (_moveMenuDeque.size() > 0)
	{
		(*_moveMenuDeque.begin())->Draw();
	}
	DequeDraw(_battlePreDeque);
}

bool PlayerUI::GetUIIsOpen() const
{
	return _playerMenu->GetIsOpen() || _statusDeque.size() > 0 || _moveMenu->GetIsOpen();
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