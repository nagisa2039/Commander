#pragma once
#include "Actor.h"
#include "Team.h"
#include <array>
#include <vector>

class TurnChangeAnim :
	public Actor
{
private:
	struct TeamInf
	{
		const char* name;
		uint32_t color;

		TeamInf():name(), color(0) {};
		TeamInf(const char* n, uint32_t c) :name(n), color(c) {};
	};

	struct Key
	{
		int frame;

		float exrate;
		float alpha;

		Key() : frame(0), exrate(0), alpha(0){};
		Key(const float f, const float e, const float a) : frame(f), exrate(e), alpha(a) {};
	};

	Team _currentTeam;
	std::array<TeamInf, static_cast<size_t>(Team::max)> teamInfs;
	bool _animEnd;

	std::vector<TurnChangeAnim::Key> _timeLine;
	Key _currentVal;

public:
	TurnChangeAnim();
	~TurnChangeAnim();

	void Update(const Input& input)override final;
	void Draw(const Camera& camera)override final;

	void TurnStart(const Team team);
	bool GetAnimEnd()const;
};

