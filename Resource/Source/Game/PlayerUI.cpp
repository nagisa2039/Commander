#include "PlayerUI.h"
#include "PlayerCommander.h"
#include "MapCtrl.h"
#include "Input.h"
#include "Camera.h"
#include "DxLibUtility.h"
#include "Application.h"
#include "FileSystem.h"
#include <Dxlib.h>

using namespace std;

PlayerUI::PlayerUI(PlayerCommander& playerCommander, const MapCtrl& mapCtrl): _playerCommander(playerCommander), _mapCtrl(mapCtrl)
{
	auto wsize = Application::Instance().GetWindowSize();
	Vector2Int offset(-30, 60);

	_isMenuOpen = false;

	// çsä‘
	int lineSpace = 30;
	auto menuFrameH = Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/UI/menuFrame.png");
	Size menuFrameSize;
	GetGraphSize(menuFrameH, menuFrameSize);
	Vector2Int center = Vector2Int(offset.x + wsize.w - menuFrameSize.w / 2, offset.y + menuFrameSize.h / 2);

	for (auto& centerPos : _menuCenterPoss)
	{
		centerPos = center;
		center.y += lineSpace + menuFrameSize.h;
	}

	_penAnimTrack = make_unique<Track<float>>(true);
	_penAnimTrack->AddKey(0, 0.0f);
	_penAnimTrack->AddKey(30, 1.0f);
	_penAnimTrack->AddKey(60, 0.0f);

	_menuOpenAnimTrack = make_unique<Track<float>>();
	_menuOpenAnimTrack->AddKey(0,0.0f);
	_menuOpenAnimTrack->AddKey(15, 1.0f);

	_menuCloseAnimTrack = make_unique<Track<float>>();
	_menuCloseAnimTrack->AddKey(0, 0.0f);
	_menuCloseAnimTrack->AddKey(15, 1.0f);

	_menuUpdater = &PlayerUI::MenuCloseUpdate;
	_menuDrawer = &PlayerUI::MenuCloseDraw;

	_menuContentNames[static_cast<size_t>(MenuContent::situation)] = "êÌãµ";
	_menuContentNames[static_cast<size_t>(MenuContent::retreat)] = "ëﬁãp";
	_menuContentNames[static_cast<size_t>(MenuContent::turnEnd)] = "É^Å[ÉìèIóπ";

	_selectMenuContent = MenuContent::situation;
}

PlayerUI::~PlayerUI()
{
}

void PlayerUI::Update(const Input& input)
{
	_penAnimTrack->Update();
	_menuOpenAnimTrack->Update();
	_menuCloseAnimTrack->Update();
	(this->*_menuUpdater)(input);
}

void PlayerUI::Draw()
{
	DrawTerrainInf();
	(this->*_menuDrawer)();
}

bool PlayerUI::GetIsOpen() const
{
	return _isMenuOpen;
}

void PlayerUI::OpenMenu()
{
	if (_isMenuOpen) return;
	_menuUpdater = &PlayerUI::MenuOpenAnimUpdate;
	_menuDrawer = &PlayerUI::MenuOpenAnimDraw;
	_menuOpenAnimTrack->Reset();
	_selectMenuContent = MenuContent::situation;
	_isMenuOpen = true;
}

void PlayerUI::CloseMenu()
{
	if (!_isMenuOpen) return;
	_menuUpdater = &PlayerUI::MenuCloseAnimUpdate;
	_menuDrawer = &PlayerUI::MenuCloseAnimDraw;
	_menuCloseAnimTrack->Reset();
	_isMenuOpen = false;
}

void PlayerUI::DrawTerrainInf()
{
	// ínå`èÓïÒÇÃï`âÊ

	// ìyë‰ÇÃï`âÊ
	Rect terrainInfRect = Rect(Vector2Int(100, 65), Size(160, 90));
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

	// ògÇÃï`âÊ
	terrainInfRect.Draw(0xaaaaaa, false);
	DrawLine(terrainInfRect.Left(), terrainInfRect.center.y, terrainInfRect.Right(), terrainInfRect.center.y, 0xaaaaaa);
}

void PlayerUI::MenuOpenUpdate(const Input& input)
{
	if (input.GetButtonDown(0, "back"))
	{
		CloseMenu();
	}

	if (input.GetButtonDown(0, "ok"))
	{
	}

	if (input.GetButtonDown(0, "up") && static_cast<int>(_selectMenuContent) > 0)
	{
		_selectMenuContent = static_cast<MenuContent>(static_cast<int>(_selectMenuContent) - 1);
	}

	if (input.GetButtonDown(0, "down") && static_cast<int>(_selectMenuContent) < static_cast<int>(MenuContent::max)-1)
	{
		_selectMenuContent = static_cast<MenuContent>(static_cast<int>(_selectMenuContent) + 1);
	}
}

