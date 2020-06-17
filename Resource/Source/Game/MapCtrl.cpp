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

#include "Swordsman.h"
#include "Warrior.h"
#include "Soldier.h"
#include "Mage.h"
#include "Archer.h"
#include "Priest.h"

#include "SceneController.h"
#include "Effect/Effect.h"

using namespace std;

constexpr unsigned int CHIP_SIZE_W = 32*2.5;
constexpr unsigned int CHIP_SIZE_H = 32*2.5;

constexpr unsigned int MAP_CHIP_CNT_W = 20;
constexpr unsigned int MAP_CHIP_CNT_H = 20;

constexpr unsigned int WAR_SITUATION_CHIP_SIZE = 20;

void MapCtrl::DrawToMapChipScreen()
{
	int currentScreen = GetDrawScreen();
	SetDrawScreen(_mapChipH);
	ClsDrawScreen();

	for (int y = 0; y < _mapDataVec2.size(); y++)
	{
		for (int x = 0; x < _mapDataVec2[y].size(); x++)
		{
			if (_mapDataVec2[y][x].mapChip > Map_Chip::none && _mapDataVec2[y][x].mapChip < Map_Chip::max)
			{
				DrawMapChip(Vector2Int(x, y), _mapDataVec2[y][x].mapChip);
			}
		}
	}

	SetDrawScreen(currentScreen);
}

void MapCtrl::DrawToMapFloorScreen()
{
	int currentScreen = GetDrawScreen();
	SetDrawScreen(_mapFloorH);
	ClsDrawScreen();

	for (int y = 0; y < _mapDataVec2.size(); y++)
	{
		for (int x = 0; x < _mapDataVec2[y].size(); x++)
		{
			DrawMapChip(Vector2Int(x, y), Map_Chip::floor_meadow);
		}
	}

	SetDrawScreen(currentScreen);
}

