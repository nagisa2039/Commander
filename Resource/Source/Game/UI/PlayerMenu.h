#pragma once
#include "Menu.h"

class MenuTopInf;

/// <summary>
/// プレイヤー指揮官メニュー
/// </summary>
class PlayerMenu :
	public Menu
{
private:
	/// <summary>
	/// 項目
	/// </summary>
	enum class Content
	{
		// 戦況確認
		situation,
		// 退却
		retreat,
		// ターン終了
		turnEnd,
		max
	};

	// 情報表示クラス
	std::shared_ptr<MenuTopInf> _menuTop;

public:
	/// <param name="uiDeque">UIの親子関係管理Deque</param>
	/// <param name="playerCom"></param>
	/// <param name="mapCtrl">マップ管理クラス</param>
	/// <param name="turnCnt"></param>
	PlayerMenu(std::deque<std::shared_ptr<UI>>* uiDeque, PlayerCommander& playerCom, const MapCtrl& mapCtrl, const unsigned char& turnCnt);
	~PlayerMenu();

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

