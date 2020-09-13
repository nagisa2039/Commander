#include "MapCtrl.h"
#include "../Utility/DxLibUtility.h"
#include "../System/Application.h"
#include "../System/FileSystem.h"
#include "DataBase.h"
#include "../Game/Camera.h"
#include "Charactor.h"
#include "DxLib.h"
#include <algorithm>
#include "SaveData.h"

#include "SceneController.h"
#include "Effect/Effect.h"
#include "Map.h"
#include "RouteManager.h"

using namespace std;

namespace
{
	constexpr unsigned int WAR_SITUATION_CHIP_SIZE = 20;
}

void MapCtrl::CreateCharactor(const CharactorChipInf& characotChipInf, const Status& initStatus, SceneController& ctrl, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera)
{
	auto charactor = make_shared<Charactor>(characotChipInf.type, characotChipInf.level, characotChipInf.mapPos, characotChipInf.team, characotChipInf.groupNum, *this, ctrl, effects, camera);
	charactor->SetMoveActive(characotChipInf.active);
	charactor->InitStatus(initStatus);
	_charactors.emplace_back(charactor);
}

MapCtrl::MapCtrl(const int mapId, std::vector<std::shared_ptr<Charactor>>& charactors) : _charactors(charactors)
{
	_astar = make_unique<Astar>();
	_map = DataBase::Instance().GetMapData(mapId);

	auto& mapSize = _map->GetMapSize();
	_warSituationH = FileSystem::Instance().
		MakeScreen("war_situation", mapSize * WAR_SITUATION_CHIP_SIZE, true);
}

MapCtrl::~MapCtrl()
{
}

void MapCtrl::Draw(const Camera& camera)
{
	_map->Draw(camera);
}

bool MapCtrl::DrawSortieMass(const Vector2Int& offset, const CharactorChipInf& charactorChipInf, const unsigned int color, const unsigned int frameColor)
{
	if (charactorChipInf.team != Team::player/* || charactorChipInf.type == CharactorType::max*/)return false;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 192);
	auto chipSize = _map->GetChipSize();
	Vector2Int leftup = offset + charactorChipInf.mapPos * chipSize;
	DrawBox(leftup, leftup + chipSize, color);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	DrawBox(leftup + 1, leftup + chipSize, frameColor, false);
	return true;
}

const Size& MapCtrl::GetChipSize()const
{
	return _map->GetChipSize();
}

const Size& MapCtrl::GetMapSize() const
{
	return _map->GetMapSize();
}

Charactor* MapCtrl::GetMapPosChar(const Vector2Int mapPos)const
{
	for (auto& charactor : _charactors)
	{
		if (mapPos == charactor->GetMapPos())
		{
			return &(*charactor);
		}
	}
	return nullptr;
}

void MapCtrl::CreateCharactor(SceneController& ctrl, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera)
{
	auto& dataBase = DataBase::Instance();
	auto charactorTypeMax = dataBase.GetCharactorDataTable().size();
	for (const auto& mapDataVec : _map->GetMapData())
	{
		for (const auto& mapData : mapDataVec)
		{
			if (mapData.charactorChip.type == charactorTypeMax || mapData.charactorChip.team == Team::max) continue;

			auto initStatus = dataBase.GetLevelInitStatus(mapData.charactorChip.level, mapData.charactorChip.type);
			initStatus.weaponId = mapData.charactorChip.weaponId;
			CreateCharactor(mapData.charactorChip, initStatus, ctrl, effects, camera);

		}
	}
}

void MapCtrl::CreateMapVec(std::vector<std::vector<Astar::MapData>>& mapVec2, const Team team)
{
	auto& mapData = _map->GetMapData();
	auto& dataBase = DataBase::Instance();
	mapVec2.resize(mapData.size());
	for (int y = 0; y < mapVec2.size(); y++)
	{
		mapVec2[y].resize(mapData[y].size());
		for (int x = 0; x < mapVec2[y].size(); x++)
		{
			mapVec2[y][x] = Astar::MapData(dataBase.GetMapChipData(mapData[y][x].mapChip).moveCost, Team::max, false);
		}
	}
	for (const auto& charactor : _charactors)
	{
		auto mapPos = charactor->GetMapPos();
		mapVec2[mapPos.y][mapPos.x].team = charactor->GetTeam();
		mapVec2[mapPos.y][mapPos.x].isHurt = charactor->GetHurtPoint() > 0;
	}
}

