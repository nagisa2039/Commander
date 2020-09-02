#pragma once
#include "Charactor.h"
class Mage :
	public Charactor
{
private:

public:
	Mage(const uint8_t level, const Vector2Int& mapPos, const Team team, const unsigned int groupNum, MapCtrl& mapCtrl, SceneController& ctrl,
		std::vector<std::shared_ptr<Effect>>& effects, Camera& camera);
	~Mage();
};

#include "BattleCharactor.h"

class MageBC :
	public BattleCharactor
{
public:
	MageBC(Charactor& charactor, const int imageHandle, Camera& camera);
	~MageBC();

	std::shared_ptr<Effect> CreateAttackEffect(std::vector<std::shared_ptr<Effect>>& effects, const bool critical)override final;
};