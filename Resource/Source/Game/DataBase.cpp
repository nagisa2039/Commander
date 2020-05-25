#include "DataBase.h"
#include <fstream>
#include <sstream>
#include "Application.h"
#include "FileSystem.h"

using namespace std;

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
	// _attributeRateTableの初期化
	for (auto& valueArray : _attributeRateTable)
	{
		valueArray.fill(1.0f);
	}

	SetAttributeRateValue(Attribute::red, Attribute::green, true);
	SetAttributeRateValue(Attribute::red, Attribute::blue, false);
	SetAttributeRateValue(Attribute::green, Attribute::blue, true);

	auto split = [](const string& input, const char delimiter, vector<string>& output)
	{
		istringstream stream(input);
		string field;
		output.clear();
		while (getline(stream, field, delimiter))
		{
			output.emplace_back(field);
		}
	};

	{
		ifstream ifs("Resource/DataBase/charactorDataBase.csv");
		string line;
		vector<string> outputVec;
		outputVec.reserve(20);
		// 最初の行はスキップ
		getline(ifs, line);
		int idx = 0;
		while (getline(ifs, line))
		{
			split(line, ',', outputVec);
			CharactorData charactorData;
			// キャラクターID
			// 職業名
			charactorData.name = outputVec[1];
			// 攻撃範囲
			charactorData.range = Range(atoi(outputVec[2].c_str()), atoi(outputVec[3].c_str()));
			// 初期ステータス
			charactorData.initialStatus = Status(1, atoi(outputVec[4].c_str()), atoi(outputVec[5].c_str()), atoi(outputVec[6].c_str()),
				atoi(outputVec[7].c_str()), atoi(outputVec[8].c_str()), atoi(outputVec[9].c_str()), atoi(outputVec[10].c_str()), Attribute::red, outputVec[18] != "", outputVec[19] != "");
			// ステータス成長率
			charactorData.statusGrowRate = Status(1, atoi(outputVec[11].c_str()), atoi(outputVec[12].c_str()), atoi(outputVec[13].c_str()),
				atoi(outputVec[14].c_str()), atoi(outputVec[15].c_str()), atoi(outputVec[16].c_str()), atoi(outputVec[17].c_str()), Attribute::red, outputVec[18] != "", outputVec[19] != "");
			// キャラクター画像パス
			charactorData.ImagePath = outputVec[20];
			// 職業アイコンパス
			charactorData.iconImagePath = outputVec[21];

			_charactorDataTable[idx++] = charactorData;
		}
	}

	{
			ifstream ifs("Resource/DataBase/mapChipDataBase.csv");
			string line;
			vector<string> outputVec;
			outputVec.reserve(20);
			// 最初の行はスキップ
			getline(ifs, line);
			int idx = 0;
			while (getline(ifs, line))
			{
				split(line, ',', outputVec);
				MapChipData mapChipData;
				// 名前
				mapChipData.name = outputVec[1];
				// パス
				mapChipData.drawData.path = outputVec[2];
				// 画像の左上座標
				mapChipData.drawData.leftup = Vector2Int(atoi(outputVec[3].c_str()), atoi(outputVec[4].c_str()));
				// 画像のサイズ
				mapChipData.drawData.size = Size(atoi(outputVec[5].c_str()), atoi(outputVec[6].c_str()));
				// 簡略化色
				mapChipData.simpleColor = atoi(outputVec[7].c_str());
				// 移動コスト
				mapChipData.moveCost = atoi(outputVec[8].c_str());
				// 防御補正値
				mapChipData.defense = atoi(outputVec[9].c_str());
				// 回避補正値
				mapChipData.avoidance = atoi(outputVec[10].c_str());
				// 回復量
				mapChipData.recovery = atoi(outputVec[11].c_str());

				_mapChipDataTable[idx] = mapChipData;
				idx++;
			}
	}
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

int DataBase::GetCharactorImageHandle(const CharactorType charactorType, const Team team) const
{
	char str[256];
	sprintf_s(str, 256, "%s%s", GetCharactorData(charactorType).ImagePath.c_str(), team == Team::player ? "_player.png" : "_enemy.png");
	return Application::Instance().GetFileSystem()->GetImageHandle(str);
}

const DataBase::CharactorData& DataBase::GetCharactorData(const CharactorType charactorType) const
{
	return _charactorDataTable.at(static_cast<size_t>(charactorType));
}

const DataBase::MapChipData& DataBase::GetMapChipData(const Map_Chip mapChip) const
{
	return _mapChipDataTable.at(static_cast<size_t>(mapChip));
}