MapCtrl::MapCtrl(std::vector<std::shared_ptr<Charactor>>& charactors) : _charactors(charactors), imageFolderPath("Resource/Image/MapChip/")
{
	_astar = make_shared<Astar>();
	_mapChipH = MakeScreen(100 * CHIP_SIZE_W, 100 * CHIP_SIZE_H, true);
	_mapFloorH = MakeScreen(100 * CHIP_SIZE_W, 100 * CHIP_SIZE_H, true);
	_warSituationH = MakeScreen(WAR_SITUATION_CHIP_SIZE * MAP_CHIP_CNT_W, WAR_SITUATION_CHIP_SIZE * MAP_CHIP_CNT_H, true);

	_mapId = -1;

	int frameNum = 2;

	_mapDataVec2.resize(MAP_CHIP_CNT_H);
	for (int y = 0; y < MAP_CHIP_CNT_H; y++)
	{
		_mapDataVec2[y].resize(MAP_CHIP_CNT_W);
		for (int x = 0; x < MAP_CHIP_CNT_W; x++)
		{
			_mapDataVec2[y][x].mapChip = CheckMapPosPutRange(Vector2Int(x, y)) ? Map_Chip::none : Map_Chip::rock;
			_mapDataVec2[y][x].charactorChip.team = Team::max;
			_mapDataVec2[y][x].charactorChip.mapPos = Vector2Int(x, y);
		}
	}

	DrawToMapFloorScreen();
	DrawToMapChipScreen();

	_charactorCreateFuncs[static_cast<size_t>(CharactorType::swordman)] = 
		[&](const CharactorChipInf& characotChipInf, const Status& initStatus, SceneController& ctrl, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera)
	{
		auto charactor = make_shared<Swordsman>(characotChipInf.level, characotChipInf.mapPos, characotChipInf.team, characotChipInf.groupNum, *this, ctrl, effects, camera);
		charactor->SetMoveActive(characotChipInf.active);
		charactor->InitStatus(initStatus);
		_charactors.emplace_back(charactor);
	};

	_charactorCreateFuncs[static_cast<size_t>(CharactorType::soldier)] =
		[&](const CharactorChipInf& characotChipInf, const Status& initStatus, SceneController& ctrl, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera)
	{
		auto charactor = make_shared<Soldier>(characotChipInf.level, characotChipInf.mapPos, characotChipInf.team, characotChipInf.groupNum, *this, ctrl, effects, camera);
		charactor->SetMoveActive(characotChipInf.active);
		charactor->InitStatus(initStatus);
		_charactors.emplace_back(charactor);
	};

	_charactorCreateFuncs[static_cast<size_t>(CharactorType::warrior)] =
		[&](const CharactorChipInf& characotChipInf, const Status& initStatus, SceneController& ctrl, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera)
	{
		auto charactor = make_shared<Warrior>(characotChipInf.level, characotChipInf.mapPos, characotChipInf.team, characotChipInf.groupNum, *this, ctrl, effects, camera);
		charactor->SetMoveActive(characotChipInf.active);
		charactor->InitStatus(initStatus);
		_charactors.emplace_back(charactor);
	};

	_charactorCreateFuncs[static_cast<size_t>(CharactorType::mage)] =
		[&](const CharactorChipInf& characotChipInf, const Status& initStatus, SceneController& ctrl, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera)
	{
		auto charactor = make_shared<Mage>(characotChipInf.level, characotChipInf.mapPos, characotChipInf.team, characotChipInf.groupNum, *this, ctrl, effects, camera);
		charactor->SetMoveActive(characotChipInf.active);
		charactor->InitStatus(initStatus);
		_charactors.emplace_back(charactor);
	};

	_charactorCreateFuncs[static_cast<size_t>(CharactorType::archer)] =
		[&](const CharactorChipInf& characotChipInf, const Status& initStatus, SceneController& ctrl, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera)
	{
		auto charactor = make_shared<Archer>(characotChipInf.level, characotChipInf.mapPos, characotChipInf.team, characotChipInf.groupNum, *this, ctrl, effects, camera);
		charactor->SetMoveActive(characotChipInf.active);
		charactor->InitStatus(initStatus);
		_charactors.emplace_back(charactor);
	};

	_charactorCreateFuncs[static_cast<size_t>(CharactorType::priest)] =
		[&](const CharactorChipInf& characotChipInf, const Status& initStatus, SceneController& ctrl, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera)
	{
		auto charactor = make_shared<Priest>(characotChipInf.level, characotChipInf.mapPos, characotChipInf.team, characotChipInf.groupNum, *this, ctrl, effects, camera);
		charactor->SetMoveActive(characotChipInf.active);
		charactor->InitStatus(initStatus);
		_charactors.emplace_back(charactor);
	};
}

MapCtrl::~MapCtrl()
{
}

void MapCtrl::Draw(const Camera& camera, const bool edit)
{
	auto offset = camera.GetCameraOffset();

	DrawGraph(offset, _mapFloorH);
	DrawGraph(offset, _mapChipH);

	if (edit)
	{
		for (const auto& mapDataVec : _mapDataVec2)
		{
			for (const auto& mapData : mapDataVec)
			{
				DrawCharactorChip(mapData.charactorChip, offset);
			}
		}

		auto mapSize = GetMapSize();
		for (int h = 0; h <= mapSize.h; h++)
		{
			DrawLine(Vector2Int(0, h * CHIP_SIZE_H) + offset, 
				Vector2Int(mapSize.w * CHIP_SIZE_W, h * CHIP_SIZE_H) + offset, 0xffffff);
		}
		for (int w = 0; w <= mapSize.w; w++)
		{
			DrawLine(Vector2Int(w * CHIP_SIZE_W, 0) + offset, 
				Vector2Int(w * CHIP_SIZE_W, mapSize.h * CHIP_SIZE_H) + offset, 0xffffff);
		}
	}
}

