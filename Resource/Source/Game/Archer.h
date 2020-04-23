#pragma once
#include "Charactor.h"
class Archer :
	public Charactor
{
private:

public:
	Archer(const uint8_t level, const Vector2Int& mapPos, const Team team, MapCtrl& mapCtrl, SceneController& ctrl,
		std::vector<std::shared_ptr<Effect>>& effects);
	~Archer();
};

#include "BattleCharactor.h"

class ArcherBC :
	public BattleCharactor
{
public:
	ArcherBC(Charactor& charactor, const int imageHandle);
	~ArcherBC();

	std::shared_ptr<Effect> CreateAttackEffect(const Vector2Int& effectPos)override final;
};