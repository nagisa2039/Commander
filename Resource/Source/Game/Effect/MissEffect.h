#pragma once
#include "TimeLine.h"
#include "FlyText.h"
#include <memory>

/// <summary>
/// 跳ねる失敗エフェクト
/// </summary>
class PopupMissEffect :
	public PopupText
{
public:
	/// <param name="pos">座標</param>
	/// <param name="camera">カメラ</param>
	/// <param name="cameraActive">カメラを考慮して描画するか</param>
	/// <param name="critical">クリティカルフラグ</param>
	PopupMissEffect(const Vector2Int& pos,
		Camera& camera, bool cameraActive, bool critical = false);
	~PopupMissEffect() = default;
};