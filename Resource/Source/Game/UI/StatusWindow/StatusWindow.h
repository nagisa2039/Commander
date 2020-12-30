#pragma once
#include "../UI.h"
#include "TimeLine.h"

class Charactor;
class WeaponWindow;
class PlayScene;

/// <summary>
/// ステータス確認画面
/// </summary>
class StatusWindow :
	public UI
{
private:
	// 対象キャラクター
	const Charactor& _charactor;

	// 武器詳細表示画面
	std::unique_ptr<WeaponWindow> _weaponWindow;
	// 拡縮アニメーション用トラック
	std::unique_ptr<Track<float>> _animTrack;

	// 描画先スクリーンハンドル
	int _windowH;

	// 側面表示枠画像ハンドル
	Size _sideWindowSize;
	// 中央表示枠画像ハンドル
	Size _centerWindowSize;
	// PlaySceneのポインタ
	PlayScene* _playScene;

	// 更新関数ポインタ
	void(StatusWindow::* _updater)(const Input& input);

	/// <summary>
	/// 通常更新
	/// </summary>
	/// <param name="input"></param>
	void NormalUpdate(const Input& input);

	/// <summary>
	/// 拡縮更新
	/// </summary>
	/// <param name="input"></param>
	void ScaleUpdate(const Input& input);

	/// <summary>
	/// 描画対象
	/// </summary>
	void DrawToWindowScreen();

	/// <summary>
	/// アイコン表示
	/// </summary>
	/// <param name="iconRect">表示矩形</param>
	void DrawIcon(const Rect& iconRect);

	/// <summary>
	/// 名前やレベル等の基本情報表示
	/// </summary>
	/// <param name="levelRect">表示矩形</param>
	void DrawBaseInf(const Rect& levelRect);

	/// <summary>
	/// 戦闘ステータス表示
	/// </summary>
	/// <param name="statusRect1">表示矩形</param>
	void DrawBattleStatus(const Rect& statusRect1);

	/// <summary>
	/// ステータス表示
	/// </summary>
	/// <param name="statusRect2">表示矩形</param>
	void DrawStatus(const Rect& statusRect2);

public:
	/// <param name="uiDeque">UIの親子関係管理Deque</param>
	/// <param name="charactor">対象のキャラクター</param>
	StatusWindow(std::deque<std::shared_ptr<UI>>* uiDeque, const Charactor& charactor);
	~StatusWindow();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void Update(const Input& input)override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;
};

