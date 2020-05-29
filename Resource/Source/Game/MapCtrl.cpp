#include "MapCtrl.h"
#include "../Utility/DxLibUtility.h"
#include "../System/Application.h"
#include "../System/FileSystem.h"
#include "DataBase.h"
#include "../Game/Camera.h"
#include "Charactor.h"
#include "DxLib.h"
#include <algorithm>

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
	SetDrawScreen(_mapChipH);
	ClsDrawScreen();

	for (int y = 0; y < _mapDataVec2.size(); y++)
	{
		for (int x = 0; x < _mapDataVec2[y].size(); x++)
		{
			if (_mapDataVec2[y][x] > Map_Chip::none && _mapDataVec2[y][x] < Map_Chip::max)
			{
				DrawMapChip(Vector2Int(x, y), _mapDataVec2[y][x]);
			}
		}
	}

	SetDrawScreen(DX_SCREEN_BACK);
}

void MapCtrl::DrawToMapFloorScreen()
{
	SetDrawScreen(_mapFloorH);
	ClsDrawScreen();

	for (int y = 0; y < _mapDataVec2.size(); y++)
	{
		for (int x = 0; x < _mapDataVec2[y].size(); x++)
		{
			DrawMapChip(Vector2Int(x, y), Map_Chip::floor_meadow);
		}
	}

	SetDrawScreen(DX_SCREEN_BACK);
}

