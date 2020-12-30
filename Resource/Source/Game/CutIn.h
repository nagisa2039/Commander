#pragma once
#include <memory>
#include <functional>
#include "CharactorType.h"
#include "Team.h"
#include "TimeLine.h"
#include "Dir.h"

/// <summary>
/// カットイン再生クラス
/// </summary>
class CutIn
{
private:
	// キャラクター種
	const CharactorType _charactorType;
	// チーム
	const Team _team;
	// 拡縮アニメーショントラック
	std::unique_ptr<Track_f> _animTrack;
	// 移動アニメーショントラック
	std::unique_ptr<Track_f> _moveTrack;
	// 背景画像ハンドル
	int _bgH;
	// カットインサイズ
	Size _backImageSize;
	// 画像ハンドル
	int _graphH;
	// 方向 左 : 攻撃を仕掛けた側  右 : 攻撃を仕掛けられた側
	Dir _dir;
	// アニメーションカウント
	int _animCnt;
	// 終了判定
	bool _end;
	// 終了時に呼ぶ関数
	std::function<void()> _endFunc;

public:
	/// <param name="type">キャラクター種</param>
	/// <param name="team">チーム</param>
	/// <param name="dir">方向 左 : 攻撃を仕掛けた側  右 : 攻撃を仕掛けられた側</param>
	/// <param name="endFunc">終了時に呼ぶ関数</param>
	CutIn(const CharactorType type, const Team team, const Dir dir, std::function<void()> endFunc);
	~CutIn();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了判定
	/// </summary>
	bool GetEnd()const;
};

