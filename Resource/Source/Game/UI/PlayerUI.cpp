#include "PlayerUI.h"
#include "PlayerCommander.h"
#include "MapCtrl.h"
#include "Camera.h"
#include "DxLibUtility.h"
#include <Dxlib.h>
#include "Application.h"
#include "FileSystem.h"
#include "Menu.h"

using namespace std;

PlayerUI::PlayerUI(PlayerCommander& playerCommander, const MapCtrl& mapCtrl): _playerCommander(playerCommander), _mapCtrl(mapCtrl)
{
	_menuDeque.clear();

	_menu = make_shared<Menu>(_menuDeque, playerCommander);
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
	if (GetMenuIsOpen())
	{
		if (!_terrainInfTrack->GetReverse())
		{
			_terrainInfTrack->SetReverse(true);
			_terrainInfTrack->Reset();
		}

		(*_menuDeque.begin())->Update(input);
		if ((*_menuDeque.begin())->GetDelete())
		{
			_menuDeque.pop_front();
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
}

void PlayerUI::Draw()
{
	DrawTerrainInf();
	for (auto ritr = _menuDeque.rbegin(); ritr != _menuDeque.rend(); ritr++)
	{
		(*ritr)->Draw();
	}
}

bool PlayerUI::GetMenuIsOpen() const
{
	return _menu->GetIsOpen();
}

void PlayerUI::OpenMenu(bool animation)
{
	if (GetMenuIsOpen()) return;
	_menu->Open(animation);
}

void PlayerUI::CloseMenu(bool animation)
{
	if (!GetMenuIsOpen()) return;
	_menu->Close(animation);
}

void PlayerUI::DrawTerrainInf()
{
	// �n�`���̕`��

	// �y��̕`��
	Rect terrainInfRect = Rect(Lerp(Vector2Int(-80, 65), Vector2Int(100, 65), _terrainInfTrack->GetValue()), Size(160, 90));
	terrainInfRect.Draw(0x000000);

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


	DrawFormatStringFToHandle(terrainInfRect.Left(), drawY, 0xffffff, choplin20, "DFE.");
	drawNum(mapChipData.defense, Vector2Int(terrainInfRect.Right() - 10, drawY), choplin20);
	drawY += 20;

	DrawFormatStringFToHandle(terrainInfRect.Left(), drawY, 0xffffff, choplin20, "AVD.");
	drawNum(mapChipData.avoidance, Vector2Int(terrainInfRect.Right() - 10, drawY), choplin20);
	drawY += 20;

	// �g�̕`��
	terrainInfRect.Draw(0xaaaaaa, false);
	DrawLine(terrainInfRect.Left(), terrainInfRect.center.y, terrainInfRect.Right(), terrainInfRect.center.y, 0xaaaaaa);
}