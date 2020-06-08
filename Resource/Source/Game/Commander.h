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

	// �I�𒆂̃L�����N�^�[�̃|�C���^�[��Ԃ�
	Charactor* GetSelectCharactor()const;

	// �n�`���ʂ��󂯂ă^�[�����J�n����
	virtual void StartTerrainEffectUpdate() = 0;

	// �n�`���ʂ��󂯂Ȃ��Ń^�[�����J�n����	1�^�[���ڂɎg�p
	virtual void StartNormalUpdate() = 0;
};

