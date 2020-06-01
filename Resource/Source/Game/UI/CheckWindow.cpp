#include "CheckWindow.h"
#include <Dxlib.h>
#include "Application.h"
#include "FileSystem.h"
#include "Input.h"
#include "DxLibUtility.h"
#include "PlayerCommander.h"

using namespace std;

int CheckWindow::_windowImageH = -1;

CheckWindow::CheckWindow(const std::string& messageStr, std::deque<std::shared_ptr<UI>>& uiDeque, const std::function<void()>& func)
	:_messageStr(messageStr), _func(func), UI(uiDeque)
{
	_exRateTrack = make_unique<Track<float>>();
	_exRateTrack->AddKey(0, 0.0f);
	_exRateTrack->AddKey(15, 1.0f);

	_selectEcRateTrack = make_unique<Track<float>>(true);
	_selectEcRateTrack->AddKey(0, 1.0f);
	_selectEcRateTrack->AddKey(30, 1.2f);
	_selectEcRateTrack->AddKey(60, 1.0f);

	_select = Select::yes;

	_updater = &CheckWindow::ScalingUpdate;

	if (_windowImageH == -1)
	{
		auto wsize = Application::Instance().GetWindowSize();
		_windowImageH = MakeScreen(wsize.w, wsize.h, true);
	}
}

CheckWindow::~CheckWindow()
{
}

void CheckWindow::Update(const Input& input)
{
	(this->*_updater)(input);
}

void CheckWindow::NormalUpdate(const Input& input)
{
	if (input.GetButtonDown(0, "ok") || input.GetButtonDown(1, "ok"))
	{
		_exRateTrack->Reset();
		_exRateTrack->SetReverse(true);
		_updater = &CheckWindow::ScalingUpdate;
		return;
	}

	_selectEcRateTrack->Update();
	if (input.GetButtonDown(0, "left") || input.GetButtonDown(1, "left"))
	{
		if (_select > Select::yes)
		{
			_select = static_cast<Select>(static_cast<int>(_select) - 1);
			_selectEcRateTrack->Reset();
		}
	}

	if (input.GetButtonDown(0, "right") || input.GetButtonDown(1, "right"))
	{
		if (_select < Select::no)
		{
			_select = static_cast<Select>(static_cast<int>(_select) + 1);
			_selectEcRateTrack->Reset();
		}
	}
}

void CheckWindow::ScalingUpdate(const Input& input)
{
	_exRateTrack->Update();
	if (_exRateTrack->GetEnd())
	{
		if (_exRateTrack->GetReverse())
		{
			// •Â‚ß‚é‚Æ‚«
			_delete = true;

			if (_select == Select::yes)
			{
				_func();
			}
			_uiDeque.pop_front();
		}
		else
		{
			// ŠJ‚¯‚é‚Æ‚«
			_updater = &CheckWindow::NormalUpdate;
		}
	}
}

void CheckWindow::Draw()
{
	SetDrawScreen(_windowImageH);
	ClsDrawScreen();

	Size space(50, 10);

	int messageH = Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/UI/checkWindow.png");
	Size messageSize;
	GetGraphSize(messageH, messageSize);

	int selectH = Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/UI/checkWindowSelect.png");
	Size selectSize;
	GetGraphSize(selectH, selectSize);

	Rect totalRect = Rect(Application::Instance().GetWindowSize().ToVector2Int() * 0.5f, Size(messageSize.w, messageSize.h + selectSize.h + space.h));
	auto massageWindowLeftup = Vector2Int(totalRect.Left(), totalRect.Top());
	DrawGraph(massageWindowLeftup, messageH, true);

	int choplin30 = Application::Instance().GetFileSystem()->GetFontHandle("choplin30");

	auto CenterDrawFormatString = [](const Vector2Int& center, const char* str, const int fontHandle)
	{
		Size strSize;
		int lineCnt;
		Vector2Int namePos = Vector2Int();
		GetDrawFormatStringSizeToHandle(&strSize.w, &strSize.h, &lineCnt, fontHandle, str);
		auto drawPos = GetDrawPos(center, strSize, Anker::center);
		DrawFormatStringToHandle(drawPos.x, drawPos.y, 0xffffff, fontHandle, str);
	};

	CenterDrawFormatString(massageWindowLeftup + messageSize * 0.5f, _messageStr.c_str(), choplin30);
	
	auto yseSelectCenter = Vector2Int(totalRect.center.x - (space.w + selectSize.w) / 2, totalRect.Botton() - selectSize.h / 2);
	DrawRotaGraph(yseSelectCenter, _select == Select::yes ? _selectEcRateTrack->GetValue() : 1.0f, 0.0f, selectH, true);
	CenterDrawFormatString(yseSelectCenter, "‚Í‚¢", choplin30);

	auto noSelectCenter = Vector2Int(totalRect.center.x + (space.w + selectSize.w) / 2, totalRect.Botton() - selectSize.h / 2);
	DrawRotaGraph(noSelectCenter, _select == Select::no ? _selectEcRateTrack->GetValue() : 1.0f, 0.0f, selectH, true);
	CenterDrawFormatString(noSelectCenter, "‚¢‚¢‚¦", choplin30);

	SetDrawScreen(DX_SCREEN_BACK);

	DrawRotaGraph(totalRect.center.x, totalRect.center.y, _exRateTrack->GetValue(), 0.0f, _windowImageH, true);
}
