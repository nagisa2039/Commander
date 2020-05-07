#include "CheckWindow.h"
#include <Dxlib.h>
#include "Application.h"
#include "FileSystem.h"
#include "Input.h"
#include "DxLibUtility.h"
#include "PlayerCommander.h"

using namespace std;

int CheckWindow::_windowImageH = -1;
Size CheckWindow::_windowSize = Size(0, 0);

CheckWindow::CheckWindow(const std::string& messageStr, std::deque<std::shared_ptr<UI>>& uiDeque, PlayerCommander& playerCom)
	:_messageStr(messageStr), _playerCommander(playerCom), UI(uiDeque)
{
	_exRateTrack = make_unique<Track<float>>();
	_exRateTrack->AddKey(0, 0.0f);
	_exRateTrack->AddKey(15, 1.0f);

	_updater = &CheckWindow::ScalingUpdate;

	if (_windowImageH == -1)
	{
		auto graphH = Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/UI/checkWindow.png");
		GetGraphSize(graphH, _windowSize);
		_windowImageH = MakeScreen(_windowSize.w, _windowSize.h, true);
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
	if (input.GetButtonDown(0, "space"))
	{
		_exRateTrack->Reset();
		_exRateTrack->SetReverse(true);
		_updater = &CheckWindow::ScalingUpdate;
	}
}

void CheckWindow::ScalingUpdate(const Input& input)
{
	_exRateTrack->Update();
	if (_exRateTrack->GetEnd())
	{
		if (_exRateTrack->GetReverse())
		{
			_delete = true;
			_playerCommander.End();
			_uiDeque.pop_front();
		}
		else
		{
			_updater = &CheckWindow::NormalUpdate;
		}
	}
}

void CheckWindow::Draw()
{
	SetDrawScreen(_windowImageH);

	DrawGraph(Vector2Int(0,0), Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/UI/checkWindow.png"), true);

	int choplin30 = Application::Instance().GetFileSystem()->GetFontHandle("choplin30");
	Size strSize;
	int lineCnt;
	Vector2Int namePos = Vector2Int();
	GetDrawFormatStringSizeToHandle(&strSize.w, &strSize.h, &lineCnt, choplin30, _messageStr.c_str());

	auto drawPos = GetDrawPos(_windowSize.ToVector2Int() * 0.5f, strSize, Anker::center);
	DrawFormatStringToHandle(drawPos.x, drawPos.y, 0xffffff, choplin30, _messageStr.c_str());

	SetDrawScreen(DX_SCREEN_BACK);

	auto center = Application::Instance().GetWindowSize().ToVector2Int() * 0.5f;
	DrawRotaGraph(center.x, center.y, _exRateTrack->GetValue(), 0.0f, _windowImageH, true);
}
