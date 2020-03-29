#pragma once
#include "Charactor.h"
class Warrior :
	public Charactor
{
private:

public:
	Warrior(const Vector2Int& mapPos, const Team team, MapCtrl& mapCtrl, SceneController& ctrl,
		std::vector<std::shared_ptr<Effect>>& effects);
	~Warrior();
};

#include "BattleCharactor.h"

class WarriorBC :
	public BattleCharactor
{
public:
	WarriorBC(Charactor& charactor);
	~WarriorBC();

	std::shared_ptr<Effect> CreateAttackEffect(const Vector2Int& effectPos)override final;
};

