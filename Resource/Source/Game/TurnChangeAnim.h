#pragma once
#include "Actor.h"
#include "Team.h"
#include <array>
#include <vector>
#include "TimeLine.h"

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

	Team _currentTeam;
	std::array<TeamInf, static_cast<size_t>(Team::max)> teamInfs;
	bool _animEnd;

	uint32_t _frame;

	TimeLine<float> _exrateTL;
	TimeLine<float> _alphaTL;

public:
	TurnChangeAnim();
	~TurnChangeAnim();

	void Update(const Input& input)override final;
	void Draw(const Camera& camera)override final;

	void TurnStart(const Team team);
	bool GetAnimEnd()const;
};

