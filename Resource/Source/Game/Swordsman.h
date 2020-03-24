#pragma once
#include "Charactor.h"

class Swordsman :
	public Charactor
{
public:
	Swordsman(const Vector2Int& mapPos, const Team team, MapCtrl& mapCtrl, SceneController& ctrl, 
		std::vector<std::shared_ptr<Effect>>& effects);
	~Swordsman();
};

#include "BattleCharactor.h"

class SwordBC : 
	public BattleCharactor
{
public:
	SwordBC(Charactor& charactor);
	~SwordBC();

	std::shared_ptr<Effect> CreateAttackEffect(const Vector2Int& effectPos)override final;
};

