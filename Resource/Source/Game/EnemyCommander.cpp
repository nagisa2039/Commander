#include "EnemyCommander.h"
#include "Charactor.h"
#include "Input.h"
#include "Camera.h"
#include "MapCtrl.h"
#include <algorithm>

using namespace std;

void EnemyCommander::NormalUpdate(const Input& input)
{
	// �d��
	_rigid = max(_rigid - 1, 0);
	if (_rigid > 0)  return;

	// �I�𒆂̃L�������ړ����Ȃ�I���𖳌��ɂ���
	if (_selectChar != nullptr)
	{
		if (_selectChar->GetIsMoveAnim() || _selectChar->GetMoveStandby())
		{
			return;
		}
		// �ړ����I������̂őҋ@�ɂ���
		_selectChar->MoveEnd(false);
		_selectChar = nullptr;
		return;
	}

	// �ړ��\�ȃL������T��
	for (auto charactor : _charactors)
	{
		if (charactor->GetTeam() == _ctrlTeam
			&& charactor->GetCanMove())
		{
			_selectChar = &*charactor;
			_selectChar->SetMoveStandby(15);
			_camera.AddTargetActor(_selectChar);
			_mapPos = _selectChar->GetMapPos();
			_pos = (_mapPos * _mapCtrl.GetChipSize().ToVector2Int()).ToVector2();
			return;
		}
	}

	// �ړ��\�ȷ�ׂ����Ȃ��̂ŏI��
	_end = true;

	return;
}

void EnemyCommander::TerrainEffectUpdate(const Input& input)
{
	bool end = true;
	for (auto& charactor : _charactors)
	{
		if (charactor->GetTeam() != _ctrlTeam)continue;

		bool effectEnd = charactor->GetTerrainEffectEnd();
		end = end && effectEnd;
	}

	if (end)
	{
		_uniqueUpdater = &EnemyCommander::NormalUpdate;
	}
}

EnemyCommander::EnemyCommander(std::vector<std::shared_ptr<Charactor>>& charactors, MapCtrl& mapCtrl, const Team ctrlTeam, Camera& camera):
	Commander(charactors, mapCtrl, ctrlTeam, camera)
{
	_uniqueUpdater = &EnemyCommander::TerrainEffectUpdate;
}

EnemyCommander::~EnemyCommander()
{
}

void EnemyCommander::Update(const Input& input)
{
	(this->*_uniqueUpdater)(input);
}

void EnemyCommander::Draw()
{
}