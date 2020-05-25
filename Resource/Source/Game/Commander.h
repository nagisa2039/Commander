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

	// �I������Ȃ�true��Ԃ�
	virtual void TerrainEffectUpdate(const Input& input);

	virtual void NormalUpdate(const Input& input);
	virtual void SelectUpdate(const Input& input);
	virtual void BattaleUpdate(const Input& input);	// �퓬��
	void(Commander::* _uniqueUpdater)(const Input& input);

public:
	Commander(std::vector<std::shared_ptr<Charactor>>& charactors, MapCtrl& mapCtrl, const Team ctrlTeam, Camera& camera);
	~Commander();

	virtual void Update(const Input& input)override = 0;
	virtual void Draw()override = 0;

	virtual void DrawMovableMass();

	virtual void End();
	bool CheckEnd()const;

	void TurnReset();

	// �I�𒆂̃L�����N�^�[�̃|�C���^�[��Ԃ�
	Charactor* GetSelectCharactor()const;

	void StartTerrainEffect();
	bool GetTerrainEffectUpdate()const;
};

