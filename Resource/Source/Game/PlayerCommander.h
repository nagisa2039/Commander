#pragma once
#include "Commander.h"
#include <memory>

class PlayerUI;

class PlayerCommander :
	public Commander
{
private:
	std::unique_ptr<PlayerUI> _playerUI;

	void CharactorControl(const Input& input);

public:
	PlayerCommander(std::vector<std::shared_ptr<Charactor>>& charactors, MapCtrl& mapCtrl, const Team ctrlTeam);
	~PlayerCommander();

	void Update(const Input& input)override final;
	void Draw(const Camera& camera)override final;
};