MapCtrl::MapCtrl(std::vector<std::shared_ptr<Charactor>>& charactors) : _charactors(charactors), imageFolderPath("Resource/Image/MapChip/")
{
	_astar = make_shared<Astar>();
	_mapChipH = MakeScreen(100 * CHIP_SIZE_W, 100 * CHIP_SIZE_H, true);
	_mapFloorH = MakeScreen(100 * CHIP_SIZE_W, 100 * CHIP_SIZE_H, true);
	_warSituationH = MakeScreen(WAR_SITUATION_CHIP_SIZE * MAP_CHIP_CNT_W, WAR_SITUATION_CHIP_SIZE * MAP_CHIP_CNT_H, true);

	int frameNum = 2;

	_mapDataVec2.resize(MAP_CHIP_CNT_H);
	for (int y = 0; y < MAP_CHIP_CNT_H; y++)
	{
		_mapDataVec2[y].resize(MAP_CHIP_CNT_W);
		for (int x = 0; x < MAP_CHIP_CNT_W; x++)
		{
			if (y < frameNum || y >= MAP_CHIP_CNT_H - frameNum
			 || x < frameNum || x >= MAP_CHIP_CNT_W - frameNum)
			{
				_mapDataVec2[y][x] = Map_Chip::rock;
			}
			else
			{
				_mapDataVec2[y][x] = Map_Chip::none;
			}
		}
	}

	_charactorChips.clear();
	_charactorChips.reserve(30);

	DrawToMapFloorScreen();
	DrawToMapChipScreen();

	_charactorCreateFuncs[static_cast<size_t>(CharactorType::swordman)] = 
		[&](const CharactorChipInf& characotChipInf, SceneController& ctrl, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera)
	{
		_charactors.emplace_back(make_shared<Swordsman>(characotChipInf.level, characotChipInf.mapPos, characotChipInf.team, characotChipInf.groupNum, *this, ctrl, effects, camera));
		(*_charactors.rbegin())->SetMoveActive(characotChipInf.active);
	};

	_charactorCreateFuncs[static_cast<size_t>(CharactorType::soldier)] =
		[&](const CharactorChipInf& characotChipInf, SceneController& ctrl, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera)
	{
		_charactors.emplace_back(make_shared<Soldier>(characotChipInf.level, characotChipInf.mapPos, characotChipInf.team, characotChipInf.groupNum, *this, ctrl, effects, camera));
		(*_charactors.rbegin())->SetMoveActive(characotChipInf.active);
	};

	_charactorCreateFuncs[static_cast<size_t>(CharactorType::warrior)] =
		[&](const CharactorChipInf& characotChipInf, SceneController& ctrl, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera)
	{
		_charactors.emplace_back(make_shared<Warrior>(characotChipInf.level, characotChipInf.mapPos, characotChipInf.team, characotChipInf.groupNum, *this, ctrl, effects, camera));
		(*_charactors.rbegin())->SetMoveActive(characotChipInf.active);
	};

	_charactorCreateFuncs[static_cast<size_t>(CharactorType::mage)] =
		[&](const CharactorChipInf& characotChipInf, SceneController& ctrl, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera)
	{
		_charactors.emplace_back(make_shared<Mage>(characotChipInf.level, characotChipInf.mapPos, characotChipInf.team, characotChipInf.groupNum, *this, ctrl, effects, camera));
		(*_charactors.rbegin())->SetMoveActive(characotChipInf.active);
	};

	_charactorCreateFuncs[static_cast<size_t>(CharactorType::archer)] =
		[&](const CharactorChipInf& characotChipInf, SceneController& ctrl, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera)
	{
		_charactors.emplace_back(make_shared<Archer>(characotChipInf.level, characotChipInf.mapPos, characotChipInf.team, characotChipInf.groupNum, *this, ctrl, effects, camera));
		(*_charactors.rbegin())->SetMoveActive(characotChipInf.active);
	};

	_charactorCreateFuncs[static_cast<size_t>(CharactorType::priest)] =
		[&](const CharactorChipInf& characotChipInf, SceneController& ctrl, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera)
	{
		_charactors.emplace_back(make_shared<Priest>(characotChipInf.level, characotChipInf.mapPos, characotChipInf.team, characotChipInf.groupNum, *this, ctrl, effects, camera));
		(*_charactors.rbegin())->SetMoveActive(characotChipInf.active);
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
		for (const auto& charactorChipInf : _charactorChips)
		{
			DrawCharactorChip(charactorChipInf, offset);
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
	if (mapPos.x >= 0 && mapPos.x < _mapDataVec2[0].size()
		&& mapPos.y >= 0 && mapPos.y < _mapDataVec2.size())
	{
		_mapDataVec2[mapPos.y][mapPos.x] = mapChip;
		DrawToMapChipScreen();
		return true;
	}
	return false;
}

bool MapCtrl::DrawMapChip(const Vector2Int& mapPos, const Map_Chip mapChip, const Vector2Int& offset)
{
	auto drawData = Application::Instance().GetDataBase().GetMapChipData(mapChip).drawData;

	auto graphH = Application::Instance().GetFileSystem()->GetImageHandle((imageFolderPath + drawData.path).c_str());
	DrawRectExtendGraph(offset.x + CHIP_SIZE_W * mapPos.x, offset.y + CHIP_SIZE_H * mapPos.y,
		offset.x + CHIP_SIZE_W * mapPos.x + CHIP_SIZE_W, offset.y + CHIP_SIZE_H * mapPos.y + CHIP_SIZE_H,
		drawData.leftup.x, drawData.leftup.y, drawData.size.w, drawData.size.h,
		graphH, true);

	return true;
}

bool MapCtrl::SetCharactorChip(const CharactorChipInf& charactorChipInf)
{
	for (auto itr = _charactorChips.begin(); itr != _charactorChips.end();)
	{
		if (itr->mapPos == charactorChipInf.mapPos)
		{
			itr = _charactorChips.erase(itr);
		}
		else
		{
			itr++;
		}
	}
	if (charactorChipInf.type == CharactorType::max)
	{
		return true;
	}
	_charactorChips.emplace_back(charactorChipInf);
	return true;
}

bool MapCtrl::DrawCharactorChip(const CharactorChipInf& charactorChipInf, const Vector2Int& offset)
{
	auto chipSize = GetChipSize();
	Vector2Int leftup = offset + charactorChipInf.mapPos * chipSize;
	if (charactorChipInf.type == CharactorType::max)
	{
		return false;
	}
	int handle = Application::Instance().GetDataBase().GetCharactorImageHandle(charactorChipInf.type, charactorChipInf.team);
	DrawRectExtendGraph(leftup.x, leftup.y, leftup.x + chipSize.w, leftup.y + chipSize.h, 0, 0, 32, 32, handle, true);
	DrawFormatString(leftup.x, leftup.y, 0x000000, "Level.%d", charactorChipInf.level);
	DrawFormatString(leftup.x, leftup.y + 16, 0x000000, "GN.%d", charactorChipInf.groupNum);
	DrawFormatString(leftup.x, leftup.y + 32, 0x000000, charactorChipInf.active ? "Active : True" : "False");

	return true;
}

void MapCtrl::CreateCharactor(SceneController& ctrl, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera)
{
	for (const auto& charactorChipInf : _charactorChips)
	{
		_charactorCreateFuncs[static_cast<size_t>(charactorChipInf.type)](charactorChipInf, ctrl, effects, camera);
	}
}

bool MapCtrl::SaveMap(const std::string fileName)
{
	FILE* fp = nullptr;

	string folderName("Resource/Map/");
	fopen_s(&fp, (folderName + fileName).c_str(), "wb");
	fseek(fp, 0, SEEK_SET);

	// マップサイズの書き込み
	auto mapSize = GetMapSize();
	fwrite(&mapSize, sizeof(mapSize), 1, fp);

	// マップチップの書き込み
	for (const auto& mapChipVec : _mapDataVec2)
	{
		fwrite(mapChipVec.data(), sizeof(Map_Chip), mapChipVec.size(), fp);
	}

	// キャラクターチップ数の書き込み
	unsigned int charactorChipCnt = _charactorChips.size();
	fwrite(&charactorChipCnt, sizeof(unsigned int), 1, fp);

	// キャラクターチップの書き込み
	fwrite(_charactorChips.data(), sizeof(CharactorChipInf), charactorChipCnt, fp);

	fclose(fp);

	return true;
}

bool MapCtrl::LoadMap(const std::string fileName)
{
	FILE* fp = nullptr;

	string folderName("Resource/Map/");
	fopen_s(&fp, (folderName + fileName).c_str(), "rb");
	fseek(fp, 0, SEEK_SET);

	if (fp == NULL)
	{
		return false;
	}

	// マップチップの読み込み
	Size mapSize;
	fread_s(&mapSize, sizeof(mapSize), sizeof(mapSize), 1, fp);

	if (mapSize != GetMapSize())
	{
		return false;
	}

	_mapDataVec2.resize(mapSize.h);
	for (auto& mapChipVec : _mapDataVec2)
	{
		mapChipVec.resize(mapSize.w);
		fread_s(mapChipVec.data(), sizeof(Map_Chip) * mapSize.w, sizeof(Map_Chip), mapSize.w, fp);
	}

	// キャラクターチップの読み込み
	unsigned int charactorChipCnt = 0;
	fread_s(&charactorChipCnt, sizeof(charactorChipCnt), sizeof(charactorChipCnt), 1, fp);
	_charactorChips.resize(charactorChipCnt);
	fread_s(_charactorChips.data(), sizeof(CharactorChipInf) * charactorChipCnt, sizeof(CharactorChipInf), charactorChipCnt, fp);

	fclose(fp);

	DrawToMapChipScreen();

	return true;
}

void MapCtrl::RouteSearch(Charactor& charactor)
{
	std::vector<std::vector<Astar::MapData>> mapVec2;
	CreateMapVec(mapVec2, charactor.GetTeam());

	return _astar->RouteSearch(charactor.GetMapPos(), charactor.GetStatus().move, charactor.GetAttackRange(), 
		mapVec2, charactor.GetResutlPosListVec2(), charactor.GetTeam(), charactor.GetStatus().heal);
}

bool MapCtrl::MoveRouteSearch(const Vector2Int& startPos, const unsigned int move, std::vector<std::vector<std::list<Astar::ResultPos>>>& resutlPosListVec2, const Team team)
{
	std::vector<std::vector<Astar::MapData>> mapVec2;
	CreateMapVec(mapVec2, team);

	return _astar->MoveRouteSerch(startPos, move, mapVec2, resutlPosListVec2, team);
}

Vector2Int MapCtrl::SearchMovePos(Charactor& charactor)
{
	std::vector<std::vector<Astar::MapData>> mapVec2;
	CreateMapVec(mapVec2, charactor.GetTeam());
	auto status = charactor.GetStatus();

	bool heal = charactor.GetStatus().heal;
	auto& resultPosListVec2 = charactor.GetResutlPosListVec2();
	_astar->RouteSearch(charactor.GetMapPos(), charactor.GetMoveActive() ? 100 : status.move, charactor.GetAttackRange(), 
		mapVec2, resultPosListVec2, charactor.GetTeam(), heal);

	struct TargetCharactor
	{
		Charactor* charactor;
		int distance;

		TargetCharactor():charactor(nullptr), distance(1){};
		TargetCharactor(Charactor* ch, const int di) :charactor(ch), distance(di) {};
	};

	// 範囲内の敵を格納するリスト
	list<TargetCharactor> targetCharactorList;
	// 派以外の敵を格納するリスト
	list<Charactor*> outRangeCharactorList;
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
				if (heal != (charactor.GetTeam() == mapCharactor->GetTeam()))continue;

				// 攻撃範囲外
				if (resultPos.moveCnt > status.move)
				{
					outRangeCharactorList.emplace_back(mapCharactor);
					continue;
				}

				auto mapPosSub = mapCharactor->GetMapPos() - (resultPos.prev == nullptr ? charactor.GetMapPos() : resultPos.prev->mapPos);
				int distance = abs(mapPosSub.x) + abs(mapPosSub.y);
				targetCharactorList.emplace_back(TargetCharactor(mapCharactor, distance));
			}
		}
	}
	
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
		if (status.heal)
		{
			// 最もダメージを受けているキャラクターを探す
			targetCharactorList.sort([](const TargetCharactor& left, const TargetCharactor& right)
			{
				return left.charactor->GetHurtPoint() > right.charactor->GetHurtPoint();
			});
			return targetCharactorList.begin()->charactor->GetMapPos();
		}
		else
		{
			// 最もダメージを与えられるキャラクターを探す
			targetCharactorList.sort([&](const TargetCharactor& left, const TargetCharactor& right)
			{
				return status.GetDamage(left.charactor->GetStatus()) > status.GetDamage(right.charactor->GetStatus());
			});
			return targetCharactorList.begin()->charactor->GetMapPos();
		}
	}

	if (outRangeCharactorList.size() > 0)
	{
		auto charactor = *outRangeCharactorList.begin();
		return charactor->GetMapPos();
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
			mapVec2[y][x] = Astar::MapData(Application::Instance().GetDataBase().GetMapChipData(_mapDataVec2[y][x]).moveCost, Team::max, false);
		}
	}
	for (const auto& charactor : _charactors)
	{
		auto mapPos = charactor->GetMapPos();
		mapVec2[mapPos.y][mapPos.x].team = charactor->GetTeam();
		mapVec2[mapPos.y][mapPos.x].isHurt = charactor->GetHurtPoint() > 0;
	}
}

