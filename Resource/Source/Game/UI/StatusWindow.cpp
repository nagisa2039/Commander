#include "StatusWindow.h"
#include "Charactor.h"
#include "Application.h"
#include "DxLibUtility.h"
#include <Dxlib.h>
#include "FileSystem.h"
#include "Input.h"

using namespace std;

int StatusWindow::_windowH = -1;

void StatusWindow::NormalUpdate(const Input& input)
{
	if (input.GetButtonDown(0, "status") || input.GetButtonDown(0, "back")
		|| input.GetButtonDown(1, "status") || input.GetButtonDown(1, "back"))
	{
		_updater = &StatusWindow::ScaleUpdate;
		_animTrack->SetReverse(true);
		_animTrack->Reset();
		return;
	}
}

void StatusWindow::ScaleUpdate(const Input& input)
{
	_animTrack->Update();
	if (_animTrack->GetEnd())
	{
		// ï¬Ç∂ÇÈÇ»ÇÁ
		if (_animTrack->GetReverse())
		{
			_uiDeque.pop_front();
			_isOpen = false;
		}
		else
		{
			_updater = &StatusWindow::NormalUpdate;
		}
	}
}

StatusWindow::StatusWindow(std::deque<std::shared_ptr<UI>>& uiDeque, const Charactor& charactor):UI(uiDeque), _charactor (charactor)
{
	_isOpen = true;
	_animTrack = make_unique<Track<float>>();
	_animTrack->AddKey(0, 0.0f);
	_animTrack->AddKey(15, 1.0f);

	_leftWindowSize = Size(250, 250);
	_rightWindowSize = Size(450, 250);

	_updater = &StatusWindow::ScaleUpdate;

	if (_windowH == -1)
	{
		_windowH = MakeScreen(_leftWindowSize.w + _rightWindowSize.w, _leftWindowSize.h + _rightWindowSize.h, true);
	}
}

StatusWindow::~StatusWindow()
{
}

void StatusWindow::Update(const Input& input)
{
	(this->*_updater)(input);
}

void StatusWindow::Draw()
{
	DrawToWindowScreen();

	auto screenCenter = Application::Instance().GetWindowSize() * 0.5f;
	DrawRotaGraph(screenCenter.w, screenCenter.h, _animTrack->GetValue(), 0.0f, _windowH, true);

}

void StatusWindow::DrawToWindowScreen()
{
	int currentScreen = GetDrawScreen();
	SetDrawScreen(_windowH);
	ClsDrawScreen();

	auto wsize = Application::Instance().GetWindowSize();

	Size space(0, 0);

	auto fileSystem = Application::Instance().GetFileSystem();


	Rect iconRect(Vector2Int(0,0) + _leftWindowSize * 0.5f + space, _leftWindowSize);
	Rect statusRect0(iconRect.center + Vector2Int(0, _leftWindowSize.h + space.h * 2), _leftWindowSize);
	Rect statusRect1(Vector2Int(iconRect.Right() + space.w * 2 + _rightWindowSize.w * 0.5f, iconRect.center.y), _rightWindowSize);
	Rect statusRect2(Vector2Int(statusRect1.center.x, statusRect1.Botton() + space.h * 2 + _rightWindowSize.h * 0.5f), _rightWindowSize);

	DrawIcon(iconRect);
	DrawBaseInf(statusRect0);
	DrawBattleStatus(statusRect1);
	DrawStatus(statusRect2);

	SetDrawScreen(currentScreen);
}

void StatusWindow::DrawIcon(Rect& iconRect)
{
	iconRect.DrawGraph(Application::Instance().GetFileSystem().GetImageHandle("Resource/Image/UI/statusWindow1.png"));
	_charactor.DrawCharactorIcon(iconRect);
}

