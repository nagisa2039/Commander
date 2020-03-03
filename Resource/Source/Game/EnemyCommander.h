#pragma once
#include "Commander.h"
#include <vector>

class EnemyCommander :
	public Commander
{
private:

public:
	EnemyCommander(std::vector<std::shared_ptr<Charactor>>& charactors, MapCtrl& mapCtrl, const Team ctrlTeam);
	~EnemyCommander();

	void Update(const Input& input)override final;
	void Draw(const Camera& camera)override final;
};

