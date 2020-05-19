#pragma once
#include "Charactor.h"

class Swordsman :
	public Charactor
{
private:

public:
	Swordsman(const uint8_t level, const Vector2Int& mapPos, const Team team, const unsigned int groupNum, MapCtrl& mapCtrl, SceneController& ctrl,
		std::vector<std::shared_ptr<Effect>>& effects, Camera& camera);
	~Swordsman();
};

#include "BattleCharactor.h"

class SwordBC : 
	public BattleCharactor
{
public:
	SwordBC(Charactor& charactor, const int imageHandle, Camera& camera);
	~SwordBC();

	std::shared_ptr<Effect> CreateAttackEffect(const Vector2Int& effectPos)override final;
};

