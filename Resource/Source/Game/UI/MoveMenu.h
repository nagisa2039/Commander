#pragma once
#include "Menu.h"
#include <list>

/// <summary>
/// キャラクター
/// </summary>
class MoveMenu :
	public Menu
{
private:
	/// <summary>
	/// 項目種
	/// </summary>
	enum class Content
	{
		// 戦闘
		battle,
		// 待機
		wait,
		// 閉じる
		close,
		max
	};

	// 先頭項目を選んだか
	bool _isBattle;
	// 攻撃可能マスリスト
	std::list<Vector2Int> _attackPosList;
	// 選択中の戦闘可能マス
	std::list<Vector2Int>::iterator _attackPosListItr;

	/// <summary>
	/// 決定
	/// </summary>
	void Decision()override;

	/// <summary>
	/// 戻る
	/// </summary>
	void Back()override;

	/// <summary>
	/// 開いている時の更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void OpenUpdate(const Input& input)override;

public:
	/// <param name="uiDeque">UIの親子関係管理Deque</param>
	/// <param name="playerCom">プレイヤー指揮官</param>
	/// <param name="mapCtrl">マップ管理クラス</param>
	MoveMenu(std::deque<std::shared_ptr<UI>>* uiDeque, PlayerCommander& playerCom, const MapCtrl& mapCtrl);
	~MoveMenu();

	/// <summary>
	/// 項目情報を設定
	/// </summary>
	/// <param name="attackPosList">攻撃対象座標</param>
	void SetContent(const std::list<Vector2Int>& attackPosList);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;
};

