#pragma once
#include "Actor.h"
#include <memory>
#include "Geometry.h"

class Animator;

/// <summary>
/// エフェクト基底クラス
/// </summary>
class Effect :
	public Actor
{
protected:
	// アニメーション再生クラス
	std::shared_ptr<Animator> _animator;

	// エフェクトサイズ
	Size _size;

	// カメラを考慮して描画するか
	bool _cameraActive;

public:
	/// <param name="pos">座標</param>
	/// <param name="camera">カメラ</param>
	/// <param name="cameraActive">カメラを考慮して描画するか</param>
	Effect(const Vector2Int& pos, Camera& camera, bool cameraActive = false);
	~Effect();

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

