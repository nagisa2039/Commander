#pragma once

#include <array>
#include "Attribute.h"
#include <string>
#include "Geometry.h"
#include "Status.h"
#include "Team.h"
#include "CharactorType.h"

class DataBase
{
private:
	std::array<std::array<float, static_cast<size_t>(Attribute::max)>, static_cast<size_t>(Attribute::max)> _attributeRateTable;

	//	テーブルに値を設定する 有利不利がセットで設定される	SetAttributeRateValue(攻撃する側,	攻撃される側, 有効か？)
	bool SetAttributeRateValue(const Attribute self, const Attribute target, const bool advantage);

	struct CharactorData
	{
		std::string name;
		Range range;
		Status initialStatus;
		Status statusGrowRate;
		std::string ImagePath;
		std::string iconImagePath;
	};

	std::array<CharactorData, static_cast<size_t>(CharactorType::max)> _charactorDataTable;

public:
	DataBase();
	~DataBase();

	// 属性ごとの威力倍率を取得する		GetAttributeRate(攻撃する側,	攻撃される側)
	float GetAttributeRate(const Attribute self, const Attribute target)const;

	int GetCharactorImageHandle(const CharactorType charactorType, const Team team)const;

	const CharactorData& GetCharactorData(const CharactorType charactorType)const;
};

