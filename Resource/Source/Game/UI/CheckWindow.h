#pragma once
#include "UI.h"
#include <string>
#include "TimeLine.h"
#include <functional>
#include <memory>

class PlayerCommander;

class PopupWindow :
	public UI
{
protected:
	std::string _messageStr;
	std::unique_ptr<Track<float>> _exRateTrack;
	std::unique_ptr<Track<float>> _selectExRateTrack;
	static int _messageImageH;

	void DrawToWindow();

	void (PopupWindow::* _updater)(const Input& input);
	virtual void NormalUpdate(const Input& input);
	virtual void ScalingUpdate(const Input& input);

	virtual void Closed();

	void (PopupWindow::* _drawer)();
	virtual void NormalDraw();
	virtual void ScalingDraw();

public:
	PopupWindow(const std::string& messageStr, std::deque<std::shared_ptr<UI>>* uiDeque);
	~PopupWindow();

	void Update(const Input& input)override;
	void Draw()override;
};

class CheckWindow :
	public PopupWindow
{
private:
	enum class Select
	{
		yes,
		no,
		max
	};
	Select _select;
	std::function<void()> _func;
	static int _yesImageH;
	static int _noImageH;

	Rect _yesSelectRect;
	Rect _noSelectRect;

	void NormalUpdate(const Input& input)override;

	void DrawToSelectImage();
	void Closed()override;

	void NormalDraw()override;

public:
	CheckWindow(const std::string& messageStr, std::deque<std::shared_ptr<UI>>* uiDeque, const std::function<void()>& func);
	~CheckWindow();
};

class MessageWindow :
	public PopupWindow
{
private:
	void NormalUpdate(const Input& input)override;

public:
	MessageWindow(const std::string& messageStr, std::deque<std::shared_ptr<UI>>* uiDeque);
	~MessageWindow();
};