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

	void(PlayerCommander::* _uniqueUpdater)(const Input& input);

	void NormalUpdate(const Input& input);
	void SelectUpdate(const Input& input);
	void BattlePredictionUpdate(const Input& input);	// �퓬�\�����
	void BattaleUpdate(const Input& input);	// �퓬��

	void SelectCharactor(Charactor* charactor);

	void CursorMoveMoment()override;

public:
	PlayerCommander(std::vector<std::shared_ptr<Charactor>>& charactors, MapCtrl& mapCtrl, const Team ctrlTeam, Camera& camera);
	~PlayerCommander();

	void Update(const Input& input)override final;
	void Draw()override final;

	void DrawMovableMass()override;

	void End()override;
};

