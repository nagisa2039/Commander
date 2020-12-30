#pragma once
#include "UI.h"
#include "TimeLine.h"
#include <memory>
#include <list>

class MapCtrl;
class MapCursor;
class PlayerUI;

/// <summary>
/// ステータス情報
/// </summary>
class StatusInf
	: public UI
{
private:
	// マップ管理クラス
	const MapCtrl& _mapCtrl;
	// カーソルのマップ座標
	const Vector2Int& _mapPos;

	// 目標マップ座標
	Vector2Int _targetMapPos;

	// 移動アニメーショントラック
	std::shared_ptr<Track<float>> _moveAnimTrack;

public:
	/// <param name="uiDeque">UIの親子関係管理Deque</param>
	/// <param name="mapCtrl">マップ管理クラス</param>
	/// <param name="mapPos"></param>
	StatusInf(std::deque<std::shared_ptr<UI>>* uiDeque, const MapCtrl& mapCtrl, const Vector2Int& mapPos);
	~StatusInf();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void Update(const Input& input)override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

	/// <summary>
	/// 開く
	/// </summary>
	void Open();

	/// <summary>
	/// 閉じる
	/// </summary>
	void Close();
};

