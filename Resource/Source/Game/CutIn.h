#pragma once
#include <memory>
#include <functional>
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
	int _bgH;
	Size _backImageSize;
	int _graphH;
	Dir _dir;
	int _animCnt;
	bool _end;
	std::function<void()> _endFunc;

public:
	CutIn(const CharactorType type, const Team team, const Dir dir, std::function<void()> endFunc);
	~CutIn();

	void Update();
	void Draw();
	bool GetEnd()const;
};

