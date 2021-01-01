#pragma once

/// <summary>
/// チーム
/// </summary>
enum class Team
{
	// プレイヤー
	player,
	// 敵
	enemy,
	max
};

/// <summary>
/// チームごとの色取得
/// </summary>
/// <param name="team">チーム</param>
unsigned int GetTeamColor(const Team team);

/// <summary>
/// チームごとの戦闘時の色取得
/// </summary>
/// <param name="team">チーム</param>
unsigned int GetTeamColorBattle(const Team team);