#include "StatusWindow.h"
#include "Charactor.h"
#include "Application.h"
#include "DxLibUtility.h"
#include <Dxlib.h>
#include "FileSystem.h"
#include "Input.h"

using namespace std;

StatusWindow::StatusWindow(std::deque<std::shared_ptr<UI>>& uiDeque, const Charactor& charactor):UI(uiDeque), _charactor (charactor)
{
	_isOpen = true;
}

StatusWindow::~StatusWindow()
{
}

void StatusWindow::Update(const Input& input)
{
	if (input.GetButtonDown(0, "status") || input.GetButtonDown(0, "back")
	 || input.GetButtonDown(1, "status") || input.GetButtonDown(1, "back"))
	{
		_uiDeque.pop_front();
		_isOpen = false;
		return;
	}
}

void StatusWindow::Draw()
{
	auto wsize = Application::Instance().GetWindowSize();

	Size leftWindowSize(250, 250);
	Size rightWindowSize(450, 250);
	Size space(0, 0);

	Rect windowRect(wsize.ToVector2Int() * 0.5f, leftWindowSize + rightWindowSize + space*4);

	auto fileSystem = Application::Instance().GetFileSystem();


	Rect iconRect(Vector2Int(windowRect.Left(), windowRect.Top()) + leftWindowSize * 0.5f + space, leftWindowSize);
	Rect statusRect0(iconRect.center + Vector2Int(0, leftWindowSize.h + space.h * 2), leftWindowSize);
	Rect statusRect1(Vector2Int(iconRect.Right() + space.w * 2 + rightWindowSize.w * 0.5f, iconRect.center.y),		rightWindowSize);
	Rect statusRect2(Vector2Int(statusRect1.center.x, statusRect1.Botton() + space.h * 2 + rightWindowSize.h * 0.5f),	rightWindowSize);

	// ƒLƒƒƒ‰ƒNƒ^[ƒAƒCƒRƒ“
	{
		iconRect.DrawGraph(fileSystem->GetImageHandle("Resource/Image/UI/statusWindow1.png"));
		_charactor.DrawCharactorIcon(iconRect);
	}


	auto choplin40 = Application::Instance().GetFileSystem()->GetFontHandle("choplin40edge");
	auto choplin30 = Application::Instance().GetFileSystem()->GetFontHandle("choplin30edge");

	auto startStatus = _charactor.GetStartStatus();
	auto currentStatus = _charactor.GetStatus();

	// Window0
	{
		statusRect0.DrawGraph(fileSystem->GetImageHandle("Resource/Image/UI/statusWindow1.png"));

		Size contentSize(250, 250 / 4);
		Vector2Int center(statusRect0.Left() + contentSize.w / 2, statusRect0.Top() + contentSize.h/2);

		Size strSize;
		int line;

		// –¼‘O
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

		// ƒŒƒxƒ‹
		string rightStr;
		rightStr.resize(256);
		sprintf_s(rightStr.data(), 255, "%d", currentStatus.level);
		DrawNumContent("Lv.", rightStr);

		// ‘Ì—Í
		rightStr.resize(256);
		sprintf_s(rightStr.data(), 255, "%d/%d", currentStatus.health, startStatus.health);
		DrawNumContent("HP", rightStr);

		// s“®—Í
		rightStr.resize(256);
		sprintf_s(rightStr.data(), 255, "%d", startStatus.move);
		DrawNumContent("MOV", rightStr);
	}

	// Window1
	{
		statusRect1.DrawGraph(fileSystem->GetImageHandle("Resource/Image/UI/statusWindow2.png"));

		// 150, 125
		Rect contentRect = Rect(Vector2Int(0,0), Size(rightWindowSize.w / 3, rightWindowSize.h / 2));
		contentRect.center = Vector2Int(statusRect1.Left(), statusRect1.Top()) + contentRect.size * 0.5f;
		int spaceX = 10;

		auto DrawContentVertical = [&](const string& name, const int num)
		{
			contentRect.DrawGraph(fileSystem->GetImageHandle("Resource/Image/UI/verticalWindow.png"));
			DrawStringToHandle(Vector2Int(contentRect.center.x, contentRect.center.y - contentRect.size.h * 0.25f), Anker::center, 0xffffff, choplin30, name.c_str());
			DrawStringToHandle(Vector2Int(contentRect.center.x, contentRect.center.y + contentRect.size.h * 0.25f), Anker::center, 0xffffff, choplin30, "%d", num);
		};

		auto DrawContentHorizontal = [&](const string& name, const int num)
		{
			contentRect.DrawGraph(fileSystem->GetImageHandle("Resource/Image/UI/horizontalWindow.png"));
			DrawStringToHandle(Vector2Int(contentRect.center.x - contentRect.size.w * 0.25f, contentRect.center.y), Anker::center, 0xffffff, choplin30, name.c_str());
			DrawStringToHandle(Vector2Int(contentRect.center.x + contentRect.size.w * 0.25f, contentRect.center.y), Anker::center, 0xffffff, choplin30, "%d", num);
		};

		// UŒ‚
		DrawContentVertical("UŒ‚", currentStatus.power);
		contentRect.center.x += contentRect.size.w;
		// –½’†
		DrawContentVertical("–½’†", currentStatus.GetHitRate());
		contentRect.center.x += contentRect.size.w;
		// •KŽE
		DrawContentVertical("•KŽE", currentStatus.GetCriticalRate());
		contentRect.center.x += contentRect.size.w;

		//150, 62
		contentRect.size = Size(rightWindowSize.w / 3, rightWindowSize.h / 4);
		contentRect.center = Vector2Int(statusRect1.Left(), statusRect1.center.y) + contentRect.size * 0.5f;

		DrawContentHorizontal("U‘¬", currentStatus.GetAttackSpeed());
		contentRect.center.x += contentRect.size.w;
		DrawContentHorizontal("–hŒä", currentStatus.GetDifense());
		contentRect.center.x += contentRect.size.w;
		DrawContentHorizontal("‘Ï–‚", currentStatus.GetMagicDifense());

		contentRect.center.x = statusRect1.Left() + contentRect.size.w*0.5f;
		contentRect.center.y += contentRect.size.h;
		DrawContentHorizontal("‰ñ”ð", currentStatus.GetAvoidance());

		// 300, 62
		contentRect.center.x += contentRect.size.w *1.5;
		contentRect.size.w *= 2;
		auto attackRange = _charactor.GetAttackRange();

		char str[256];
		if (attackRange.min == attackRange.max)
		{
			sprintf_s(str, 256, "%d", attackRange.max);
		}
		else
		{
			sprintf_s(str, 256, "%d ` %d", attackRange.min, attackRange.max);
		}

		contentRect.DrawGraph(fileSystem->GetImageHandle("Resource/Image/UI/horizontalWindow.png"));
		DrawStringToHandle(Vector2Int(contentRect.center.x - contentRect.size.w * 0.25f, contentRect.center.y), Anker::center, 0xffffff, choplin30, "ŽË’ö");
		DrawStringToHandle(Vector2Int(contentRect.center.x + contentRect.size.w * 0.25f, contentRect.center.y), Anker::center, 0xffffff, choplin30, str);
	}

	// Window2
	{
		statusRect2.DrawGraph(fileSystem->GetImageHandle("Resource/Image/UI/statusWindow2.png"));

		Size contentSize(rightWindowSize.w / 2, rightWindowSize.h / 3);
		Rect contentRect(Vector2Int(statusRect2.Left(), statusRect2.Top()) + contentSize * 0.5f, contentSize);

		auto DrawNumContent = [&](const Rect& contentRect, const string& leftStr, const string& rightStr)
		{
			Size strSize;
			int line;
			GetDrawFormatStringSizeToHandle(&strSize.w, &strSize.h, &line, choplin30, leftStr.c_str());
			auto drawPos = GetDrawPos(Vector2Int(contentRect.Left(), contentRect.center.y), strSize, Anker::leftcenter);
			DrawFormatStringToHandle(drawPos.x + 10, drawPos.y, 0xffffff, choplin30, leftStr.c_str());

			GetDrawFormatStringSizeToHandle(&strSize.w, &strSize.h, &line, choplin30, rightStr.c_str());
			drawPos = GetDrawPos(Vector2Int(contentRect.Right(), contentRect.center.y), strSize, Anker::rightcenter);
			DrawFormatStringToHandle(drawPos.x - 10, drawPos.y, 0xffffff, choplin30, rightStr.c_str());
		};

		string num;
		num.resize(256);
		sprintf_s(num.data(), 255, "%d", currentStatus.power);
		DrawNumContent(contentRect, "—Í", num);
		contentRect.center.y += contentRect.Height();
		sprintf_s(num.data(), 255, "%d", currentStatus.skill);
		DrawNumContent(contentRect, "‹Z", num);
		contentRect.center.y += contentRect.Height();
		sprintf_s(num.data(), 255, "%d", currentStatus.speed);
		DrawNumContent(contentRect, "‘¬‚³", num);
		contentRect.center = Vector2Int(statusRect2.Left(), statusRect2.Top()) + contentSize * 0.5f;
		contentRect.center.x += contentRect.Width();
		sprintf_s(num.data(), 255, "%d", currentStatus.defense);
		DrawNumContent(contentRect, "Žç”õ", num);
		contentRect.center.y += contentRect.Height();
		sprintf_s(num.data(), 255, "%d", currentStatus.magic_defense);
		DrawNumContent(contentRect, "–‚–h", num);
	}

}