void PlayerUI::MenuCloseUpdate(const Input& input)
{
}

void PlayerUI::MenuOpenAnimUpdate(const Input& input)
{
	_menuOpenAnimTrack->Update();
	if (_menuOpenAnimTrack->GetEnd())
	{
		_menuUpdater = &PlayerUI::MenuOpenUpdate;
		_menuDrawer = &PlayerUI::MenuOpenDraw;
	}
}

void PlayerUI::MenuCloseAnimUpdate(const Input& input)
{
	_menuCloseAnimTrack->Update();
	if (_menuCloseAnimTrack->GetEnd())
	{
		_menuUpdater = &PlayerUI::MenuCloseUpdate;
		_menuDrawer = &PlayerUI::MenuCloseDraw;
	}
}

void PlayerUI::MenuOpenDraw()
{
	for (int idx = 0; idx < static_cast<int>(MenuContent::max); idx++)
	{
		DrawMenuContent(_menuCenterPoss[idx], idx);
	}

	DrawPen();
}

void PlayerUI::MenuCloseDraw()
{
}

void PlayerUI::MenuOpenAnimDraw()
{
	auto menuFrameH = Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/UI/menuFrame.png");
	Size menuFrameSize;
	GetGraphSize(menuFrameH, menuFrameSize);

	auto GetCenterPos = [&](const unsigned int idx)
	{
		
		return Lerp(Vector2Int(_menuCenterPoss[idx].x, -menuFrameSize.h / 2), _menuCenterPoss[idx], _menuOpenAnimTrack->GetValue());
	};

	for (int idx = 0; idx < static_cast<int>(MenuContent::max); idx++)
	{
		DrawMenuContent(GetCenterPos(idx), idx);
	}
}

void PlayerUI::MenuCloseAnimDraw()
{
	auto wsize = Application::Instance().GetWindowSize();
	auto menuFrameH = Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/UI/menuFrame.png");
	Size menuFrameSize;
	GetGraphSize(menuFrameH, menuFrameSize);

	auto GetCenterPos = [&](const unsigned int idx)
	{
		return Lerp(_menuCenterPoss[idx], Vector2Int(wsize.w + menuFrameSize.w / 2, _menuCenterPoss[idx].y), _menuCloseAnimTrack->GetValue());
	};

	for (int idx = 0; idx < static_cast<int>(MenuContent::max); idx++)
	{
		DrawMenuContent(GetCenterPos(idx), idx);
	}
}

void PlayerUI::DrawPen()
{
	auto penH = Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/UI/quillPen.png");
	Size penSize;
	GetGraphSize(penH, penSize);
	Vector2Int penMove = penSize.ToVector2Int() * -0.2f;

	auto menuFrameH = Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/UI/menuFrame.png");
	Size menuFrameSize;
	GetGraphSize(menuFrameH, menuFrameSize);

	Vector2Int penDrawPos = penMove * _penAnimTrack->GetValue() + _menuCenterPoss[static_cast<size_t>(_selectMenuContent)] - Vector2Int(menuFrameSize.w/2 - 10, -10);
	DrawGraph(GetDrawPos(penDrawPos, penSize, Anker::rightdown), penH, true);
}

void PlayerUI::DrawMenuContent(const Vector2Int& drawCenterPos, const unsigned int idx)
{
	int choplin30 = Application::Instance().GetFileSystem()->GetFontHandle("choplin30");
	auto menuFrameH = Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/UI/menuFrame.png");
	Size menuFrameSize;
	GetGraphSize(menuFrameH, menuFrameSize);
	DrawGraph(GetDrawPos(drawCenterPos, menuFrameSize, Anker::center), menuFrameH, true);
	Size strSize;
	int lineCnt;
	Vector2Int namePos = Vector2Int();
	GetDrawFormatStringSizeToHandle(&strSize.w, &strSize.h, &lineCnt, choplin30, _menuContentNames[idx].c_str());

	auto drawPos = GetDrawPos(drawCenterPos, strSize, Anker::center);
	DrawFormatStringToHandle(drawPos.x, drawPos.y, 0xffffff, choplin30, _menuContentNames[idx].c_str());
}
