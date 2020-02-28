#include "Team.h"

unsigned int GetTeamColor(const Team team)
{
	switch (team)
	{
	case Team::player:
		return 0x7695bc;
	case Team::enemy:
		return 0xec5f1d;
	default:
		return 0xffffff;
	};
}
