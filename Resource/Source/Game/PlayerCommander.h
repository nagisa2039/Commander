#pragma once
#include "Commander.h"
#include <memory>
#include <deque>
#include <list>

class PlayerUI;
class UI;

class PlayerCommander :
	public Commander
{
private:
	std::unique_ptr<PlayerUI> _playerUI;
	bool _backMapSelect;

	void(PlayerCommander::* _uniqueUpdater)(const Input& input);

	// _mapPos��_selectChar�̍s���͈͓��Ȃ̊m�F
	bool CheckMoveRange();
	// _mapPos��_selectChar�̍U���}�X���ǂ����̊m�F
	bool CheckAttackMass();
	void BattlePredictionUpdate(const Input& input);	// �퓬�\�����
	void TerrainEffectUpdate(const Input& input);
	void NormalUpdate(const Input& input);
	void SelectUpdate(const Input& input);
	void BattaleUpdate(const Input& input);	// �퓬��

	void CursorMoveMoment()override;

public:
	PlayerCommander(std::vector<std::shared_ptr<Charactor>>& charactors, MapCtrl& mapCtrl, const Team ctrlTeam, Camera& camera);
	~PlayerCommander();

	void Update(const Input& input)override final;
	void Draw()override final;

	void DrawMovableMass()override;

	void StartTerrainEffectUpdate()override;
	void StartNormalUpdate()override;

	void End()override;

	void SelectCharactor(Charactor* charactor, const bool nextTarget);

	bool AttackPrePos(const Vector2Int& mapPos);
	void BattleStart();
	void BackBattalePrediction();

	bool MoveCancel();

	virtual bool GetBackMapSelect()const;
	void SetBackMapSelect(const bool value);
};

