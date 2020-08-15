#pragma once
#include <vector>
#include <string>
#include <array>
#include "MapChip.h"
#include <memory>
#include "Astar.h"
#include <functional>
#include "DataBase.h"
#include <windows.h>

class Camera;
class Charactor;
class SceneController;
class Effect;
class Map;

class MapCtrl
{
private:
	std::unique_ptr<Astar> _astar;
	std::shared_ptr<Map> _map;

	int _warSituationH;

	std::vector<std::shared_ptr<Charactor>>& _charactors;

	// CharactorTypeに対応したCharactorクラスを_charactorsに追加する
	std::array<std::function<void(const CharactorChipInf&, const Status&, SceneController&, std::vector<std::shared_ptr<Effect>>&, Camera&)>,
		static_cast<size_t>(CharactorType::max)> _charactorCreateFuncs;

	template<typename T>
	inline void CreateCharactor(const CharactorChipInf& characotChipInf, const Status& initStatus,
		SceneController& ctrl, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera);

public:
	MapCtrl(const int mapId, std::vector<std::shared_ptr<Charactor>>& charactors);
	~MapCtrl();

	void Draw(const Camera& camera);

	bool DrawSortieMass(const Vector2Int& offset, const CharactorChipInf& charactorChipInf, const unsigned int color = 0x00ffff, const unsigned int frameColor = 0x0000ff);

	// そのマスにいるキャラクターを返す
	Charactor* GetMapPosChar(const Vector2Int mapPos)const;

	const Size& GetChipSize()const;
	const Size& GetMapSize()const;

	void CreateCharactor(SceneController& ctrl, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera);

	// 移動可能なマスを探す
	void RouteSearch(Charactor& charactor);

	// 指定した移動力で行ける場所を探す
	bool MoveRouteSearch(const Vector2Int& startPos, const unsigned int move, std::list<Astar::ResultPos>& resultPosList, const Team team, const std::list<Astar::ResultPos>& excludeList);

	// 指定キャラから視野範囲にいるキャラクターを返す
	Vector2Int SearchMovePos(Charactor& charactor, Vector2Int& targetCnt);

	void CreateMapVec(std::vector<std::vector<Astar::MapData>>& mapVec2, const Team team);

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

	// 指定した集団のmoveActiveを変更
	void SetGroupActive(const unsigned int groupNum, const bool active);

	// キャラクターの配列を取得
	const std::vector<std::shared_ptr<Charactor>>& GetCharacots()const;

	const std::shared_ptr<Map>& GetMap()const;
};

template<typename T>
inline void MapCtrl::CreateCharactor(const CharactorChipInf& characotChipInf, const Status& initStatus, SceneController& ctrl, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera)
{
	auto charactor = make_shared<T>(characotChipInf.level, characotChipInf.mapPos, characotChipInf.team, characotChipInf.groupNum, *this, ctrl, effects, camera);
	charactor->SetMoveActive(characotChipInf.active);
	charactor->InitStatus(initStatus);
	_charactors.emplace_back(charactor);
}
