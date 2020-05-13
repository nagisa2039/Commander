#pragma once
#include "Commander.h"
#include <memory>
#include <deque>

class PlayerUI;
class UI;

class PlayerCommander :
	public Commander
{
private:
	std::unique_ptr<PlayerUI> _playerUI;

	void CharactorControl(const Input& input);

public:
	PlayerCommander(std::vector<std::shared_ptr<Charactor>>& charactors, MapCtrl& mapCtrl, const Team ctrlTeam, Camera& camera);
	~PlayerCommander();

	void Update(const Input& input)override final;
	void Draw()override final;

	void End()override;
};

