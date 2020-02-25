#pragma once

enum class Team
{
	player,
	enemy,
	max
};

//Team operator+(const Team lval, const int rval)
//{
//	int t = static_cast<int>(lval) + rval;
//	if (t < 0)
//	{
//		t = static_cast<int>(Team::Enemy);
//	}
//	if (t >= static_cast<int>(Team::Max))
//	{
//		t = 0;
//	}
//	return static_cast<Team>(t);
//}