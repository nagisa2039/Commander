#include "PlayerUI.h"
#include "PlayerCommander.h"
#include "MapCtrl.h"
#include "Input.h"
#include "Camera.h"
#include "DxLibUtility.h"
#include "Application.h"
#include "FileSystem.h"
#include <Dxlib.h>

PlayerUI::PlayerUI(const PlayerCommander& playerCommander, const MapCtrl& mapCtrl): _playerCommander(playerCommander), _mapCtrl(mapCtrl)
{
}

PlayerUI::~PlayerUI()
{
}

void PlayerUI::Update(const Input& input)
{
}

void PlayerUI::Draw()
{
	// ’nŒ`î•ñ‚Ì•`‰æ

	// “y‘ä‚Ì•`‰æ
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

	// ˜g‚Ì•`‰æ
	terrainInfRect.Draw(0xaaaaaa, false);
	DrawLine(terrainInfRect.Left(), terrainInfRect.center.y, terrainInfRect.Right(), terrainInfRect.center.y, 0xaaaaaa);
}
