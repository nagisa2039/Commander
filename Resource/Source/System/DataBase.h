#pragma once
#include <array>
#include <string>
#include <vector>
#include <memory>
#include "../Utility/Geometry.h"
#include "Team.h"
#include "MapChip.h"
#include "WeaponData.h"
#include "SaveDataCharactor.h"

class BattleEffectFactory;
class Map;

class DataBase
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
		DrawData drawData = {};
		int moveCost = 0;
		int defense = 0;
		int avoidance = 0;
		int recovery = 0;
		std::string name ="";
		unsigned int simpleColor=0;
	};

private:
	DataBase();
	DataBase(const DataBase&) = delete;
	DataBase& operator=(const DataBase&) = delete;

	struct CharactorData
	{
		std::string name;
		uint8_t weaponType = 0;
		Status initialStatus;
		Status statusGrowRate;
		std::string imagePath;
		std::string iconImagePath;

		void DrawIcon(const Rect& rect, const Team team)const;
	};

	//struct ExpData
	//{
	//	unsigned int maxPoint;	// 必要量
	//	unsigned int getPoint;	// 取得量
	//	ExpData() :maxPoint(0), getPoint(0) {};
	//	ExpData(unsigned int m, unsigned int g) :maxPoint(m), getPoint(g) {};
	//};

	struct AttributeData
	{
		std::string name;
		unsigned int color;
		AttributeData():name(""), color(0) {};
		AttributeData(const std::string& na, const unsigned int c) :name(na), color(c) {};

	};

	struct BattleEffectData
	{
		std::string name;
		std::string seName;
	};

	std::vector<CharactorData> _charactorDataTable;

	std::vector<DataBase::MapChipData> _mapChipDataTable;

	//std::vector<ExpData> _expDataTable;

	std::vector<AttributeData> _attributeDataTable;
	std::vector<std::vector<float>> _attributeRateTable;

	std::vector<WeaponTypeData> _weaponTypeDataTable;
	std::vector<WeaponData> _weaponDataTable;

	std::vector<std::shared_ptr<Map>> _mapTable;

	std::vector<SaveDataCharactor> _saveDataCharactors;	// セーブデータ生成用

	std::vector<BattleEffectData> _battleEffectDataTable;
	std::unique_ptr<BattleEffectFactory> _battleEffectFactory;

	std::vector<std::string> _advanceLoadTable;	// 事前読込するリソース名を格納

	bool ReadData(const char* path, std::vector<std::vector<std::string>>& out);

public:
	~DataBase();

	static DataBase& Instance()
	{
		static DataBase instance;
		return instance;
	}

	void Init();

	// 属性ごとの威力倍率を取得する		GetAttributeRate(攻撃する側,	攻撃される側)
	float GetAttributeRate(const uint8_t selfAttributeId, const uint8_t targetAttributeId)const;

	// キャラクターの画像を取得する
	int GetCharactorImageHandle(const CharactorType charactorType, const Team team)const;

	// キャラクターのﾃﾞｰﾀを取得する
	const CharactorData& GetCharactorData(const CharactorType charactorType)const;

	/// <summary>
	/// キャラクターのデータテーブルを取得
	/// </summary>
	const std::vector<CharactorData>& GetCharactorDataTable()const;

	// キャラクタータイプとレベルを引数に期待値でStatusを作成する
	Status GetLevelInitStatus(const uint8_t level, const CharactorType charType)const;

	// マップチップデータを取得する
	const MapChipData& GetMapChipData(const Map_Chip mapChip)const;

	/// <summary>
	/// マップチップのデータテーブルを取得
	/// </summary>
	const std::vector<MapChipData>& GetMapChipDataTable()const;

	// 経験値データを取得する
	//const ExpData& GetExpData(const uint8_t level)const;

	// 属性データを取得する
	const AttributeData& GetAttributeData(const uint8_t attlebuteId)const;

	// マップデータを取得する
	const std::vector<std::shared_ptr<Map>>& GetMapDataTable()const;
	const std::shared_ptr<Map> GetMapData(const unsigned int mapDataId)const;

	// WeaponTypeDataを取得する
	const WeaponTypeData& GetWeaponTypeData(const uint8_t type)const;
	const WeaponTypeData& GetWeaponTypeDataFromWeaponId(const uint8_t weaponId)const;

	// WeaponDataを取得する
	const WeaponData& GetWeaponData(const unsigned int weaponId)const;
	// weaponDataの配列取得
	const std::vector<WeaponData>& GetWeaponDataTable()const;

	// 初期のセーブデータ作成用配列の取得
	const std::vector<SaveDataCharactor> GetSaveDataCharactors()const;

	/// <summary>
	/// 攻撃エフェクト生成クラスを取得
	/// </summary>
	const BattleEffectFactory& GetBattleEffectFactory()const;

	/// <summary>
	/// BattleEffectDataの取得
	/// </summary>
	const BattleEffectData& GetBattleEffectData(const BattleEffectType type)const;

	/// <summary>
	/// 事前読込するリソースパス取得
	/// </summary>
	const std::vector<std::string>& GetAdvanceLoadTable()const;
};

