#pragma once
#include "CharactorType.h"

using Map_Chip = unsigned int;

namespace
{
	// �}�b�v�`�b�v��0�Ԃ��Ȃ����̂Ƃ��Ĉ���
	constexpr unsigned int MAP_CHIP_NONE = 0;
}

/// <summary>
/// �}�b�v�f�[�^
/// </summary>
struct MapData
{
	// �}�b�v�`�b�v���
	Map_Chip mapChip = 0;
	// �L�����N�^�[�`�b�v���
	CharactorChipInf charactorChip;
};