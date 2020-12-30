#pragma once
#include "UI.h"
#include "TimeLine.h"
#include <memory>

class PlayScene;
class MapCtrl;

/// <summary>
/// 戦況確認画面
/// </summary>
class WarSituation :
	public UI
{
private:
	// マップ管理クラス
	const MapCtrl& _mapCtrl;
	// 拡縮アニメーショントラック
	std::unique_ptr<Track<float>> _exRate;
	// 更新関数ポインタ
	void(WarSituation::* _updater)(const Input& input);
	// 終了判定
	bool _end;
	// PlayScene
	PlayScene* _playScene;

	/// <summary>
	/// 拡縮更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void ScalingUpdate(const Input& input);

	/// <summary>
	/// 通常更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void NormalUpdate(const Input& input);

public:
	/// <param name="uiDeque">UIの親子関係管理Deque</param>
	/// <param name="mapCtrl">マップ管理クラス</param>
	WarSituation(std::deque<std::shared_ptr<UI>>* uiDeque, const MapCtrl& mapCtrl);
	~WarSituation();

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