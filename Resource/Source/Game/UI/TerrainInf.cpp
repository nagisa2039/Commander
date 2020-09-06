#include "TerrainInf.h"
#include <Dxlib.h>
#include "Application.h"
#include "FileSystem.h"
#include "DxLibUtility.h"
#include "DataBase.h"
#include "MapCtrl.h"
#include "Map.h"

using namespace std;

TerrainInf::TerrainInf(std::deque<std::shared_ptr<UI>>* uiDeque, const MapCtrl& mapCtrl, const Vector2Int& mapPos): UI(uiDeque), _mapCtrl(mapCtrl), _mapPos(mapPos)
{
	_animTrack = make_shared<Track<float>>();
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
	// ínå`èÓïÒÇÃï`âÊ

	// ìyë‰ÇÃï`âÊ
	Vector2Int space(0, 20);
	auto& fileSystem = FileSystem::Instance();
	auto graphH = fileSystem.GetImageHandle("Resource/Image/UI/terrainNameFrame.png");
	Size graphSize;
	GetGraphSize(graphH, graphSize);
	Vector2Int startPos = Vector2Int(static_cast<int>(-graphSize.w*0.5f), space.y + static_cast<int>(graphSize.h * 0.5f));
	Vector2Int endPos = space + graphSize * 0.5f;
	Rect terrainInfRect = Rect(Lerp(startPos, endPos, _animTrack->GetValue()), graphSize);
	DrawGraph(terrainInfRect.Left(), terrainInfRect.Top(), graphH, true);

	auto mapChipData = Application::Instance().GetDataBase().GetMapChipData( _mapCtrl.GetMap()->GetMapData(_mapPos).mapChip);

	int choplin40 = fileSystem.GetFontHandle("choplin40edge");
	DrawStringToHandle(terrainInfRect.center, Anker::center, 0xffffff, choplin40, mapChipData.name.c_str());

	if (_animTrack->GetReverse())return;

	int choplin20No = fileSystem.GetFontHandle("choplin20");
	Vector2Int leftup = Vector2Int(terrainInfRect.Right(), terrainInfRect.Top());
	int efcSpaceX = 5;
	const unsigned normalColor = 0x000000;
	const unsigned badColor = 0xff0000;

	auto drawEffect = [&](const char* graphPath, const char* drawString, const bool badEffect)
	{
		auto graphH = fileSystem.GetImageHandle(graphPath);
		Size graphSize;
		GetGraphSize(graphH, graphSize);
		DrawGraph(leftup, graphH);
		DrawStringToHandle(leftup + (graphSize.ToVector2() * Vector2(0.5f, 0.75f)).ToVector2Int(), Anker::center, 
			badEffect ? badColor : normalColor, choplin20No, drawString);
		leftup.x += (graphSize.w + efcSpaceX);
	};

	if (abs(mapChipData.avoidance) > 0)
	{
		char buf[10];
		sprintf_s(buf, 10, "%dÅì", mapChipData.avoidance);
		drawEffect("Resource/Image/UI/terrainAvoid.png", buf, mapChipData.avoidance < 0);
	}
	if (abs(mapChipData.defense) > 0)
	{
		char buf[10];
		sprintf_s(buf, 10, "%d", mapChipData.defense);
		drawEffect("Resource/Image/UI/terrainDef.png", buf, mapChipData.defense < 0);
	}
	if (abs(mapChipData.recovery) > 0)
	{
		char buf[10];
		sprintf_s(buf, 10, "%dÅì", mapChipData.recovery);
		drawEffect("Resource/Image/UI/terrainRecover.png", buf, mapChipData.recovery < 0);
	}
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