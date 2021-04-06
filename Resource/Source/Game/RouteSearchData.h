#pragma once
#include <vector>
#include <list>
#include "../Utility/Geometry.h"
#include "../Utility/Dir.h"
#include "Team.h"

namespace SearchData
{
	/// <summary>
	/// �}�X���Ƃ̌�������
	/// </summary>
	struct ResultPos
	{
		// �U���}�X��
		bool attack;
		// �}�b�v���W
		Vector2Int mapPos;
		// �X�^�[�g���炽�ǂ������̐e
		ResultPos* prev;
		// �X�^�[�g���炽�ǂ������̈ړ�����
		Dir dir;
		// �����ړ���
		int moveCnt;
	};

	// �}�b�v�`�b�v���
	struct MapChipData
	{
		// �ړ��R�X�g
		int moveCost;
		// �ǂ̃`�[���̃L�����N�^�[�����邩
		Team team;
		// �_���[�W���󂯂Ă��邩
		bool isHurt;
	};

	// �����p�}�b�v���
	using SearchMapData = std::vector<std::vector<MapChipData>>;
	// �P�}�X���̌�������
	using ResultList = std::list<ResultPos>;
	// �}�b�v�f�[�^���̌������ʏ��
	using RouteSearchData = std::vector<std::vector<ResultList>>;
}