#pragma once
#include "Geometry.h"
#include <memory>
#include <deque>
#include "TimeLine.h"

class PlayerCommander;
class MapCtrl;
class Input;
class UI;
class PlayerMenu;
class MoveMenu;
class StatusInf;
class TerrainInf;
class StatusWindow;

/// <summary>
/// プレイヤーのUI管理クラス
/// </summary>
class PlayerUI
{
private:
	// マップ管理クラス
	const MapCtrl& _mapCtrl;
	// プレイヤー指揮官クラス
	PlayerCommander& _playerCommander;

	// プレイヤーメニュー用親子関係Deque
	std::deque<std::shared_ptr<UI>> _playerMenuDeque;
	// プレイヤーメニュー
	std::shared_ptr<PlayerMenu> _playerMenu;

	// 移動メニュー用親子関係Deque
	std::deque<std::shared_ptr<UI>> _moveMenuDeque;
	// 移動メニュー用親子関係Deque
	std::shared_ptr<MoveMenu> _moveMenu;

	// 地形情報
	std::shared_ptr<TerrainInf> _terrainInf;
	// ステータス情報
	std::shared_ptr<StatusInf> _statusInf;
	// ステータスウィンドウ
	std::shared_ptr<UI> _statusWindow;
	// 戦闘予測
	std::shared_ptr<UI> _battlePre;

public:
	/// <param name="playerCommander"></param>
	/// <param name="mapCtrl">マップ管理クラス</param>
	/// <param name="turnCnt"></param>
	PlayerUI(PlayerCommander& playerCommander, const MapCtrl& mapCtrl, const unsigned char& turnCnt);
	~PlayerUI();

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
	/// 何かしらのUIを開いているか
	/// </summary>
	bool GetUIIsOpen()const;

	/// <summary>
	/// 指揮官メニューを開く
	/// </summary>
	/// <param name="animation">アニメーションを行うか</param>
	void OpenPlayerMenu(bool animation = true);

	/// <summary>
	/// 指揮官メニューを閉じる
	/// </summary>
	/// <param name="animation">アニメーションを行うか</param>
	void ClosePlayerMenu(bool animation = true);

	/// <summary>
	/// 移動メニューを取得
	/// </summary>
	std::shared_ptr<MoveMenu> GetMoveMenu();

	/// <summary>
	/// 戦闘予測画面を追加する
	/// </summary>
	void OpenBattlePrediction();

	/// <summary>
	/// 戦闘予測画面を閉じる
	/// </summary>
	void ClearBattlePrediction();
};