void StatusWindow::DrawBaseInf(Rect& statusRect0)
{
	auto fileSystem = Application::Instance().GetFileSystem();
	statusRect0.DrawGraph(fileSystem.GetImageHandle("Resource/Image/UI/statusWindow1.png"));

	Size contentSize(250, 250 / 4);
	Vector2Int center(statusRect0.Left() + contentSize.w / 2, statusRect0.Top() + contentSize.h / 2);

	Size strSize;
	int line;

	auto choplin40 = fileSystem.GetFontHandle("choplin40edge");

	// ñºëO
	GetDrawFormatStringSizeToHandle(&strSize.w, &strSize.h, &line, choplin40, _charactor.GetName().c_str());
	auto drawPos = GetDrawPos(center, strSize, Anker::center);
	DrawFormatStringToHandle(drawPos.x, drawPos.y, 0xffffff, choplin40, _charactor.GetName().c_str());
	center.y += contentSize.h;

	auto DrawNumContent = [&](const string& leftStr, const string& rightStr)
	{
		Size strSize;
		int line;
		GetDrawFormatStringSizeToHandle(&strSize.w, &strSize.h, &line, choplin40, leftStr.c_str());
		auto drawPos = GetDrawPos(Vector2Int(statusRect0.Left(), center.y), strSize, Anker::leftcenter);
		DrawFormatStringToHandle(drawPos.x + 10, drawPos.y, 0xffffff, choplin40, leftStr.c_str());

		GetDrawFormatStringSizeToHandle(&strSize.w, &strSize.h, &line, choplin40, rightStr.c_str());
		drawPos = GetDrawPos(Vector2Int(statusRect0.Right(), center.y), strSize, Anker::rightcenter);
		DrawFormatStringToHandle(drawPos.x - 10, drawPos.y, 0xffffff, choplin40, rightStr.c_str());
		center.y += contentSize.h;
	};

	auto startStatus = _charactor.GetStartStatus();
	auto currentStatus = _charactor.GetStatus();

	// ÉåÉxÉã
	string rightStr;
	rightStr.resize(256);
	sprintf_s(rightStr.data(), 255, "%d", currentStatus.level);
	DrawNumContent("Lv.", rightStr);

	// ëÃóÕ
	rightStr.resize(256);
	sprintf_s(rightStr.data(), 255, "%d/%d", currentStatus.health, startStatus.health);
	DrawNumContent("HP", rightStr);

	// çsìÆóÕ
	rightStr.resize(256);
	sprintf_s(rightStr.data(), 255, "%d", startStatus.move);
	DrawNumContent("MOV", rightStr);
}

