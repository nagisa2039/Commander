#pragma once
#include "Charactor.h"

class Swordsman :
	public Charactor
{
public:
	Swordsman(const Vector2Int& mapPos, const Team team, MapCtrl& mapCtrl, SceneController& ctrl, 
		std::vector<std::shared_ptr<Effect>>& effects);
	~Swordsman();

	void Update(const Input& input)override final;
	void Draw(const Camera& camera)override final;

};

class SwordBC : 
	public BattleCharactor
{
public:
	SwordBC(Charactor& charactor);
	~SwordBC();

	void Update(BattleScene& battleScene);

	void SetDir(const Dir dir)override;
	std::shared_ptr<Effect> CreateAttackEffect(const Vector2Int& effectPos)override final;
};

