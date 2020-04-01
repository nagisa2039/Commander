#include "EnemyCommander.h"
#include "Charactor.h"
#include "Input.h"
#include "Camera.h"
#include "MapCtrl.h"
#include <algorithm>

using namespace std;

EnemyCommander::EnemyCommander(std::vector<std::shared_ptr<Charactor>>& charactors, MapCtrl& mapCtrl, const Team ctrlTeam):
	Commander(charactors, mapCtrl, ctrlTeam)
{
}

EnemyCommander::~EnemyCommander()
{
}

void EnemyCommander::Update(const Input& input)
{
	// �d��
	_rigid = std::max(_rigid - 1, 0);
	if (_rigid > 0)  return;

	// �I�𒆂̃L�������ړ����Ȃ�I���𖳌��ɂ���
	if (_selectChar != nullptr)
	{
		if (_selectChar->GetIsMoveAnim())
		{
			return;
		}
		// �ړ����I������̂őҋ@�ɂ���
		_selectChar->MoveEnd();
		_selectChar = nullptr;
		_rigid = 30;
		return;
	}

	// �ړ��\�ȃL������T��
	for (auto charactor : _charactors)
	{
		if (charactor->GetTeam() == _ctrlTeam 
			&& charactor->GetCanMove())
		{
			_selectChar = &*charactor;
  			_selectChar->SearchAndMove();
			return;
		}
	}

	// �ړ��\�ȷ�ׂ����Ȃ��̂ŏI��
	_end = true;

	return;
}

void EnemyCommander::Draw(const Camera& camera)
{
}