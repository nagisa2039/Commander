#pragma once
#include "UI.h"
#include <string>
#include "TimeLine.h"
#include <functional>
#include <memory>

class PlayerCommander;

/// <summary>
/// ポップアップウィンドウ
/// </summary>
class PopupWindow :
	public UI
{
protected:
	// 表示用テキスト
	std::string _messageStr;
	// 拡縮アニメーショントラック
	std::unique_ptr<Track<float>> _exRateTrack;
	// 選択中項目アニメーショントラック
	std::unique_ptr<Track<float>> _selectExRateTrack;
	// 描画先ウィンドウ
	int _messageImageH;

	// 更新関数ポインタ
	void (PopupWindow::* _updater)(const Input& input);
	// 描画関数ポインタ
	void (PopupWindow::* _drawer)();

	/// <summary>
	/// 通常更新
	/// </summary>
	/// <param name="input">入力情報</param>
	virtual void NormalUpdate(const Input& input);

	/// <summary>
	/// 拡縮更新
	/// </summary>
	/// <param name="input">入力情報</param>
	virtual void ScalingUpdate(const Input& input);

	/// <summary>
	/// 閉じ切った瞬間
	/// </summary>
	virtual void Closed();
	
	/// <summary>
	/// 通常描画
	/// </summary>
	virtual void NormalDraw();

	/// <summary>
	/// 拡縮描画
	/// </summary>
	virtual void ScalingDraw();

	/// <summary>
	/// ウィンドウへの描画
	/// </summary>
	void DrawToWindow();

public:
	/// <param name="messageStr">メッセージテキスト</param>
	/// <param name="uiDeque">UIの親子関係管理Deque</param>
	PopupWindow(const std::string& messageStr, std::deque<std::shared_ptr<UI>>* uiDeque);
	~PopupWindow();

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

/// <summary>
/// 選択しありのメッセージウィンドウ
/// </summary>
class CheckWindow :
	public PopupWindow
{
private:
	/// <summary>
	/// 選択項目
	/// </summary>
	enum class Select
	{
		yes,
		no,
		max
	};

	// 選択中の項目
	Select _select;
	// yes選択時実行される関数
	std::function<void()> _func;
	// yes項目画像ハンドル
	int _yesImageH;
	// no項目画像ハンドル
	int _noImageH;

	// yes項目矩形
	Rect _yesSelectRect;
	// no項目矩形
	Rect _noSelectRect;

	/// <summary>
	/// 通常更新
	/// </summary>
	/// <param name="input">入力情報</param>
	void NormalUpdate(const Input& input)override;

	/// <summary>
	/// 閉じる
	/// </summary>
	void StartClose();

	/// <summary>
	/// 項目への描画
	/// </summary>
	void DrawToSelectImage();

	/// <summary>
	/// 閉じ切った瞬間
	/// </summary>
	void Closed()override;

	/// <summary>
	/// 通常描画
	/// </summary>
	void NormalDraw()override;

public:
	/// <param name="messageStr">メッセージテキスト</param>
	/// <param name="uiDeque">UIの親子関係管理Deque</param>
	/// <param name="func">yes選択時に実行される関数</param>
	CheckWindow(const std::string& messageStr, std::deque<std::shared_ptr<UI>>* uiDeque, const std::function<void()>& func);
	~CheckWindow();
};

/// <summary>
/// メッセージ表示用ウィンドウ
/// </summary>
class MessageWindow :
	public PopupWindow
{
private:
	/// <summary>
	/// 通常描画
	/// </summary>
	/// <param name="input">入力情報</param>
	void NormalUpdate(const Input& input)override;

public:
	/// <param name="messageStr">メッセージテキスト</param>
	/// <param name="uiDeque">UIの親子関係管理Deque</param>
	MessageWindow(const std::string& messageStr, std::deque<std::shared_ptr<UI>>* uiDeque);
	~MessageWindow();
};