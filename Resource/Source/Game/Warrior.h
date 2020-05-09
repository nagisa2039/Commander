#pragma once
#include "Charactor.h"
class Warrior :
	public Charactor
{
private:

public:
	Warrior(const uint8_t level, const Vector2Int& mapPos, const Team team, MapCtrl& mapCtrl, SceneController& ctrl,
		std::vector<std::shared_ptr<Effect>>& effects, Camera& camera);
	~Warrior();
};

#include "BattleCharactor.h"

class WarriorBC :
	public BattleCharactor
{
public:
	WarriorBC(Charactor& charactor, const int imageHandle, Camera& camera);
	~WarriorBC();

	std::shared_ptr<Effect> CreateAttackEffect(const Vector2Int& effectPos)override final;
};