DataBase::MapChipData MapCtrl::GetMapChipData(const Vector2Int& mapPos) const
{
	return Application::Instance().GetDataBase().GetMapChipData(_mapDataVec2[mapPos.y][mapPos.x]);
}

void MapCtrl::CreateWarSituation()const
{
	SetDrawScreen(_warSituationH);
	ClsDrawScreen();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 230);
	// マップチップの描画
	for (int y = 0; y < MAP_CHIP_CNT_H; y++)
	{
		for (int x = 0; x < MAP_CHIP_CNT_W; x++)
		{
			DrawBox(x * WAR_SITUATION_CHIP_SIZE, y * WAR_SITUATION_CHIP_SIZE,
				(x+1) * WAR_SITUATION_CHIP_SIZE, (y+1) * WAR_SITUATION_CHIP_SIZE, 
				GetMapChipData(Vector2Int(x,y)).simpleColor, true);
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

	SetDrawScreen(DX_SCREEN_BACK);
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

bool MapCtrl::CreateCharactorData()
{
	FILE* fp = nullptr;

	fopen_s(&fp, "Resource/SaveData/savedata", "ab");
	if (fp == nullptr)
	{
		return false;
	}
	// キャラクター数の書き込み
	int cnt = 6;
	fwrite(&cnt, sizeof(cnt), 1, fp);
	// キャラクターデータの書き込み
	for (CharactorType type = static_cast<CharactorType>(0); type != CharactorType::max; type = static_cast<CharactorType>(static_cast<int>(type) + 1))
	{
		SaveData saveData(type, GetLevelInitStatus(5, type));
		fwrite(&saveData, sizeof(saveData), 1, fp);
	}

	fclose(fp);

	return true;
}

bool MapCtrl::SaveCharactorData()
{
	FILE* fp = nullptr;

	fopen_s(&fp, "Resource/SaveData/savedata", "ab");
	fseek(fp, 0, SEEK_SET);

	int cnt = 0;
	for (const auto& charactor : _charactors)
	{
		if (charactor->GetTeam() != Team::player)continue;
		cnt++;
	}
	// キャラクター数の書き込み
	fwrite(&cnt, sizeof(cnt), 1, fp);
	// キャラクターデータの書き込み
	for (const auto& charactor : _charactors)
	{
		if (charactor->GetTeam() != Team::player)continue;
		SaveData saveData(charactor->GetCharactorType(), charactor->GetStatus());

		fwrite(&saveData, sizeof(saveData), 1, fp);
	}

	fclose(fp);

	return true;
}

bool MapCtrl::LoadCharactorData()
{
	FILE* fp = nullptr;

	fopen_s(&fp, "Resource/SaveData/savedata", "rb");
	fseek(fp, 0, SEEK_SET);

	if (fp == NULL)
	{
		return false;
	}

	// キャラクターサイズの読み込み
	int cnt = 0;
	fread_s(&cnt, sizeof(cnt), sizeof(cnt), 1, fp);
	_charactorSaveDataVec.resize(cnt);

	// キャラクターデータの読み込み
	fread_s(_charactorSaveDataVec.data(), sizeof(SaveData) * cnt, sizeof(SaveData), cnt, fp);

	return true;
}

Status MapCtrl::GetLevelInitStatus(const uint8_t level, const CharactorType charType)
{
	Status status;
	auto charactorData = Application::Instance().GetDataBase().GetCharactorData(charType);
	status = charactorData.initialStatus;
	status.level = level;
	status.health += level * charactorData.statusGrowRate.health / 100.0f;
	status.power += level * charactorData.statusGrowRate.power / 100.0f;
	status.defense += level * charactorData.statusGrowRate.defense / 100.0f;
	status.magic_defense += level * charactorData.statusGrowRate.magic_defense / 100.0f;
	status.speed += level * charactorData.statusGrowRate.speed / 100.0f;
	status.skill += level * charactorData.statusGrowRate.skill / 100.0f;
	return status;
}
