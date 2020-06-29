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
	uint8_t _currentExp;
	uint8_t _currentAddExp;
	uint8_t _addExp;
	const uint8_t _maxExp;

	Status _addStatus;

	std::unique_ptr<Track<int>> _expAnimTrack;
	std::unique_ptr<Track<int>> _itemAnimTrack;
	
	int _endCnt;

	void StartEndUpdate();

	void AddExpUpdate(const Input& input);
	void LevelUpUpdate(const Input& input);
	void EndUpdate(const Input& input);
	void (Experience::* _updater)(const Input& input);

	void ExpBerDraw();
	void LevelUpDraw();
	void (Experience::* _drawer)();

	struct drawData
	{
		std::string name;
		uint8_t current;
		uint8_t add;

		drawData():name(""), current(0), add(0) {};
		drawData(const std::string& na, const uint8_t cu, const uint8_t ad) :name(na), current(cu), add(ad) {};
	};

	enum class Item
	{
		health,
		power,
		magic_power,
		skill,
		speed,
		defence,
		magic_defence,
		luck,
		max
	};
	std::array<drawData, static_cast<size_t>(Item::max)> _drawDatas;
	int _drawIdx;

public:
	Experience(BattleCharactor& battleChar, const bool kill, std::deque<std::shared_ptr<UI>>& uiDeque);
	~Experience();

	void Update(const Input& input)override;
	void Draw()override;
};

