#pragma once
#include "../UI.h"
#include "Geometry.h"

struct WeaponData;

/// <summary>
/// 武器ウィンドウ
/// </summary>
class WeaponWindow :
    public UI
{
private:
    // 描画先スクリーンハンドル
    int _windowH;
    // 武器ID
    uint8_t _weaponId;
    // 表示座標
    const Vector2Int _drawPos;

    /// <summary>
    /// 固有の描画先への描画
    /// </summary>
    void DrawToWindow();

public:
    /// <param name="weaponId">武器ID</param>
    /// <param name="drawPos">表示座標</param>
	/// <param name="uiDeque">UIの親子関係管理Deque</param>
    WeaponWindow(const uint8_t weaponId, const Vector2Int& drawPos, std::deque<std::shared_ptr<UI>>* uiDeque);
    ~WeaponWindow();

    /// <summary>
    /// 描画
    /// </summary>
    void Draw()override;

    /// <summary>
    /// 表示サイスの取得
    /// </summary>
    Size GetSize()const;

    /// <summary>
    /// 
    /// </summary>
    /// <param name="weaponId"></param>
    void SetWeaponId(const uint8_t weaponId);
};

