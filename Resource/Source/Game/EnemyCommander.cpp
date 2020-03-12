#include "EnemyCommander.h"
#include "Charactor.h"
#include "Input.h"
#include "Camera.h"
#include "MapCtrl.h"

EnemyCommander::EnemyCommander(std::vector<std::shared_ptr<Charactor>>& charactors, MapCtrl& mapCtrl, const Team ctrlTeam):
	Commander(charactors, mapCtrl, ctrlTeam)
{
}

EnemyCommander::~EnemyCommander()
{
}

void EnemyCommander::Update(const Input& input)
{
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
	}

	// �ړ��\�ȃL������T��
	for (auto charactor : _charactors)
	{
		if (charactor->GetTeam() == _ctrlTeam 
			&& charactor->GetCanMove())
		{
			_selectChar = &*charactor;
  			_selectChar->SearchAndMove();
			break;
		}
	}

	// �ړ��\�ȷ�ׂ����Ȃ��̂ŏI��
	if (_selectChar == nullptr)
	{
		return;
	}


	return;
}

void EnemyCommander::Draw(const Camera& camera)
{
}