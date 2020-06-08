#pragma once
#include "Commander.h"
#include <vector>

class EnemyCommander :
	public Commander
{
private:

	void(EnemyCommander::* _uniqueUpdater)(const Input& input);

	void NormalUpdate(const Input& input);
	void TerrainEffectUpdate(const Input& input);

public:
	EnemyCommander(std::vector<std::shared_ptr<Charactor>>& charactors, MapCtrl& mapCtrl, const Team ctrlTeam, Camera& camera);
	~EnemyCommander();

	void Update(const Input& input)override final;
	void Draw()override final;

	void StartTerrainEffectUpdate()override;
	void StartNormalUpdate()override;
};