void MapCtrl::CreateWarSituation()const
{
	int currentScreen = GetDrawScreen();
	SetDrawScreen(_warSituationH);
	ClsDrawScreen();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 230);

	auto& mapData = _map->GetMapData();
	auto& dataBase = DataBase::Instance();
	auto& mapSize = _map->GetMapSize();

	// マップチップの描画
	int y = 0;
	for (const auto& mapDataVec : mapData)
	{
		int x = 0;
		for (const auto& mapData : mapDataVec)
		{
			DrawBox(x * WAR_SITUATION_CHIP_SIZE, y * WAR_SITUATION_CHIP_SIZE,
				(x + 1) * WAR_SITUATION_CHIP_SIZE, (y + 1) * WAR_SITUATION_CHIP_SIZE,
				dataBase.GetMapChipData(mapData.mapChip).simpleColor, true);
			x++;
		}
		y++;
	}
	for (int y = 0; y < mapSize.h; y++)
	{
		for (int x = 0; x < mapSize.w; x++)
		{
			DrawBox(x * WAR_SITUATION_CHIP_SIZE, y * WAR_SITUATION_CHIP_SIZE,
				(x+1) * WAR_SITUATION_CHIP_SIZE, (y+1) * WAR_SITUATION_CHIP_SIZE, 
				dataBase.GetMapChipData(mapData[y][x].mapChip).simpleColor, true);
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	// キャラクターの描画
	for (const auto& charactor : _charactors)
	{
		DrawCircle(charactor->GetMapPos() * WAR_SITUATION_CHIP_SIZE + Vector2Int(WAR_SITUATION_CHIP_SIZE/2, WAR_SITUATION_CHIP_SIZE / 2),
			static_cast<int>(WAR_SITUATION_CHIP_SIZE*0.3f), GetTeamColor(charactor->GetTeam()));
	}

	// グリット
	unsigned int lineColor = 0xaaaaaa;
	for (int x = 0; x < mapSize.w; x++)
	{
		DxLib::DrawLine(x * WAR_SITUATION_CHIP_SIZE, 0, x * WAR_SITUATION_CHIP_SIZE, mapSize.h * WAR_SITUATION_CHIP_SIZE, lineColor);
	}
	for (int y = 0; y < mapSize.h; y++)
	{
		DxLib::DrawLine(0, y * WAR_SITUATION_CHIP_SIZE, mapSize.w * WAR_SITUATION_CHIP_SIZE, y * WAR_SITUATION_CHIP_SIZE, lineColor);
	}
	DrawLine(mapSize.w * WAR_SITUATION_CHIP_SIZE-1, 0, mapSize.w * WAR_SITUATION_CHIP_SIZE-1, mapSize.h * WAR_SITUATION_CHIP_SIZE, lineColor);
	DrawLine(0, mapSize.h * WAR_SITUATION_CHIP_SIZE-1, mapSize.w * WAR_SITUATION_CHIP_SIZE, mapSize.h * WAR_SITUATION_CHIP_SIZE-1, lineColor);

	SetDrawScreen(currentScreen);
}

void MapCtrl::DrawWarSituatin(const Vector2Int center)const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);

	Vector2Int space(50, 50);
	Size graphSize;
	GetGraphSize(_warSituationH, graphSize);

	DrawBox(center - (graphSize * 0.5f).ToVector2Int() - space, center + (graphSize * 0.5f).ToVector2Int() + space, 0x000000);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	DrawRotaGraph(center, 1.0f, 0.0f, _warSituationH, true);
}

Vector2Int MapCtrl::GetCharactorCnt()const
{
	Vector2Int charactorCnt(0,0);
	for (const auto& charactor : _charactors)
	{
		charactorCnt += charactor->GetTeam() == Team::player ?Vector2Int(1,0) : Vector2Int(0,1);
	}
	return charactorCnt;
}

unsigned int MapCtrl::GetCanMoveCnt() const
{
	int cnt = 0;
	for (const auto& charactor : _charactors)
	{
		if (charactor->GetTeam() == Team::player && charactor->GetCanMove())
		{
			cnt++;
		}
	}
	return cnt;
}

void MapCtrl::AllCharactorRouteSearch() const
{
	for (auto& charactor : _charactors)
	{
		charactor->RouteSearch();
	}
}

void MapCtrl::SetGroupActive(const unsigned int groupNum, const bool active)
{
	for (auto& charactor : _charactors)
	{
		if (charactor->GetGroupNum() == groupNum)
		{
			charactor->SetMoveActive(active);
		}
	}
}

const std::vector<std::shared_ptr<Charactor>>& MapCtrl::GetCharacots() const
{
	return _charactors;
}

std::shared_ptr<Map> MapCtrl::GetMap()const
{
	assert(_map);
	return _map;
}

Astar& MapCtrl::GetAstar()
{
	return *_astar;
}
