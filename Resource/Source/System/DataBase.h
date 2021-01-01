#pragma once
#include <array>
#include <string>
#include <vector>
#include <memory>
#include "../Utility/Geometry.h"
#include "Team.h"
#include "MapChip.h"
#include "WeaponData.h"
#include "Status.h"

class BattleEffectFactory;
class Map;

/// <summary>
/// データベース管理クラス
/// </summary>
class DataBase
{
public:
	/// <summary>
	/// 描画データ
	/// </summary>
	struct DrawData
	{ 
		// 左上座標
		Vector2Int leftup;
		// 描画サイズ
		Size size;
		// 画像パス
		std::string path;
	};

	/// <summary>
	/// マップチップ情報
	/// </summary>
	struct MapChipData
	{
		// 描画データ
		DrawData drawData = {};
		// 移動コスト
		int moveCost = 0;
		// 防御補正値
		int defense = 0;
		// 回避補正値
		int avoidance = 0;
		// 回復量
		int recovery = 0;
		// 表示名
		std::string name ="";
		// 戦況確認画面で表示するときの色
		unsigned int simpleColor=0;
	};

private:
	DataBase();
	DataBase(const DataBase&) = delete;
	DataBase& operator=(const DataBase&) = delete;

	/// <summary>
	/// キャラクター情報
	/// </summary>
	struct CharactorData
	{
		// 表示名
		std::string name;
		// 武器種
		uint8_t weaponType = 0;
		// 初期ステータス
		Status initialStatus = {};
		// ステータス成長率
		Status statusGrowRate = {};
		// 画像ファイルパス
		std::string imagePath;
		// アイコン画像パス
		std::string iconImagePath;

		/// <summary>
		/// アイコン描画
		/// </summary>
		/// <param name="rect">表示矩形</param>
		/// <param name="team">チーム</param>
		void DrawIcon(const Rect& rect, const Team team)const;
	};

	/// <summary>
	/// 属性データ
	/// </summary>
	struct AttributeData
	{
		// 属性名
		std::string name;
		// 表示色
		unsigned int color;
	};

	/// <summary>
	/// 攻撃エフェクトデータ
	/// </summary>
	struct BattleEffectData
	{
		// エフェクト名
		std::string name;
		// SEファイルパス
		std::string seName;
	};

	// キャラクターデータテーブル
	std::vector<CharactorData> _charactorDataTable;
	// マップチップデータテーブル
	std::vector<DataBase::MapChipData> _mapChipDataTable;
	// 属性データテーブル
	std::vector<AttributeData> _attributeDataTable;
	// 属性相性データテーブル
	std::vector<std::vector<float>> _attributeRateTable;
	// 武器種データテーブル
	std::vector<WeaponTypeData> _weaponTypeDataTable;
	// 武器データテーブル
	std::vector<WeaponData> _weaponDataTable;
	// マップデータテーブル
	std::vector<std::shared_ptr<Map>> _mapTable;
	// 戦闘エフェクトデータテーブル
	std::vector<BattleEffectData> _battleEffectDataTable;

	// 戦闘エフェクト生成クラス
	std::unique_ptr<BattleEffectFactory> _battleEffectFactory;

	// 事前読込するリソース名を格納
	std::vector<std::string> _advanceLoadTable;

	/// <summary>
	/// DATAファイル(CSVをバイナリ化したもの)読込
	/// </summary>
	/// <param name="path">ファイルパ</param>
	/// <param name="out">文字列を区切り2次元配列に格納</param>
	void ReadData(const char* path, std::vector<std::vector<std::string>>& out);

public:
	~DataBase();

	/// <summary>
	/// DataBaseの参照取得
	/// </summary>
	static DataBase& Instance()
	{
		static DataBase instance;
		return instance;
	}

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 属性ごとの威力倍率を取得する
	/// </summary>
	/// <param name="selfAttributeId">攻撃する側,</param>
	/// <param name="targetAttributeId">攻撃される側</param>
	float GetAttributeRate(const uint8_t selfAttributeId, const uint8_t targetAttributeId)const;

	/// <summary>
	/// キャラクターの画像ハンドルを取得する
	/// </summary>
	/// <param name="charactorType">キャラクター種</param>
	/// <param name="team">チーム</param>
	int GetCharactorImageHandle(const CharactorType charactorType, const Team team)const;

	/// <summary>
	/// キャラクターのデータを取得する
	/// </summary>
	/// <param name="charactorType">キャラクター種</param>
	const CharactorData& GetCharactorData(const CharactorType charactorType)const;

	/// <summary>
	/// キャラクターのデータテーブルを取得
	/// </summary>
	const std::vector<CharactorData>& GetCharactorDataTable()const;

	/// <summary>
	/// タイプとレベルを引数に期待値でステータスを作成する
	/// </summary>
	/// <param name="level">レベル</param>
	/// <param name="charType">キャラクター種</param>
	Status GetLevelInitStatus(const uint8_t level, const CharactorType charType)const;

	/// <summary>
	/// マップチップデータを取得する
	/// </summary>
	/// <param name="mapChip">マップチップ</param>
	const MapChipData& GetMapChipData(const Map_Chip mapChip)const;

	/// <summary>
	/// マップチップのデータテーブルを取得
	/// </summary>
	const std::vector<MapChipData>& GetMapChipDataTable()const;

	/// <summary>
	/// 属性データを取得する
	/// </summary>
	/// <param name="attlebuteId">属性ID</param>
	const AttributeData& GetAttributeData(const uint8_t attlebuteId)const;

	/// <summary>
	/// マップデータを取得する
	/// </summary>
	const std::vector<std::shared_ptr<Map>>& GetMapDataTable()const;

	/// <summary>
	/// マップデータを取得
	/// </summary>
	/// <param name="mapDataId">マップデータID</param>
	const std::shared_ptr<Map> GetMapData(const unsigned int mapDataId)const;

	/// <summary>
	/// WeaponTypeDataを取得する
	/// </summary>
	/// <param name="type">武器種</param>
	const WeaponTypeData& GetWeaponTypeData(const uint8_t type)const;

	/// <summary>
	/// 武器IDから武器種の取得
	/// </summary>
	/// <param name="weaponId">武器ID</param>
	const WeaponTypeData& GetWeaponTypeDataFromWeaponId(const uint8_t weaponId)const;

	/// <summary>
	/// WeaponDataを取得する
	/// </summary>
	/// <param name="weaponId">武器ID</param>
	const WeaponData& GetWeaponData(const unsigned int weaponId)const;
	/// <summary>
	/// weaponDataの配列取得
	/// </summary>
	const std::vector<WeaponData>& GetWeaponDataTable()const;

	/// <summary>
	/// 攻撃エフェクト生成クラスを取得
	/// </summary>
	const BattleEffectFactory& GetBattleEffectFactory()const;

	/// <summary>
	/// BattleEffectDataの取得
	/// </summary>
	/// <param name="type">戦闘エフェクト種</param>
	const BattleEffectData& GetBattleEffectData(const BattleEffectType type)const;

	/// <summary>
	/// 事前読込するリソースパス取得
	/// </summary>
	const std::vector<std::string>& GetAdvanceLoadTable()const;
};

