#include "PlayerUI.h"
#include "PlayerCommander.h"
#include "MapCtrl.h"
#include "Camera.h"
#include "DxLibUtility.h"
#include <Dxlib.h>
#include "Application.h"
#include "FileSystem.h"
#include "Menu.h"
#include "Input.h"
#include "StatusWindow.h"

using namespace std;

PlayerUI::PlayerUI(PlayerCommander& playerCommander, const MapCtrl& mapCtrl): _playerCommander(playerCommander), _mapCtrl(mapCtrl)
{
	_menuDeque.clear();

	_statusDeque.clear();

	_menu = make_shared<Menu>(_menuDeque, playerCommander, _mapCtrl);
	_menuDeque.emplace_front(_menu);

	_terrainInfTrack = make_unique<Track<float>>();
	_terrainInfTrack->AddKey(0, 0.0f);
	_terrainInfTrack->AddKey(15, 1.0f);
}

PlayerUI::~PlayerUI()
{
}

void PlayerUI::Update(const Input& input)
{
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

	if (_menu->GetIsOpen())
	{
		(*_menuDeque.begin())->Update(input);
		if ((*_menuDeque.begin())->GetDelete())
		{
			_menuDeque.pop_front();
		}
	}

	if (_statusDeque.size() > 0)
	{
		(*_statusDeque.begin())->Update(input);
	}

	// �X�e�[�^�X�m�F
	if (input.GetButtonDown(0, "status"))
	{
		auto charactor = _mapCtrl.GetMapPosChar(_playerCommander.GetMapPos());
		if (charactor == nullptr) return;

		_statusDeque.emplace_front(make_shared<StatusWindow>(_statusDeque, *charactor));
		return;
	}
}

void PlayerUI::Draw()
{
	DrawTerrainInf();
	for (auto ritr = _menuDeque.rbegin(); ritr != _menuDeque.rend(); ritr++)
	{
		(*ritr)->Draw();
	}
	for (auto ritr = _statusDeque.rbegin(); ritr != _statusDeque.rend(); ritr++)
	{
		(*ritr)->Draw();
	}
}

bool PlayerUI::GetUIIsOpen() const
{
	return _menu->GetIsOpen() || _statusDeque.size() > 0;
}

void PlayerUI::OpenMenu(bool animation)
{
	if (_menu->GetIsOpen())return;
	_menu->Open(animation);
}

void PlayerUI::CloseMenu(bool animation)
{
	if (!GetUIIsOpen()) return;
	_menu->Close(animation);
}

void PlayerUI::DrawTerrainInf()
{
	// �n�`���̕`��

	// �y��̕`��
	Rect terrainInfRect = Rect(Lerp(Vector2Int(-80, 65), Vector2Int(100, 65), _terrainInfTrack->GetValue()), Size(160, 90));
	DrawGraph(terrainInfRect.Left(), terrainInfRect.Top(), Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/UI/terrainInf.png"), true);

	int drawY = terrainInfRect.Top();
	MapCtrl::MapChipData mapChipData = _mapCtrl.GetMapChipData(_playerCommander.GetMapPos());

	int choplin40 = Application::Instance().GetFileSystem()->GetFontHandle("choplin40");
	Size strSize;
	int lineCnt;
	GetDrawFormatStringSizeToHandle(&strSize.w, &strSize.h, &lineCnt, choplin40, mapChipData.name);
	auto namePos = GetDrawPos(Vector2Int(terrainInfRect.center.x, drawY), strSize, Anker::centerup);
	DrawFormatStringToHandle(namePos.x, namePos.y, 0xffffff, choplin40, mapChipData.name);
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