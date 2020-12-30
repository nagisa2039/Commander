#pragma once
#include <memory>
#include "TimeLine.h"

/// <summary>
/// シーン遷移時のフェード再生クラス
/// </summary>
class Fade
{
private:
	// アニメーショントラック
	std::unique_ptr<Track<float>> _fadeTrack;
	// フェード時の色
	unsigned int _fadeColor;

public:
	Fade();
	~Fade();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// フェードアウトを開始する
	/// </summary>
	/// <returns></returns>
	bool GetEnd()const;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="color">フェード時の色</param>
	void StartFadeIn(const unsigned color = 0x000000);

	/// <summary>
	/// フェードインを開始する
	/// </summary>
	/// <param name="color">フェード時の色</param>
	void StartFadeOut(const unsigned color = 0x000000);
};

