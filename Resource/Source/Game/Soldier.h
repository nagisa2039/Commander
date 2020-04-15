#pragma once
#include "Charactor.h"
class Soldier :
	public Charactor
{
private:

public:
	Soldier(const uint8_t level, const Vector2Int& mapPos, const Team team, MapCtrl& mapCtrl, SceneController& ctrl,
		std::vector<std::shared_ptr<Effect>>& effects);
	~Soldier();
};

#include "BattleCharactor.h"

class SoldierBC :
	public BattleCharactor
{
public:
	SoldierBC(Charactor& charactor, const int imageHandle);
	~SoldierBC();

	std::shared_ptr<Effect> CreateAttackEffect(const Vector2Int& effectPos)override final;
};

