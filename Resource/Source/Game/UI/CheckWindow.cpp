#include "CheckWindow.h"
#include <Dxlib.h>
#include "Application.h"
#include "FileSystem.h"
#include "Input.h"
#include "DxLibUtility.h"
#include "PlayerCommander.h"

using namespace std;

PopupWindow::PopupWindow(const std::string& messageStr, std::deque<std::shared_ptr<UI>>* uiDeque)
	:_messageStr(messageStr), UI(uiDeque)
{
	_exRateTrack = make_unique<Track<float>>();
	_exRateTrack->AddKey(0, 0.0f);
	_exRateTrack->AddKey(15, 1.0f);

	_selectExRateTrack = make_unique<Track<float>>(true);
	_selectExRateTrack->AddKey(0, 1.0f);
	_selectExRateTrack->AddKey(30, 1.2f);
	_selectExRateTrack->AddKey(60, 1.0f);

	_updater	= &PopupWindow::ScalingUpdate;
	_drawer		= &PopupWindow::ScalingDraw;

	int messageH = Application::Instance().GetFileSystem().
		GetImageHandle("Resource/Image/UI/checkWindow.png");
	Size messageSize;
	GetGraphSize(messageH, messageSize);
	_messageImageH = Application::Instance().GetFileSystem().MakeScreen("check_window", messageSize, true);

	DrawToWindow();
}

PopupWindow::~PopupWindow()
{
}

void PopupWindow::Update(const Input& input)
{
	(this->*_updater)(input);
}

void PopupWindow::Draw()
{
	(this->*_drawer)();
}

void PopupWindow::NormalUpdate(const Input& input)
{
}

void PopupWindow::ScalingUpdate(const Input& input)
{
	_exRateTrack->Update();
	if (_exRateTrack->GetEnd())
	{
		if (_exRateTrack->GetReverse())
		{
			// •Â‚ß‚é‚Æ‚«
			_delete = true;
			Closed();
			_uiDeque->pop_front();
		}
		else
		{
			// ŠJ‚¯‚é‚Æ‚«
			_updater = &PopupWindow::NormalUpdate;
			_drawer = &PopupWindow::NormalDraw;
		}
	}
}

void PopupWindow::Closed()
{
}

void PopupWindow::NormalDraw()
{
	auto wsize = Application::Instance().GetWindowSize();
	DrawRotaGraph(wsize.w / 2, wsize.h / 2, _exRateTrack->GetValue(), 0.0f, _messageImageH, true);
}

void PopupWindow::ScalingDraw()
{
	PopupWindow::NormalDraw();
}

void PopupWindow::DrawToWindow()
{
	int currentScreen = GetDrawScreen();
	SetDrawScreen(_messageImageH);
	ClsDrawScreen();

	Size space(50, 10);

	auto& fileSystem = Application::Instance().GetFileSystem();
	int messageH = fileSystem.GetImageHandle("Resource/Image/UI/checkWindow.png");
	Size messageSize;
	GetGraphSize(_messageImageH, messageSize);

	int choplin30 = fileSystem.GetFontHandle("choplin30edge");

	DrawGraph(0,0, messageH, true);
	DrawStringToHandle(messageSize.ToVector2Int() * 0.5f, Anker::center, 0xffffff, choplin30, _messageStr.c_str());

	SetDrawScreen(currentScreen);
}

CheckWindow::CheckWindow(const std::string& messageStr, std::deque<std::shared_ptr<UI>>* uiDeque, const std::function<void()>&func)
	:_func(func), PopupWindow(messageStr, uiDeque)
{
	auto wsize = Application::Instance().GetWindowSize();

	Size messageSize;
	GetGraphSize(_messageImageH, messageSize);
	int selectH = Application::Instance().GetFileSystem().GetImageHandle("Resource/Image/UI/checkWindowSelect.png");
	Size selectSize;
	GetGraphSize(selectH, selectSize);

	Size space(50, 10);
	int offsetX = (space.w + selectSize.w);
	int drawY = (wsize.h + messageSize.h + selectSize.h + space.h) / 2;

	_yesSelectRect	= Rect(Vector2Int((wsize.w - offsetX) / 2, drawY), selectSize);
	_noSelectRect	= Rect(Vector2Int((wsize.w + offsetX) / 2, drawY), selectSize);

	_select = Select::yes;

	auto& fileSystem = Application::Instance().GetFileSystem();
	_yesImageH	= fileSystem.MakeScreen("check_window_yes", selectSize, true);
	_noImageH	= fileSystem.MakeScreen("check_window_no", selectSize, true);;

	DrawToSelectImage();
}