bool MapCtrl::DrawSortieMass(const Vector2Int& offset, const CharactorChipInf& charactorChipInf, const unsigned int color, const unsigned int frameColor)
{
	if (charactorChipInf.team != Team::player || charactorChipInf.type == CharactorType::max)return false;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 192);
	auto chipSize = GetChipSize();
	Vector2Int leftup = offset + charactorChipInf.mapPos * chipSize;
	DrawBox(leftup, leftup + chipSize, color);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	DrawBox(leftup + 1, leftup + chipSize, frameColor, false);
	return true;
}

Size MapCtrl::GetChipSize()const
{
	return Size(CHIP_SIZE_W, CHIP_SIZE_H);
}

Size MapCtrl::GetMapSize() const
{
	return Size(MAP_CHIP_CNT_W, MAP_CHIP_CNT_H);
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

bool MapCtrl::SetMapChip(const Vector2Int& mapPos, const Map_Chip mapChip)
{
	if (CheckMapPosPutRange(mapPos))
	{
		_mapDataVec2[mapPos.y][mapPos.x].mapChip = mapChip;
		DrawToMapChipScreen();
		return true;
	}
	return false;
}

bool MapCtrl::DrawMapChip(const Vector2Int& mapPos, const Map_Chip mapChip, const Vector2Int& offset)
{
	auto drawData = Application::Instance().GetDataBase().GetMapChipData(mapChip).drawData;

	auto graphH = Application::Instance().GetFileSystem().GetImageHandle((imageFolderPath + drawData.path).c_str());
	DrawRectExtendGraph(offset.x + CHIP_SIZE_W * mapPos.x, offset.y + CHIP_SIZE_H * mapPos.y,
		offset.x + CHIP_SIZE_W * mapPos.x + CHIP_SIZE_W, offset.y + CHIP_SIZE_H * mapPos.y + CHIP_SIZE_H,
		drawData.leftup.x, drawData.leftup.y, drawData.size.w, drawData.size.h,
		graphH, true);

	return true;
}

const std::vector<std::vector<MapCtrl::MapData>>& MapCtrl::GetMapData() const
{
	return _mapDataVec2;
}

CharactorChipInf MapCtrl::GetCharactorChipInf(const Vector2Int& mapPos) const
{
	return _mapDataVec2[mapPos.y][mapPos.x].charactorChip;
}

bool MapCtrl::SetCharactorChip(const CharactorChipInf& charactorChipInf)
{
	// 移動不可のマスには設置できないようにする
	if (Application::Instance().GetDataBase().GetMapChipData(_mapDataVec2[charactorChipInf.mapPos.y][charactorChipInf.mapPos.x].mapChip).moveCost < 0)
	{
		return false;
	}

	_mapDataVec2[charactorChipInf.mapPos.y][charactorChipInf.mapPos.x].charactorChip = charactorChipInf;
	return true;
}

bool MapCtrl::DrawCharactorChip(const CharactorChipInf& charactorChipInf, const Vector2Int& offset)
{
	auto chipSize = GetChipSize();
	Vector2Int leftup = offset + charactorChipInf.mapPos * chipSize;

	if(!DrawSortieMass(offset, charactorChipInf))
	{
		if (charactorChipInf.team == Team::enemy && charactorChipInf.type != CharactorType::max)
		{
			int handle = Application::Instance().GetDataBase().GetCharactorImageHandle(charactorChipInf.type, charactorChipInf.team);
			DrawRectExtendGraph(leftup.x, leftup.y, leftup.x + chipSize.w, leftup.y + chipSize.h, 0, 0, 32, 32, handle, true);
			DrawFormatString(leftup.x, leftup.y, 0x000000, "Level.%d", charactorChipInf.level);
			DrawFormatString(leftup.x, leftup.y + 16, 0x000000, "GN.%d", charactorChipInf.groupNum);
			DrawFormatString(leftup.x, leftup.y + 32, 0x000000, charactorChipInf.active ? "Active : True" : "False");
		}
	}

	return true;
}

void MapCtrl::CreateCharactor(SceneController& ctrl, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera)
{
	auto dataBase = Application::Instance().GetDataBase();
	auto charactorDataVec = Application::Instance().GetSaveData().GetCharactorDataVec();
	auto itr = charactorDataVec.begin();

	for (const auto& mapDataVec : _mapDataVec2)
	{
		for (const auto& mapData : mapDataVec)
		{
			if (mapData.charactorChip.type == CharactorType::max || mapData.charactorChip.team == Team::max) continue;

			if (mapData.charactorChip.team == Team::player)
			{
				if (itr == charactorDataVec.end()) continue;

				auto saveData = *itr;
				auto charactorChip = mapData.charactorChip;
				_charactorCreateFuncs[static_cast<size_t>(saveData.charType)](mapData.charactorChip,
					saveData.status, ctrl, effects, camera);
				itr++;
			}
			else
			{
				_charactorCreateFuncs[static_cast<size_t>(mapData.charactorChip.type)](mapData.charactorChip, 
					dataBase.GetLevelInitStatus(mapData.charactorChip.level, mapData.charactorChip.type), ctrl, effects, camera);
			}
		}
	}
}

bool MapCtrl::SaveMap()
{
	string filePath;
	if (!FileSave(GetMainWindowHandle(), "map", "MAPファイル(*.map)\n*.map\n", "マップデータを保存します。", filePath))
	{
		return false;
	}
	SaveMapData(filePath.c_str());
	return true;
}


bool MapCtrl::LoadMap(const int mapId)
{
	_mapId = mapId;
	auto mapData = Application::Instance().GetDataBase().GetMapData(mapId);
	char filePath[MAX_PATH];
	sprintf_s(filePath, MAX_PATH, "Resource/Map/%s", mapData.fileName.c_str());
	return LoadMapData(filePath);
}

bool MapCtrl::LoadMap()
{
	string filePath;
	if (!FileLoad(GetMainWindowHandle(), "map", "MAPファイル(*.map)\n*.map\n", "マップデータを開きます。", filePath))
	{
		return false;
	}

	return LoadMapData(filePath);
}

bool MapCtrl::LoadMapData(const std::string& filePath)
{
	FILE* fp = nullptr;

	fopen_s(&fp, filePath.c_str(), "rb");

	TCHAR initPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, initPath);

	if (fp == NULL)
	{
		return false;
	}

	// マップチップの読み込み
	Size mapSize;
	fread_s(&mapSize, sizeof(mapSize), sizeof(mapSize), 1, fp);

	if (mapSize != GetMapSize())
	{
		fclose(fp);
		return false;
	}

	_mapDataVec2.resize(mapSize.h);
	for (auto& mapChipVec : _mapDataVec2)
	{
		mapChipVec.resize(mapSize.w);
		fread_s(mapChipVec.data(), sizeof(MapData) * mapSize.w, sizeof(MapData), mapSize.w, fp);
	}

	fclose(fp);

	DrawToMapChipScreen();

	return true;
}

