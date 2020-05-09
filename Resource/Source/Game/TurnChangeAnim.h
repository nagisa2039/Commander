#pragma once
#include "Actor.h"
#include "Team.h"
#include <array>
#include <vector>
#include "TimeLine.h"

class TurnChangeAnim
{
private:
	struct TeamInf
	{
		const char* name;

		TeamInf():name() {};
		TeamInf(const char* n) :name(n) {};
	};

	Team _currentTeam;
	std::array<TeamInf, static_cast<size_t>(Team::max)> teamInfs;
	bool _animEnd;

	std::unique_ptr<Track<float>> _exrateTL;
	std::unique_ptr<Track<float>> _alphaTL;
	std::unique_ptr<Track<float>> _angleTL;

public:
	TurnChangeAnim();
	~TurnChangeAnim();

	void Update(const Input& input);
	void Draw();

	void TurnStart(const Team team);
	bool GetAnimEnd()const;
};

