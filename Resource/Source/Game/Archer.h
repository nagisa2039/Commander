#pragma once
#include "Charactor.h"
class Archer :
	public Charactor
{
private:

public:
	Archer(const uint8_t level, const Vector2Int& mapPos, const Team team, const unsigned int groupNum, MapCtrl& mapCtrl, SceneController& ctrl,
		std::vector<std::shared_ptr<Effect>>& effects, Camera& camera);
	~Archer();
};

#include "BattleCharactor.h"

class ArcherBC :
	public BattleCharactor
{
public:
	ArcherBC(Charactor& charactor, const int imageHandle, Camera& camera);
	~ArcherBC();

	std::shared_ptr<Effect> CreateAttackEffect(std::vector<std::shared_ptr<Effect>>& effects)override final;
};