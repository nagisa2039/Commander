#pragma once
#include <memory>
#include "Effect.h"
#include "DxLibUtility.h"
#include "TimeLine.h"

/// <summary>
/// テキストエフェクト
/// </summary>
class TextEffect :
	public Effect
{
protected:
	// 描画する文字列
	std::string _str;
	// フォントハンドル
	int _fontHandle;
	// 色
	unsigned int _color;

public:
	/// <param name="str">描画する文字列</param>
	/// <param name="pos">座標</param>
	/// <param name="camera">カメラ</param>
	/// <param name="cameraActive">カメラを考慮して描画するか</param>
	TextEffect(const std::string& str, const Vector2Int& pos, 
		Camera& camera, const bool cameraActive);
	virtual ~TextEffect() = default;

	/// <summary>
	/// フォントハンドルの設定
	/// </summary>
	/// <param name="handle">フォントハンドル</param>
	void SetFontHandle(const int handle);

	/// <summary>
	/// 色の設定
	/// </summary>
	/// <param name="color">色</param>
	void SetColor(const unsigned int color);
};

/// <summary>
/// 上昇していくテキストエフェクト
/// </summary>
class FlyText :
	public TextEffect
{
private:
	// 描画する基準
	Anker _anker;
	// 移動力
	Vector2 _move;
	// クリティカルフラグ
	bool _critical;
	// 移動アニメーショントラック
	std::unique_ptr<Track<float>> _moveTrack;
	// 透過アニメーショントラック
	std::unique_ptr<Track<float>> _alphaTrack;

public:
	/// <param name="str">描画する文字列</param>
	/// <param name="pos">座標</param>
	/// <param name="lifeCnt">生成フレーム数</param>
	/// <param name="camera">カメラ</param>
	/// <param name="cameraActive">カメラを考慮して描画するか</param>
	/// <param name="critical">クリティカルフラグ</param>
	FlyText(const std::string& str, const Vector2Int& pos, const int lifeCnt, 
		Camera& camera, const bool cameraActive, const bool critical = false);
	~FlyText();

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
	///  描画する基準を設定
	/// </summary>
	/// <param name="anker">基準</param>
	void SetAnker(const Anker anker);

	/// <summary>
	/// 移動力を設定
	/// </summary>
	/// <param name="move"></param>
	void SetMove(const Vector2& move);
};

/// <summary>
/// 跳ねるテキストエフェクト
/// </summary>
class PopupText :
	public TextEffect
{
private:
	// 文字情報
	struct charInf
	{
		// 文字
		char c;
		// 開始座標
		Vector2Int startPos;
		// 現在のY座標
		float currentY;
		// 速度
		float velocityY;
		// 終了判定
		bool end;
	};

	// 文字後の情報管理配列
	std::vector<charInf> _charInfs;
	// 移動アニメーショントラック
	std::unique_ptr<Track<float>> _moveTrack;
	// 透過アニメーショントラック
	std::unique_ptr<Track<float>> _alphaTrack;
	// 跳ねさせる文字番号
	int _currentIdx;

	// 描画する基準
	Anker _anker;
	// クリティカルフラグ
	bool _critical;

public:
	/// <param name="str">描画する文字列</param>
	/// <param name="pos">座標</param>
	/// <param name="camera">カメラ</param>
	/// <param name="cameraActive">カメラを考慮して描画するか</param>
	/// <param name="critical">クリティカルフラグ</param>
	PopupText(const std::string& str, const Vector2Int& pos, 
		Camera& camera, bool cameraActive, bool critical = false);
	virtual ~PopupText() = default;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="input">入力情報</param>
	virtual void Update(const Input& input) override;

	/// <summary>
	///　描画
	/// </summary>
	virtual void Draw() override;
};