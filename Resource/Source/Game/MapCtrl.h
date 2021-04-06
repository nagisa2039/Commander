#pragma once
#include <vector>
#include <string>
#include <array>
#include "MapChip.h"
#include <memory>
#include <functional>
#include "DataBase.h"
#include <windows.h>
#include "RouteSearchData.h"

class Camera;
class Charactor;
class SceneController;
class Effect;
class Map;
class RouteSearch;

/// <summary>
/// マップ管理クラス
/// </summary>
class MapCtrl
{
private:
	// ルート検索クラス
	std::unique_ptr<RouteSearch> _routeSearch;
	// マップ情報
	std::shared_ptr<Map> _map;

	// 戦況確認画面ハンドル
	int _warSituationH;

	// キャラクター管理配列
	std::vector<std::shared_ptr<Charactor>>& _charactors;

	/// <summary>
	/// キャラクターの生成
	/// </summary>
	/// <param name="characotChipInf">キャラクター情報</param>
	/// <param name="initStatus">初期ステータス</param>
	/// <param name="ctrl">シーン管理クラス</param>
	/// <param name="effects">エフェクト管理配列</param>
	/// <param name="camera">カメラ</param>
	void CreateCharactor(const CharactorChipInf& characotChipInf, const Status& initStatus, SceneController& ctrl, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera);

public:
	/// <param name="mapId">マップID</param>
	/// <param name="charactors">キャラクター管理配列</param>
	MapCtrl(const int mapId, std::vector<std::shared_ptr<Charactor>>& charactors);
	~MapCtrl();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Draw(const Camera& camera);

	/// <summary>
	/// 出撃可能マスの描画
	/// </summary>
	/// <param name="offset">描画オフセット</param>
	/// <param name="charactorChipInf">キャラクターチップ情報</param>
	/// <param name="color">色</param>
	/// <param name="frameColor">フレーム色</param>
	/// <returns>プレイヤーマスか</returns>
	bool DrawSortieMass(const Vector2Int& offset, const CharactorChipInf& charactorChipInf, const unsigned int color = 0x00ffff, const unsigned int frameColor = 0x0000ff);

	/// <summary>
	/// そのマスにいるキャラクターを返す
	/// </summary>
	/// <param name="mapPos">マップ座標</param>
	Charactor* GetMapPosChar(const Vector2Int mapPos)const;

	/// <summary>
	/// マップチップサイズの取得
	/// </summary>
	const Size& GetChipSize()const;

	/// <summary>
	/// マップチップ数の取得
	/// </summary>
	const Size& GetMapSize()const;

	/// <summary>
	/// キャラクターの生成
	/// </summary>
	/// <param name="ctrl">シーン管理クラス</param>
	/// <param name="effects">エフェクト管理配列</param>
	/// <param name="camera">カメラ</param>
	void CreateCharactor(SceneController& ctrl, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera);

	/// <summary>
	/// ルート検索用マップデータの取得
	/// </summary>
	/// <param name="mapVec2">結果格納用ルート検索用マップデータ</param>
	void CreateMapVec(SearchData::SearchMapData& mapVec2);

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
	RouteSearch& GetRouteSerch();
};

