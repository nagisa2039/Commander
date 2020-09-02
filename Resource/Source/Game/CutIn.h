#pragma once
#include <memory>
#include "CharactorType.h"
#include "Team.h"
#include "TimeLine.h"
#include "Dir.h"

class CutIn
{
private:
	const CharactorType _charactorType;
	const Team _team;
	std::unique_ptr<Track_f> _animTrack;
	std::unique_ptr<Track_f> _moveTrack;
	int _graphH;
	Dir _dir;

public:
	CutIn(const CharactorType type, const Team team, const Dir dir);
	~CutIn();

	void Update();
	void Draw();
	bool GetEnd()const;
};

