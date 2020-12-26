#pragma once
#include "Team.h"
#include "Geometry.h"

using CharactorType = unsigned int;

/// <summary>
/// �L�����N�^�[���
/// </summary>
struct CharactorChipInf
{
	// �}�b�v���W
	Vector2Int mapPos;
	// ���x��
	unsigned int level;
	// �O���[�v�ԍ�
	unsigned int groupNum;
	// ���
	CharactorType type;
	// �`�[��
	Team team;
	//�@�A�N�e�B�u���
	bool active = false;
	// ����
	uint8_t weaponId;
	// 8�o�C�g��؂�ɂ��邽�߂̋l�ߕ�
	uint8_t stuffing[7];	
};
