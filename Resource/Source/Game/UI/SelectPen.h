#pragma once
#include "UI.h"
#include "TimeLine.h"

/// <summary>
/// メニュー項目選択用矢印
/// </summary>
class SelectPen :
    public UI
{
private:
	// 表示座標
    Vector2Int _pos;
	// アニメーショントラック
    std::unique_ptr<Track<float>> _penAnimTrack;

public:
	/// <param name="uiDeque">UIの親子関係管理Deque</param>
    SelectPen(std::deque<std::shared_ptr<UI>>* uiDeque);
    ~SelectPen();

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
	/// 表示座標設定
	/// </summary>
	/// <param name="pos">表示座標</param>
	void SetPos(const Vector2Int& pos);
};

