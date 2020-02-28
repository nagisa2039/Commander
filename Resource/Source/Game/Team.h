#pragma once

enum class Team
{
	player,
	enemy,
	max
};

unsigned int GetTeamColor(const Team team);