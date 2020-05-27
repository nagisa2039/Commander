#pragma once
#include "Charactor.h"

class Priest :
	public Charactor
{
private:

public:
	Priest(const uint8_t level, const Vector2Int& mapPos, const Team team, const unsigned int groupNum, MapCtrl& mapCtrl, SceneController& ctrl,
		std::vector<std::shared_ptr<Effect>>& effects, Camera& camera);
	~Priest();
};

#include "BattleCharactor.h"

class PriestBC :
	public BattleCharactor
{
public:
	PriestBC(Charactor& charactor, const int imageHandle, Camera& camera);
	~PriestBC();

	std::shared_ptr<Effect> CreateAttackEffect(const Vector2Int& effectPos, const Vector2Int& targetPos, std::vector<std::shared_ptr<Effect>> effects)override final;
};