#pragma once
#include <vector>
#include <string>
#include <array>
#include "MapChip.h"
#include <memory>
#include "Astar.h"
#include "CharactorType.h"
#include <functional>

class Camera;
class Charactor;

class SceneController;
class Effect;

class MapCtrl
{
public:
	struct DrawData
	{
		Vector2Int leftup;
		Size size;
		std::string path;

		DrawData() {};

		DrawData(Vector2Int leftup, Size size, std::string path) :
			leftup(leftup), size(size), path(path) {};

	};

	struct MapChipData
	{
		DrawData drawData;
		int moveCost;
		int defense;
		int avoidance;
		const char* name;
		unsigned int simpleColor;

		MapChipData() : drawData(DrawData()), name(""), moveCost(1), defense(0), avoidance(0), simpleColor(0x000000){};
		MapChipData(const DrawData& dd, const char* na, unsigned int sc) :
			drawData(dd), name(na), simpleColor(sc), moveCost(1), defense(0), avoidance(0) {};
		MapChipData(const DrawData& dd, const char* na, unsigned int sc, const int mc) :
			drawData(dd), name(na), simpleColor(sc), moveCost(mc), defense(0), avoidance(0) {};
		MapChipData(const DrawData& dd, const char* na, unsigned int sc, const int mc, const int fe, const int av) :
			drawData(dd), name(na), simpleColor(sc), moveCost(mc), defense(fe), avoidance(av) {};
	};

private:
	std::vector<std::vector<Map_Chip>> _mapDataVec2;			// マップデータ
	std::array<MapChipData, static_cast<size_t>(Map_Chip::max)> _mapChipData;	// マップチップのデータ
	std::shared_ptr<Astar> _astar;

	std::vector<std::shared_ptr<Charactor>>& _charactors;

	std::array<std::string, static_cast<size_t>(CharactorType::max)> _iconPaths;
	std::vector<CharactorChipInf> _charactorChips;

	// CharactorTypeに対応したCharactorクラスを_charactorsに追加する
	std::array<std::function<void(const CharactorChipInf&, SceneController&, std::vector<std::shared_ptr<Effect>>&, Camera& camera)>,
		static_cast<size_t>(CharactorType::max)> _charactorCreateFuncs;

	int _mapFloorH;
	int _mapChipH;
	int _warSituationH;

	const std::string imageFolderPath;

	void DrawToMapFloorScreen();
	void DrawToMapChipScreen();

public:
	MapCtrl(std::vector<std::shared_ptr<Charactor>>& charactors);
	~MapCtrl();

	void Draw(const Camera& camera, const bool edit = false);

	Size GetChipSize()const;
	Size GetMapCnt()const;

	// そのマスにいるキャラクターを返す
	Charactor* GetMapPosChar(const Vector2Int mapPos)const;

	bool SetMapChip(const Vector2Int& mapPos, const Map_Chip mapChip);
	bool DrawMapChip(const Vector2Int& mapPos, const Map_Chip mapChip, const Vector2Int& offset = Vector2Int(0,0));

	bool SetCharactorChip(const CharactorChipInf& charactorChipInf);
	bool DrawCharactorChip(const CharactorChipInf& charactorChipInf, const Vector2Int& offset = Vector2Int(0,0));

	void CreateCharactor(SceneController& ctrl, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera);

	bool SaveMap(const std::string fileName);
	bool LoadMap(const std::string fileName);

	// 移動可能なマスを探す
	void RouteSearch(Charactor& charactor);

	// 指定した移動力で行ける場所を探す
	bool MoveRouteSearch(const Vector2Int& startPos, const unsigned int move, std::list<Astar::ResultPos>& resutlPosList, const Team team);

	// 指定キャラから視野範囲にいるキャラクターを返す
	Vector2Int SearchMovePos(Charactor& charactor);

	void CreateMapVec(std::vector<std::vector<Astar::MapData>>& mapVec2, const Team team);

	// 指定した座標のMapChipDataを取得する
	MapChipData GetMapChipData(const Vector2Int& mapPos)const;

	// 戦況確認用の画像を生成する(更新する)
	void CreateWarSituation()const;

	// 指定した個所を中心にWarSituationを描画する
	void DrawWarSituatin(const Vector2Int center)const;

	// 自軍と敵軍の数を返す(x : 自軍, y : 敵軍)
	Vector2Int GetCharactorCnt()const;

	// 未行動の自軍数を返す
	unsigned int GetCanMoveCnt()const;

	// 全キャラクターの行動範囲計算を行う
	void AllCharactorRouteSearch()const;
};