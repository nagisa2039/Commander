#pragma once
#include "UI.h"
#include "TimeLine.h"
#include <array>
#include "Status.h"

class BattleCharactor;

class Experience :
	public UI
{
private:struct 
	drawData
	{
		std::string name;
		uint8_t current;
		uint8_t add;

		drawData() :name(""), current(0), add(0) {};
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

	static int _windowStatusH;

	BattleCharactor& _battleChar;
	uint8_t _currentExp;
	uint8_t _currentAddExp;
	uint8_t _addExp;
	const uint8_t _maxExp;

	Status _addStatus;

	int _expAnimCnt;
	int _itemAnimCnt;
	int _waitCnt;

	int _drawIdx;

	std::unique_ptr<Track<float>> _scaleTrack;
	std::unique_ptr<Track<float>> _levlAnimTrack;;

	int GetNextDrawIdx();
	bool NextDrawItem();
	void End();

	void WaitUpdate(const Input& input);
	void ExpBerUpdate(const Input& input);
	void ScaleUpdate(const Input& input);
	void LevelUpUpdate(const Input& input);
	void StatusUpdate(const Input& input);

	void (Experience::* _updater)(const Input& input);
	void (Experience::* _nextUpdater)(const Input& input);

	void Wait(void(Experience::* nextUpdate)(const Input&));

	void ExpBerDraw();
	void LevelUpDraw();
	void DrawLevel(const Vector2Int& levelUpCenter, const Status& status);
	void (Experience::* _drawer)();

	void DrawToStatusWindow();
	void DrawToNumGraph(const int num);

public:
	Experience(BattleCharactor& battleChar, const bool kill, std::deque<std::shared_ptr<UI>>& uiDeque);
	~Experience();

	void Update(const Input& input)override;
	void Draw()override;
};

