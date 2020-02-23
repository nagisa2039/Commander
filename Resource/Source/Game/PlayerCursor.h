#pragma once
#include "MapCursor.h"
#include <memory>
#include <vector>
#include "Team.h"

class Charactor;

class PlayerCursor :
	public MapCursor
{
private:
	std::vector<std::shared_ptr<Charactor>>& _charactors;
	Charactor* _selectChar;
	const Team _ctrlTeam;

	void CharactorControl(const Input& input);

public:
	PlayerCursor(std::vector<std::shared_ptr<Charactor>>& charactors, MapCtrl& mapCtrl, const Team ctrlTeam);
	~PlayerCursor();

	void Update(const Input& input)override final;
	void Draw(const Camera& camera)override final;

	Vector2Int GetMapPos()const;
	void TurnReset();
};