bool MapCtrl::CheckMapPosPutRange(const Vector2Int& mapPos)
{
	int frameNum = 2;
	return mapPos.y >= frameNum && mapPos.y < MAP_CHIP_CNT_H - frameNum
		&& mapPos.x >= frameNum && mapPos.x < MAP_CHIP_CNT_W - frameNum;
}

void MapCtrl::RouteSearch(Charactor& charactor)
{
	std::vector<std::vector<Astar::MapData>> mapVec2;
	CreateMapVec(mapVec2, charactor.GetTeam());

	return _astar->RouteSearch(charactor.GetMapPos(), charactor.GetStatus().move, charactor.GetAttackRange(), 
		mapVec2, charactor.GetResutlPosListVec2(), charactor.GetTeam(), Application::Instance().GetDataBase().GetWeaponData(charactor.GetStatus().weaponId).heal);
}

bool MapCtrl::MoveRouteSearch(const Vector2Int& startPos, const unsigned int move, std::list<Astar::ResultPos>& resutlPosList, const Team team, const std::list<Astar::ResultPos>& excludeList)
{
	if (move <= 0)return false;

	std::vector<std::vector<Astar::MapData>> mapVec2;
	CreateMapVec(mapVec2, team);

	return _astar->MoveRouteSerch(startPos, move, mapVec2, resutlPosList, team, excludeList);
}

