#pragma once
#include "Charactor.h"
class Soldier :
	public Charactor
{
private:

public:
	Soldier(const uint8_t level, const Vector2Int& mapPos, const Team team, const unsigned int groupNum, MapCtrl& mapCtrl, SceneController& ctrl,
		std::vector<std::shared_ptr<Effect>>& effects, Camera& camera);
	~Soldier();
};

#include "BattleCharactor.h"

class SoldierBC :
	public BattleCharactor
{
public:
	SoldierBC(Charactor& charactor, const int imageHandle, Camera& camera);
	~SoldierBC();

	std::shared_ptr<Effect> CreateAttackEffect(std::vector<std::shared_ptr<Effect>> effects)override final;
};

