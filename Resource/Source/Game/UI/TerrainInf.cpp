#include "TerrainInf.h"
#include <Dxlib.h>
#include "Application.h"
#include "FileSystem.h"
#include "DxLibUtility.h"
#include "DataBase.h"
#include "MapCtrl.h"

using namespace std;

TerrainInf::TerrainInf(std::deque<std::shared_ptr<UI>>& uiDeque, const MapCtrl& mapCtrl, const Vector2Int& mapPos): UI(uiDeque), _mapCtrl(mapCtrl), _mapPos(mapPos)
{
	_animTrack = make_unique<Track<float>>();
	_animTrack->AddKey(0, 0.0f);
	_animTrack->AddKey(15, 1.0f);

	Open();
}

TerrainInf::~TerrainInf()
{
}

void TerrainInf::Update(const Input& input)
{
	_animTrack->Update();
}

void TerrainInf::Draw()
{
	// ’nŒ`î•ñ‚Ì•`‰æ

	// “y‘ä‚Ì•`‰æ
	Rect terrainInfRect = Rect(Lerp(Vector2Int(-80, 65), Vector2Int(100, 65), _animTrack->GetValue()), Size(160, 90));
	DrawGraph(terrainInfRect.Left(), terrainInfRect.Top(), Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/UI/terrainInf.png"), true);

	int drawY = terrainInfRect.Top();
	auto mapChipData = Application::Instance().GetDataBase().GetMapChipData( _mapCtrl.GetMapData(_mapPos).mapChip);

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

void TerrainInf::Close()
{
	if (!_animTrack->GetReverse())
	{
		_animTrack->SetReverse(true);
		_animTrack->Reset();
	}
}

void TerrainInf::Open()
{
	if (_animTrack->GetReverse())
	{
		_animTrack->SetReverse(false);
		_animTrack->Reset();
	}
}