Vector2Int MapCtrl::SearchMovePos(Charactor& charactor, Vector2Int& targetCnt)
{
	std::vector<std::vector<Astar::MapData>> mapVec2;
	CreateMapVec(mapVec2, charactor.GetTeam());
	auto battleStatus = charactor.GetBattleStatus();

	auto& resultPosListVec2 = charactor.GetResutlPosListVec2();
	_astar->RouteSearch(charactor.GetMapPos(), battleStatus.status.move, battleStatus.weaponData.range,
		mapVec2, resultPosListVec2, charactor.GetTeam(), battleStatus.CheckHeal(), charactor.GetMoveActive());

	struct TargetCharactor
	{
		Charactor* charactor;
		int distance;
		Astar::ResultPos resultPos;

		TargetCharactor():charactor(nullptr), distance(1), resultPos(Astar::ResultPos()){};
		TargetCharactor(Charactor* ch, const int di, const Astar::ResultPos& rp) :charactor(ch), distance(di), resultPos(rp){};
	};

	// 範囲内の敵を格納するリスト
	list<TargetCharactor> targetCharactorList;
	// 派以外の敵を格納するリスト
	list<TargetCharactor> outRangeCharactorList;
	for (size_t y = 0; y < resultPosListVec2.size(); y++)
	{
		for (size_t x = 0; x < resultPosListVec2[y].size(); x++)
		{
			for (const auto& resultPos : resultPosListVec2[y][x])
			{
				// 攻撃マスになるまでcontinue
				if (!resultPos.attack)
				{
					continue;
				}

				auto mapCharactor = GetMapPosChar(resultPos.mapPos);
				// そのマスにキャラクターがいるか
				if (mapCharactor == nullptr)continue;

				// 探しているチームがいるか
				if (battleStatus.CheckHeal() != (charactor.GetTeam() == mapCharactor->GetTeam()))continue;

				auto mapPosSub = mapCharactor->GetMapPos() - (resultPos.prev == nullptr ? charactor.GetMapPos() : resultPos.prev->mapPos);
				int distance = abs(mapPosSub.x) + abs(mapPosSub.y);

				// 攻撃開始地点にいるキャラクター
				auto prevChar = GetMapPosChar(resultPos.prev->mapPos);
				// 攻撃範囲外か
				if (resultPos.moveCnt > battleStatus.status.move || (prevChar != nullptr && prevChar != &charactor))
				{
					outRangeCharactorList.emplace_back(TargetCharactor(mapCharactor, distance, resultPos));
					continue;
				}

				targetCharactorList.emplace_back(TargetCharactor(mapCharactor, distance, resultPos));
			}
		}
	}

	targetCnt.x = targetCharactorList.size();
	targetCnt.y = outRangeCharactorList.size();
	
	// 選別
	for (const auto& targetCharactor : targetCharactorList)
	{
		// 敵の攻撃範囲外からの攻撃か？
		if (!targetCharactor.charactor->GetAttackRange().Hit(targetCharactor.distance))
		{
			return targetCharactor.charactor->GetMapPos();
		}
	}

	// 敵の攻撃範囲外から攻撃できないので最初に見つけた敵の場所に向かう
	if (targetCharactorList.size() > 0)
	{
		if (battleStatus.CheckHeal())
		{
			// 最もダメージを受けているキャラクターを探す
			targetCharactorList.sort([](const TargetCharactor& left, const TargetCharactor& right)
			{
				return left.charactor->GetHurtPoint() > right.charactor->GetHurtPoint();
			});
			return targetCharactorList.begin()->charactor->GetMapPos();
		}

		// 最もダメージを与えられるキャラクターを探す
		targetCharactorList.sort([&battleStatus](const TargetCharactor& left, const TargetCharactor& right)
		{
			return battleStatus.GetDamage(left.charactor->GetBattleStatus()) > battleStatus.GetDamage(right.charactor->GetBattleStatus());
		});
		return targetCharactorList.begin()->charactor->GetMapPos();
	}

	if (outRangeCharactorList.size() > 0)
	{
		// 一番近いキャラクターを採用する
		outRangeCharactorList.sort([](const TargetCharactor& lval, const TargetCharactor& rval) 
		{
				return lval.resultPos.moveCnt < rval.resultPos.moveCnt;
		});
		auto targetCharactor = *outRangeCharactorList.begin();
		return targetCharactor.charactor->GetMapPos();
	}
	return Vector2Int(-1,-1);
}

