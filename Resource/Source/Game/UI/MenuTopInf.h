#pragma once
#include "UI.h"
#include "TimeLine.h"

class MapCtrl;

/// <summary>
/// PlaySceneのメニュー選択時に表示される情報表示
/// </summary>
class MenuTopInf :
    public UI
{
private:
    // マップ管理クラス
    const MapCtrl& _mapCtrl;
    // ターン数
    const unsigned char& _turnCnt;

    // 移動アニメーショントラック
    std::unique_ptr<Track<Vector2Int>> _moveAnimTrack;

public:
	/// <param name="mapCtrl">マップ管理クラス</param>
    /// <param name="turnCnt">ターン数</param>
	/// <param name="uiDeque">UIの親子関係管理Deque</param>
    MenuTopInf(const MapCtrl& mapCtrl, const unsigned char& turnCnt, std::deque<std::shared_ptr<UI>>* uiDeque);
    ~MenuTopInf();

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

