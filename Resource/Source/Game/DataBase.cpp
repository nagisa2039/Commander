#include "DataBase.h"

bool DataBase::SetAttributeRateValue(const Attribute self, const Attribute target, const bool advantage)
{
	auto selfSize_t = static_cast<size_t>(self);
	auto targetSize_t = static_cast<size_t>(target);

	if (_attributeRateTable.size() <= selfSize_t
		|| _attributeRateTable[0].size() <= targetSize_t)
	{
		return false;
	}

	const float good = 1.2f;
	const float bad = 0.8f;

	_attributeRateTable[selfSize_t][targetSize_t] = advantage	? good : bad;
	_attributeRateTable[targetSize_t][selfSize_t] = !advantage	? good : bad;

	return true;
}

DataBase::DataBase()
{
	// _attributeRateTable‚Ì‰Šú‰»
	for (auto& valueArray : _attributeRateTable)
	{
		valueArray.fill(1.0f);
	}

	SetAttributeRateValue(Attribute::red, Attribute::green, true);
	SetAttributeRateValue(Attribute::red, Attribute::blue, false);
	SetAttributeRateValue(Attribute::green, Attribute::blue, true);
}

DataBase::~DataBase()
{
}

float DataBase::GetAttributeRate(const Attribute self, const Attribute target)const
{
	auto selfSize_t = static_cast<size_t>(self);
	auto targetSize_t = static_cast<size_t>(target);

	if (_attributeRateTable.size() <= selfSize_t
		|| _attributeRateTable[0].size() <= targetSize_t)
	{
		return 0.0f;
	}

	return _attributeRateTable.at(selfSize_t).at(targetSize_t);
}
