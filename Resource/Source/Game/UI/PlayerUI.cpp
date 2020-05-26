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

using namespace std;

PlayerUI::PlayerUI(PlayerCommander& playerCommander, const MapCtrl& mapCtrl): _playerCommander(playerCommander), _mapCtrl(mapCtrl)
{
	_playerMenuDeque.clear();
	_moveMenuDeque.clear();
	_statusDeque.clear();
	_battlePreDeque.clear();

	_playerMenu = make_shared<PlayerMenu>(_playerMenuDeque, playerCommander, _mapCtrl);
	_playerMenuDeque.emplace_front(_playerMenu);

	_moveMenu = make_shared<MoveMenu>(_playerMenuDeque, playerCommander, _mapCtrl);
	_moveMenuDeque.emplace_back(_moveMenu);

	_statusInfDeque.clear();
	_statusInfDeque.emplace_back(make_shared<StatusInfomation>(_statusInfDeque, _mapCtrl, _playerCommander, *this));

	_terrainInfTrack = make_unique<Track<float>>();
	_terrainInfTrack->AddKey(0, 0.0f);
	_terrainInfTrack->AddKey(15, 1.0f);
}

PlayerUI::~PlayerUI()
{
}

void PlayerUI::Update(const Input& input)
{
	// ステータス確認
	if (input.GetButtonDown(0, "status"))
	{
		auto charactor = _mapCtrl.GetMapPosChar(_playerCommander.GetMapPos());
		if (_statusDeque.size() <= 0 && charactor != nullptr)
		{
			_statusDeque.emplace_front(make_shared<StatusWindow>(_statusDeque, *charactor));
			return;
		}
	}

	_terrainInfTrack->Update();
	if (GetUIIsOpen())
	{
		if (!_terrainInfTrack->GetReverse())
		{
			_terrainInfTrack->SetReverse(true);
			_terrainInfTrack->Reset();
		}
	}
	else
	{
		if (_terrainInfTrack->GetReverse())
		{
			_terrainInfTrack->SetReverse(false);
			_terrainInfTrack->Reset();
		}
	}

	auto UpdateDeque = [](auto deque, const Input& input)
	{
		if (deque.size() > 0)
		{
			(*deque.begin())->Update(input);
		}
	};
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
	DrawTerrainInf();
	auto DequeDraw = [](auto deque)
	{
		for (auto ritr = deque.rbegin(); ritr != deque.rend(); ritr++)
		{
			(*ritr)->Draw();
		}
	};

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

void PlayerUI::DrawTerrainInf()
{
	// 地形情報の描画

	// 土台の描画
	Rect terrainInfRect = Rect(Lerp(Vector2Int(-80, 65), Vector2Int(100, 65), _terrainInfTrack->GetValue()), Size(160, 90));
	DrawGraph(terrainInfRect.Left(), terrainInfRect.Top(), Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/UI/terrainInf.png"), true);

	int drawY = terrainInfRect.Top();
	auto mapChipData = _mapCtrl.GetMapChipData(_playerCommander.GetMapPos());

	int choplin40 = Application::Instance().GetFileSystem()->GetFontHandle("choplin40");
	Size strSize;
	int lineCnt;
	GetDrawFormatStringSizeToHandle(&strSize.w, &strSize.h, &lineCnt, choplin40, mapChipData.name.c_str());
	auto namePos = GetDrawPos(Vector2Int(terrainInfRect.center.x, drawY), strSize, Anker::centerup);
	DrawFormatStringToHandle(namePos.x, namePos.y, 0xffffff, choplin40, mapChipData.name.c_str());
	drawY += strSize.h;

	int choplin20 = Application::Instance().GetFileSystem()->GetFontHandle("choplin20");

	auto drawNum = [](const int num, const Vector2Int rightup, const int fontHandle, const unsigned int color = 0xffffff)
	{
		char str[20];
		sprintf_s(str, 20, "%d", num);
		Size strSize;
		int lineCnt;
		GetDrawFormatStringSizeToHandle(&strSize.w, &strSize.h, &lineCnt, fontHandle, str);
		auto drawPos = GetDrawPos(rightup, strSize, Anker::rightup);
		DrawFormatStringToHandle(drawPos.x, drawPos.y, color, fontHandle, str);
	};

	int offsetX = 15;
	DrawFormatStringToHandle(terrainInfRect.Left() + offsetX, drawY, 0xffffff, choplin20, "DFE.");
	drawNum(mapChipData.defense, Vector2Int(terrainInfRect.Right() - offsetX, drawY), choplin20);
	drawY += 20;

	DrawFormatStringToHandle(terrainInfRect.Left() + offsetX, drawY, 0xffffff, choplin20, "AVD.");
	drawNum(mapChipData.avoidance, Vector2Int(terrainInfRect.Right() - offsetX, drawY), choplin20);
	drawY += 20;
}