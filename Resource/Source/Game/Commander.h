#pragma once
#include "MapCursor.h"
#include <memory>
#include <vector>
#include "Team.h"

class Charactor;
class Animator;

class Commander :
	public MapCursor
{
protected:
	std::vector<std::shared_ptr<Charactor>>& _charactors;
	Charactor* _selectChar;
	const Team _ctrlTeam;
	int _animCnt;

	int _rigid;

	bool _end;

	bool ChaeckCharactor();


public:
	Commander(std::vector<std::shared_ptr<Charactor>>& charactors, MapCtrl& mapCtrl, const Team ctrlTeam);
	~Commander();

	virtual void Update(const Input& input)override = 0;
	virtual void Draw(const Camera& camera)override = 0;

	void End();
	bool CheckEnd()const;

	void TurnReset();
};

