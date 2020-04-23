#pragma once
#include "Charactor.h"
class Mage :
	public Charactor
{
private:

public:
	Mage(const uint8_t level, const Vector2Int& mapPos, const Team team, MapCtrl& mapCtrl, SceneController& ctrl,
		std::vector<std::shared_ptr<Effect>>& effects);
	~Mage();
};

#include "BattleCharactor.h"

class MageBC :
	public BattleCharactor
{
public:
	MageBC(Charactor& charactor, const int imageHandle);
	~MageBC();

	std::shared_ptr<Effect> CreateAttackEffect(const Vector2Int& effectPos)override final;
};