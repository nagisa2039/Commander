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

/// <summary>
/// マップ管理クラス
/// </summary>
class MapCtrl
{
private:
	// 
	std::unique_ptr<Astar> _astar;
	// 
	std::shared_ptr<Map> _map;

	// 
	int _warSituationH;

	// 
	std::vector<std::shared_ptr<Charactor>>& _charactors;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="characotChipInf"></param>
	/// <param name="initStatus"></param>
	/// <param name="ctrl"></param>
	/// <param name="effects"></param>
	/// <param name="camera"></param>
	void CreateCharactor(const CharactorChipInf& characotChipInf, const Status& initStatus, SceneController& ctrl, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera);

public:
	/// <param name="mapId"></param>
	/// <param name="charactors"></param>
	MapCtrl(const int mapId, std::vector<std::shared_ptr<Charactor>>& charactors);
	~MapCtrl();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="camera"></param>
	void Draw(const Camera& camera);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="offset"></param>
	/// <param name="charactorChipInf"></param>
	/// <param name="color"></param>
	/// <param name="frameColor"></param>
	/// <returns></returns>
	bool DrawSortieMass(const Vector2Int& offset, const CharactorChipInf& charactorChipInf, const unsigned int color = 0x00ffff, const unsigned int frameColor = 0x0000ff);

	/// <summary>
	/// そのマスにいるキャラクターを返す
	/// </summary>
	/// <param name="mapPos">マップ座標</param>
	Charactor* GetMapPosChar(const Vector2Int mapPos)const;

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	const Size& GetChipSize()const;

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	const Size& GetMapSize()const;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="ctrl"></param>
	/// <param name="effects"></param>
	/// <param name="camera"></param>
	void CreateCharactor(SceneController& ctrl, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera);

	/// <summary>
	/// ルート検索用マップデータの取得
	/// </summary>
	/// <param name="mapVec2">結果格納用ルート検索用マップデータ</param>
	void CreateMapVec(std::vector<std::vector<Astar::MapData>>& mapVec2);

	/// <summary>
	/// 戦況確認用の画像を生成する(更新する)
	/// </summary>
	void CreateWarSituation()const;

	/// <summary>
	/// 指定した個所を中心にWarSituationを描画する
	/// </summary>
	/// <param name="center">描画中心座標</param>
	void DrawWarSituatin(const Vector2Int center)const;

	/// <summary>
	/// 自軍と敵軍の数を返す(x : 自軍, y : 敵軍)
	/// </summary>
	Vector2Int GetCharactorCnt()const;

	/// <summary>
	/// 未行動の自軍数を返す
	/// </summary>
	unsigned int GetCanMoveCnt()const;

	/// <summary>
	/// 全キャラクターの行動範囲計算を行う
	/// </summary>
	void AllCharactorRouteSearch()const;

	/// <summary>
	/// 指定した集団のmoveActiveを変更
	/// </summary>
	/// <param name="team">チーム</param>
	/// <param name="groupNum">集団番号</param>
	/// <param name="active">有効化するか</param>
	void SetGroupActive(const Team team, const unsigned int groupNum, const bool active);

	/// <summary>
	/// キャラクターの配列を取得
	/// </summary>
	const std::vector<std::shared_ptr<Charactor>>& GetCharacots()const;

	/// <summary>
	/// マップ情報の取得
	/// </summary>
	std::shared_ptr<Map> GetMap()const;

	/// <summary>
	/// ルート検索オブジェクトの取得
	/// </summary>
	Astar& GetAstar();
};

