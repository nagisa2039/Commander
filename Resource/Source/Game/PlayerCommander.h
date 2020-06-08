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

	// _mapPosが_selectCharの行動範囲内科の確認
	bool CheckMoveRange();
	// _mapPosが_selectCharの攻撃マスかどうかの確認
	bool CheckAttackMass();
	void BattlePredictionUpdate(const Input& input);	// 戦闘予測画面
	void TerrainEffectUpdate(const Input& input);
	void NormalUpdate(const Input& input);
	void SelectUpdate(const Input& input);
	void BattaleUpdate(const Input& input);	// 戦闘中

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