CheckWindow::~CheckWindow()
{
}

void CheckWindow::NormalUpdate(const Input& input)
{
	_selectExRateTrack->Update();
	bool click = input.GetButtonDown("ok");
	auto mouseRect = Rect(input.GetMousePos(), Size(1, 1));

	auto decision = [this]() 
	{
		_exRateTrack->Reset();
		_exRateTrack->SetReverse(true);
		_updater = &CheckWindow::ScalingUpdate;
		_drawer = &CheckWindow::ScalingDraw;
	};

	auto back = [this]()
	{	
		_exRateTrack->Reset();
		_exRateTrack->SetReverse(true);
		_updater = &CheckWindow::ScalingUpdate;
		_drawer = &CheckWindow::ScalingDraw;
		_select = Select::no;
	};

	auto select = [this](const Select select)
	{
		if (_select == select)return;
		_select = select;
		_selectExRateTrack->Reset();
	};

	if (input.GetAnyMouseInput())
	{
		if (_yesSelectRect.IsHit(mouseRect))
		{
			select(Select::yes);
			if (click)
			{
				decision();
				return;
			}
		}
		if (_noSelectRect.IsHit(mouseRect))
		{
			select(Select::no);
			if (click)
			{
				back();
				return;
			}
		}
		return;
	}

	if (input.GetButtonDown("ok"))
	{
		decision();
		return;
	}

	if (input.GetButtonDown("back"))
	{
		back();
		return;
	}

	if (input.GetButtonDown("left"))
	{
		if (_select > Select::yes)
		{
			_select = static_cast<Select>(static_cast<int>(_select) - 1);
			_selectExRateTrack->Reset();
		}
	}

	if (input.GetButtonDown("right"))
	{
		if (_select < Select::no)
		{
			_select = static_cast<Select>(static_cast<int>(_select) + 1);
			_selectExRateTrack->Reset();
		}
	}
}

void CheckWindow::DrawToSelectImage()
{
	int currentScreen = GetDrawScreen();

	auto& fileSystem = Application::Instance().GetFileSystem();
	int selectH = fileSystem.GetImageHandle("Resource/Image/UI/checkWindowSelect.png");
	Size selectSize;
	GetGraphSize(selectH, selectSize);
	int choplin30 = fileSystem.GetFontHandle("choplin30edge");

	SetDrawScreen(_yesImageH);
	ClsDrawScreen();
	DrawGraph(0, 0, selectH, true);
	DrawStringToHandle(selectSize.ToVector2Int() * 0.5f, Anker::center, 0xffffff, choplin30, "‚Í‚¢");

	SetDrawScreen(_noImageH);
	ClsDrawScreen();
	DrawGraph(0, 0, selectH, true);
	DrawStringToHandle(selectSize.ToVector2Int() * 0.5f, Anker::center, 0xffffff, choplin30, "‚¢‚¢‚¦");

	SetDrawScreen(currentScreen);
}

void CheckWindow::Closed()
{
	if (_select == Select::yes)
	{
		_func();
	}
}

void CheckWindow::NormalDraw()
{
	auto wsize = Application::Instance().GetWindowSize();
	PopupWindow::NormalDraw();

	DrawRotaGraph(_yesSelectRect.center, _select == Select::yes ? _selectExRateTrack->GetValue() : 1.0f, 0.0f, _yesImageH, true);
	DrawRotaGraph(_noSelectRect.center, _select == Select::no ? _selectExRateTrack->GetValue() : 1.0f, 0.0f, _noImageH, true);
}

void MessageWindow::NormalUpdate(const Input& input)
{
	if (input.GetButtonDown("ok"))
	{
		_exRateTrack->Reset();
		_exRateTrack->SetReverse(true);
		_updater = &MessageWindow::ScalingUpdate;
		_drawer = &MessageWindow::ScalingDraw;
		return;
	}

	if (input.GetButtonDown("back"))
	{
		_exRateTrack->Reset();
		_exRateTrack->SetReverse(true);
		_updater = &MessageWindow::ScalingUpdate;
		_drawer = &MessageWindow::ScalingDraw;
		return;
	}
}

MessageWindow::MessageWindow(const std::string& messageStr, std::deque<std::shared_ptr<UI>>* uiDeque)
	:PopupWindow(messageStr, uiDeque)
{
}

MessageWindow::~MessageWindow()
{
}