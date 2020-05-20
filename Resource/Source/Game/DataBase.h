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

	//	�e�[�u���ɒl��ݒ肷�� �L���s�����Z�b�g�Őݒ肳���	SetAttributeRateValue(�U�����鑤,	�U������鑤, �L�����H)
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

	// �������Ƃ̈З͔{�����擾����		GetAttributeRate(�U�����鑤,	�U������鑤)
	float GetAttributeRate(const Attribute self, const Attribute target)const;

	int GetCharactorImageHandle(const CharactorType charactorType, const Team team)const;

	const CharactorData& GetCharactorData(const CharactorType charactorType)const;
};

