#pragma once

/// <summary>
/// �`�[��
/// </summary>
enum class Team
{
	// �v���C���[
	player,
	// �G
	enemy,
	max
};

/// <summary>
/// �`�[�����Ƃ̐F�擾
/// </summary>
/// <param name="team">�`�[��</param>
unsigned int GetTeamColor(const Team team);

/// <summary>
/// �`�[�����Ƃ̐퓬���̐F�擾
/// </summary>
/// <param name="team">�`�[��</param>
unsigned int GetTeamColorBattle(const Team team);