#pragma once
#include "Effect.h"
#include <memory>
#include "TimeLine.h"

/// <summary>
/// カーソルエフェクト
/// </summary>
class CorsorTarget :
	public Effect
{
private:
	// 拡縮アニメーショントラック
	std::unique_ptr<Track<float>> _exRateTrack;
	// マップチップサイズ
	const Size& _chipSize;

public:
	/// <param name="centerPos">中心座標</param>
	/// <param name="camera">カメラ</param>
	/// <param name="cameraActive">カメラを考慮して描画するか</param>
	/// <param name="chipSize">1マスのサイズ</param>
	CorsorTarget(const Vector2Int& centerPos, Camera& camera, bool cameraActive, const Size& chipSize);
	~CorsorTarget();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力情報</param>
	virtual void Update(const Input& input) override;

	/// <summary>
	///　描画
	/// </summary>
	virtual void Draw() override;

	/// <summary>
	/// もう一度再生する際に
	/// 初期化する
	/// </summary>
	virtual void Reset();
};