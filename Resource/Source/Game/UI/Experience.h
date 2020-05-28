#pragma once
#include "UI.h"
#include "TimeLine.h"
#include <array>
#include "Status.h"

class BattleCharactor;

class Experience :
	public UI
{
private:
	BattleCharactor& _battleChar;
	unsigned int _currentExp;
	unsigned int _currentAddExp;
	unsigned int _maxExp;
	unsigned int _addExp;

	Status _addStatus;

	std::unique_ptr<Track<int>> _itemAnimTrack;

	void AddExpUpdate(const Input& input);
	void LevelUpUpdate(const Input& input);
	void (Experience::* _updater)(const Input& input);

	struct drawData
	{
		std::string name;
		std::string current;
		std::string add;

		drawData():name(""), current(""), add("") {};
		drawData(const char* na, const char* cu, const char* ad) :name(na), current(cu), add(ad) {};
	};

	enum class Item
	{
		health,
		power,
		skill,
		speed,
		defence,
		magic_defence,
		max
	};
	std::array<drawData, static_cast<size_t>(Item::max)> _drawDatas;
	int _drawIdx;

public:
	Experience(BattleCharactor& battleChar, std::deque<std::shared_ptr<UI>>& uiDeque);
	~Experience();

	void Update(const Input& input)override;
	void Draw()override;
};

