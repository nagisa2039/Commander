#pragma once
#include "Actor.h"
#include "Team.h"
#include <array>
#include <vector>
#include "TimeLine.h"

/// <summary>
/// ターン切り替え時のアニメーション再生クラス
/// </summary>
class TurnChangeAnim
{
private:
	// 名前
	const char* name;
	// チーム
	Team _currentTeam;
	// チームごとの情報管理配列
	std::array<const char*, static_cast<size_t>(Team::max)> teamInfs;
	// アニメーション終了判定
	bool _animEnd;
	// SEハンドル
	int _seH;

	// 拡大率アニメーショントラック
	std::unique_ptr<Track<float>> _exrateTL;
	// 不透明度アニメーショントラック
	std::unique_ptr<Track<float>> _alphaTL;
	// 回転アニメーショントラック
	std::unique_ptr<Track<float>> _angleTL;

public:
	TurnChangeAnim();
	~TurnChangeAnim();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void Update(const Input& input);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ターン開始時にアニメーションを始める
	/// </summary>
	/// <param name="team"></param>
	void TurnStart(const Team team);

	/// <summary>
	/// 終了判定取得
	/// </summary>
	bool GetAnimEnd()const;

	/// <summary>
	/// 現在のチーム取得
	/// </summary>
	Team GetCurrentTeam()const;
};