void MapCtrl::CreateMapVec(std::vector<std::vector<Astar::MapData>>& mapVec2, const Team team)
{
	mapVec2.resize(_mapDataVec2.size());
	for (int y = 0; y < mapVec2.size(); y++)
	{
		mapVec2[y].resize(_mapDataVec2[y].size());
		for (int x = 0; x < mapVec2[y].size(); x++)
		{
			mapVec2[y][x] = Astar::MapData(Application::Instance().GetDataBase().GetMapChipData(_mapDataVec2[y][x].mapChip).moveCost, Team::max, false);
		}
	}
	for (const auto& charactor : _charactors)
	{
		auto mapPos = charactor->GetMapPos();
		mapVec2[mapPos.y][mapPos.x].team = charactor->GetTeam();
		mapVec2[mapPos.y][mapPos.x].isHurt = charactor->GetHurtPoint() > 0;
	}
}

MapCtrl::MapData MapCtrl::GetMapData(const Vector2Int& mapPos) const
{
	return _mapDataVec2.at(mapPos.y).at(mapPos.x);
}

void MapCtrl::CreateWarSituation()const
{
	int currentScreen = GetDrawScreen();
	SetDrawScreen(_warSituationH);
	ClsDrawScreen();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 230);

	auto dataBase = Application::Instance().GetDataBase();
	// マップチップの描画
	for (int y = 0; y < MAP_CHIP_CNT_H; y++)
	{
		for (int x = 0; x < MAP_CHIP_CNT_W; x++)
		{
			DrawBox(x * WAR_SITUATION_CHIP_SIZE, y * WAR_SITUATION_CHIP_SIZE,
				(x+1) * WAR_SITUATION_CHIP_SIZE, (y+1) * WAR_SITUATION_CHIP_SIZE, 
				dataBase.GetMapChipData(_mapDataVec2[y][x].mapChip).simpleColor, true);
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	// キャラクターの描画
	for (const auto& charactor : _charactors)
	{
		DrawCircle(charactor->GetMapPos() * WAR_SITUATION_CHIP_SIZE + Vector2Int(WAR_SITUATION_CHIP_SIZE/2, WAR_SITUATION_CHIP_SIZE / 2),
			WAR_SITUATION_CHIP_SIZE*0.3, GetTeamColor(charactor->GetTeam()));
	}

	// グリット
	for (int x = 0; x < MAP_CHIP_CNT_W; x++)
	{
		DxLib::DrawLine(x * WAR_SITUATION_CHIP_SIZE, 0, x * WAR_SITUATION_CHIP_SIZE, MAP_CHIP_CNT_H * WAR_SITUATION_CHIP_SIZE, 0xffffff);
	}
	for (int y = 0; y < MAP_CHIP_CNT_H; y++)
	{
		DxLib::DrawLine(0, y * WAR_SITUATION_CHIP_SIZE, MAP_CHIP_CNT_W * WAR_SITUATION_CHIP_SIZE, y * WAR_SITUATION_CHIP_SIZE, 0xffffff);
	}
	DrawLine(MAP_CHIP_CNT_W * WAR_SITUATION_CHIP_SIZE-1, 0, MAP_CHIP_CNT_W * WAR_SITUATION_CHIP_SIZE-1, MAP_CHIP_CNT_H* WAR_SITUATION_CHIP_SIZE, 0xffffff);
	DrawLine(0, MAP_CHIP_CNT_H * WAR_SITUATION_CHIP_SIZE-1, MAP_CHIP_CNT_W * WAR_SITUATION_CHIP_SIZE, MAP_CHIP_CNT_H * WAR_SITUATION_CHIP_SIZE-1, 0xffffff);

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

bool MapCtrl::CheckMapDataRange(const Vector2Int& mapPos)
{
	return mapPos.x >= 0 && mapPos.x < _mapDataVec2[mapPos.y].size() && mapPos.y >= 0 && mapPos.y < _mapDataVec2.size();
}

const std::vector<std::shared_ptr<Charactor>>& MapCtrl::GetCharacots() const
{
	return _charactors;
}

const int MapCtrl::GetMapID() const
{
	return _mapId;
}

bool MapCtrl::FileSave(const HWND hWnd, const char* ext, const char* filter, const char* title, std::string& filePath)
{
	static OPENFILENAME     ofn;
	static TCHAR            path[MAX_PATH];
	static TCHAR            file[MAX_PATH];

	TCHAR initPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, initPath);

	if (path[0] == TEXT('\n'))
	{
		GetCurrentDirectory(MAX_PATH, path);
	}
	if (ofn.lStructSize == 0)
	{
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = hWnd;
		ofn.lpstrInitialDir = path;       // 初期フォルダ位置
		ofn.lpstrFile = file;       // 選択ファイル格納
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrDefExt = TEXT(ext);
		ofn.lpstrFilter = TEXT(filter);
		ofn.lpstrTitle = TEXT(title);
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;
	}
	bool result = GetSaveFileName(&ofn);
	SetCurrentDirectory(initPath);
	filePath = file;

	return result;
}

bool MapCtrl::FileLoad(const HWND hWnd, const char* ext, const char* filter, const char* title, std::string& loadFilePath)
{
	static OPENFILENAME     ofn;
	static TCHAR            path[MAX_PATH];
	static TCHAR            file[MAX_PATH];

	TCHAR initPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, initPath);

	if (path[0] == TEXT('\n'))
	{
		GetCurrentDirectory(MAX_PATH, path);
	}
	if (ofn.lStructSize == 0)
	{
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = hWnd;
		ofn.lpstrInitialDir = path;       // 初期フォルダ位置
		ofn.lpstrFile = file;       // 選択ファイル格納
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrDefExt = TEXT(ext);
		ofn.lpstrFilter = TEXT(filter);
		ofn.lpstrTitle = TEXT(title);
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;
	}
	bool result = GetOpenFileName(&ofn);
	SetCurrentDirectory(initPath);
	loadFilePath = file;
	return result;
}

void MapCtrl::SaveMapData(const std::string& saveFilePath)
{
	FILE* fp = nullptr;
	fopen_s(&fp, saveFilePath.c_str(), "wb");

	// マップサイズの書き込み
	auto mapSize = GetMapSize();
	fwrite(&mapSize, sizeof(mapSize), 1, fp);

	// マップチップの書き込み
	for (const auto& mapChipVec : _mapDataVec2)
	{
		fwrite(mapChipVec.data(), sizeof(MapData), mapChipVec.size(), fp);
	}

	fclose(fp);
}