void StatusWindow::DrawBattleStatus(Rect& statusRect1)
{
	auto fileSystem = Application::Instance().GetFileSystem();
	statusRect1.DrawGraph(fileSystem.GetImageHandle("Resource/Image/UI/statusWindow2.png"));

	auto choplin30 = fileSystem.GetFontHandle("choplin30edge");

	// 150, 125
	Rect contentRect = Rect(Vector2Int(0, 0), Size(_rightWindowSize.w / 3, _rightWindowSize.h / 2));
	contentRect.center = Vector2Int(statusRect1.Left(), statusRect1.Top()) + contentRect.size * 0.5f;
	int spaceX = 10;

	auto DrawContentVertical = [&](const string& name, const int num)
	{
		contentRect.DrawGraph(fileSystem.GetImageHandle("Resource/Image/UI/verticalWindow.png"));
		DrawStringToHandle(Vector2Int(contentRect.center.x, contentRect.center.y - contentRect.size.h * 0.25f), Anker::center, 0xffffff, choplin30, name.c_str());
		DrawStringToHandle(Vector2Int(contentRect.center.x, contentRect.center.y + contentRect.size.h * 0.25f), Anker::center, 0xffffff, choplin30, "%d", num);
	};

	auto DrawContentHorizontal = [&](const string& name, const int num)
	{
		contentRect.DrawGraph(fileSystem.GetImageHandle("Resource/Image/UI/horizontalWindow.png"));
		DrawStringToHandle(Vector2Int(contentRect.center.x - contentRect.size.w * 0.25f, contentRect.center.y), Anker::center, 0xffffff, choplin30, name.c_str());
		DrawStringToHandle(Vector2Int(contentRect.center.x + contentRect.size.w * 0.25f, contentRect.center.y), Anker::center, 0xffffff, choplin30, "%d", num);
	};

	auto currentStatus = _charactor.GetStatus();
	// çUåÇ
	DrawContentVertical("çUåÇ", currentStatus.power);
	contentRect.center.x += contentRect.size.w;
	// ñΩíÜ
	DrawContentVertical("ñΩíÜ", currentStatus.GetHitRate());
	contentRect.center.x += contentRect.size.w;
	// ïKéE
	DrawContentVertical("ïKéE", currentStatus.GetCriticalRate());
	contentRect.center.x += contentRect.size.w;

	//150, 62
	contentRect.size = Size(_rightWindowSize.w / 3, _rightWindowSize.h / 4);
	contentRect.center = Vector2Int(statusRect1.Left(), statusRect1.center.y) + contentRect.size * 0.5f;

	DrawContentHorizontal("çUë¨", currentStatus.GetAttackSpeed());
	contentRect.center.x += contentRect.size.w;
	DrawContentHorizontal("ñhå‰", currentStatus.GetDifense());
	contentRect.center.x += contentRect.size.w;
	DrawContentHorizontal("ëœñÇ", currentStatus.GetMagicDifense());

	contentRect.center.x = statusRect1.Left() + contentRect.size.w * 0.5f;
	contentRect.center.y += contentRect.size.h;
	DrawContentHorizontal("âÒî", currentStatus.GetAvoidance());

	// 300, 62
	contentRect.center.x += contentRect.size.w * 1.5;
	contentRect.size.w *= 2;
	auto attackRange = _charactor.GetAttackRange();

	char str[256];
	if (attackRange.min == attackRange.max)
	{
		sprintf_s(str, 256, "%d", attackRange.max);
	}
	else
	{
		sprintf_s(str, 256, "%d Å` %d", attackRange.min, attackRange.max);
	}

	contentRect.DrawGraph(fileSystem.GetImageHandle("Resource/Image/UI/horizontalWindow.png"));
	DrawStringToHandle(Vector2Int(contentRect.center.x - contentRect.size.w * 0.25f, contentRect.center.y), Anker::center, 0xffffff, choplin30, "éÀíˆ");
	DrawStringToHandle(Vector2Int(contentRect.center.x + contentRect.size.w * 0.25f, contentRect.center.y), Anker::center, 0xffffff, choplin30, str);
}

void StatusWindow::DrawStatus(Rect& statusRect2)
{
	auto fileSystem = Application::Instance().GetFileSystem();
	statusRect2.DrawGraph(fileSystem.GetImageHandle("Resource/Image/UI/statusWindow2.png"));

	auto choplin30 = fileSystem.GetFontHandle("choplin30edge");

	Size contentSize(_rightWindowSize.w / 2, _rightWindowSize.h / 3);
	Rect contentRect(Vector2Int(statusRect2.Left(), statusRect2.Top()) + contentSize * 0.5f, contentSize);

	auto DrawNumContent = [&](const Rect& contentRect, const string& leftStr, const int num)
	{
		Rect box(Vector2Int(contentRect.center), Size(0, 15));
		box.size.w = 2 * num;
		box.center.x += box.size.w / 2 - 30;
		box.Draw(0x482464);

		int spaceX = 10;
		DrawStringToHandle(Vector2Int(contentRect.Left() + spaceX, contentRect.center.y), Anker::leftcenter, 0xffffff, choplin30, leftStr.c_str());
		DrawStringToHandle(Vector2Int(contentRect.Right() - spaceX, contentRect.center.y), Anker::rightcenter, 0xffffff, choplin30, "%d", num);
	};

	auto currentStatus = _charactor.GetStatus();
	DrawNumContent(contentRect, "óÕ", currentStatus.power);
	contentRect.center.y += contentRect.Height();

	DrawNumContent(contentRect, "ãZ", currentStatus.skill);
	contentRect.center.y += contentRect.Height();

	DrawNumContent(contentRect, "ë¨Ç≥", currentStatus.speed);
	contentRect.center = Vector2Int(statusRect2.Left(), statusRect2.Top()) + contentSize * 0.5f;
	contentRect.center.x += contentRect.Width();

	DrawNumContent(contentRect, "éÁîı", currentStatus.defense);
	contentRect.center.y += contentRect.Height();

	DrawNumContent(contentRect, "ñÇñh", currentStatus.magic_defense);
}
