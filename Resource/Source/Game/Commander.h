#pragma once
#include "MapCursor.h"
#include <memory>
#include <vector>
#include "Team.h"

class Charactor;
class Animator;

class Commander :
	public MapCursor
{
protected:
	std::vector<std::shared_ptr<Charactor>>& _charactors;
	Charactor* _selectChar;
	const Team _ctrlTeam;
	int _animCnt;

	int _rigid;

	bool _end;

	bool ChaeckCharactor();
	void StartTerrainEffect();

public:
	Commander(std::vector<std::shared_ptr<Charactor>>& charactors, MapCtrl& mapCtrl, const Team ctrlTeam, Camera& camera);
	~Commander();

	virtual void Update(const Input& input)override = 0;
	virtual void Draw()override = 0;

	virtual void DrawMovableMass();
	virtual bool GetBackMapSelect()const;

	virtual void End();
	bool CheckEnd()const;

	void TurnReset();

	// 選択中のキャラクターのポインターを返す
	Charactor* GetSelectCharactor()const;

	// 地形効果を受けてターンを開始する
	virtual void StartTerrainEffectUpdate() = 0;

	// 地形効果を受けないでターンを開始する	1ターン目に使用
	virtual void StartNormalUpdate() = 0;
};

