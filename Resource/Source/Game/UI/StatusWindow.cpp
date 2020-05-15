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
	if (input.GetButtonDown(0, "back")
		|| input.GetButtonDown(0, "status"))
	{
		_uiDeque.pop_front();
		_isOpen = false;
		return;
	}
}

void StatusWindow::Draw()
{
	Size windowSize(800, 600);
	Vector2Int offset = Application::Instance().GetWindowSize().ToVector2Int()*0.5f - (windowSize*0.5f).ToVector2Int();
	Size space(25, 25);

	DrawBox(offset, offset + windowSize, 0x000000);

	auto wsize = Application::Instance().GetWindowSize();
	Size leftWindow(250, 250);
	Size rightWindow(450, 250);

	Rect iconRect(offset + leftWindow.ToVector2Int()*0.5f + space, leftWindow);
	iconRect.Draw(0xffffff);
	_charactor.DrawCharactorIcon(iconRect);

	Rect statusRect0(iconRect.center + Vector2Int(0, leftWindow.h + space.h * 2), leftWindow);
	Rect statusRect1(offset + space + Vector2Int(space.w * 2 + leftWindow.w + rightWindow.w / 2, rightWindow.h / 2), rightWindow);
	Rect statusRect2(statusRect1.center + Vector2Int(0, rightWindow.h + space.h * 2), rightWindow);

	auto choplin40 = Application::Instance().GetFileSystem()->GetFontHandle("choplin40");

	auto startStatus = _charactor.GetStartStatus();
	auto currentStatus = _charactor.GetStatus();

	// Window0
	{
		statusRect0.Draw(0xffffff);

		Size contentSize(250, 250 / 4);
		Vector2Int center(statusRect0.Left() + contentSize.w / 2, statusRect0.Top() + contentSize.h/2);

		Size strSize;
		int line;

		// ñºëO
		GetDrawFormatStringSizeToHandle(&strSize.w, &strSize.h, &line, choplin40, _charactor.GetName().c_str());
		auto drawPos = GetDrawPos(center, strSize, Anker::center);
		DrawFormatStringToHandle(drawPos.x, drawPos.y, 0x000000, choplin40, _charactor.GetName().c_str());
		center.y += contentSize.h;

		auto DrawNumContent = [&](const string& leftStr, const string& rightStr)
		{
			Size strSize;
			int line;
			GetDrawFormatStringSizeToHandle(&strSize.w, &strSize.h, &line, choplin40, leftStr.c_str());
			auto drawPos = GetDrawPos(Vector2Int(statusRect0.Left(), center.y), strSize, Anker::leftcenter);
			DrawFormatStringToHandle(drawPos.x + 10, drawPos.y, 0x000000, choplin40, leftStr.c_str());

			GetDrawFormatStringSizeToHandle(&strSize.w, &strSize.h, &line, choplin40, rightStr.c_str());
			drawPos = GetDrawPos(Vector2Int(statusRect0.Right(), center.y), strSize, Anker::rightcenter);
			DrawFormatStringToHandle(drawPos.x - 10, drawPos.y, 0x000000, choplin40, rightStr.c_str());
			center.y += contentSize.h;
		};

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

	// Window1
	{
		statusRect1.Draw(0xffffff);

		Size contentSize(rightWindow.w / 3, rightWindow.h / 4);
		Rect contentRect(Vector2Int(statusRect1.Left(), statusRect1.Top()) + contentSize*0.5f, contentSize);

		auto DrawContent = [&](const Vector2Int& center, const string& content)
		{
			Size strSize;
			int line;
			GetDrawFormatStringSizeToHandle(&strSize.w, &strSize.h, &line, choplin40, content.c_str());
			auto drawPos = GetDrawPos(center, strSize, Anker::center);
			DrawFormatStringToHandle(drawPos.x, drawPos.y, 0x000000, choplin40, content.c_str());
		};

		auto DrawContentNum = [&](const Vector2Int& center, const int num)
		{
			string str;
			str.resize(256);
			sprintf_s(str.data(), 255, "%d", num);
			DrawContent(center, str);
		};

		auto DrawContent1 = [&](const Vector2Int& center, const string& name, const int num)
		{
			DrawContent(center, name);
			DrawContentNum(contentRect.center + Vector2Int(0, contentSize.h), num);
			contentRect.center.x += contentSize.w;
		};

		// çUåÇ
		DrawContent1(contentRect.center, "çUåÇ", currentStatus.power);
		// ñΩíÜ
		DrawContent1(contentRect.center, "ñΩíÜ", currentStatus.GetHitRate());
		// ïKéE
		DrawContent1(contentRect.center, "ïKéE", currentStatus.GetCriticalRate());

		contentSize = Size(rightWindow.w / 6, rightWindow.h / 4);
		contentRect = Rect(Vector2Int(statusRect1.Left(), statusRect1.center.y) + contentSize * 0.5f, contentSize);

		DrawContent(contentRect.center, "çUë¨");
		contentRect.center.x += contentRect.Width();
		DrawContentNum(contentRect.center, currentStatus.GetAttackSpeed());
		contentRect.center.x += contentRect.Width();
		DrawContent(contentRect.center, "ñhå‰");
		contentRect.center.x += contentRect.Width();
		DrawContentNum(contentRect.center, currentStatus.GetDifense());
		contentRect.center.x += contentRect.Width();
		DrawContent(contentRect.center, "ëœñÇ");
		contentRect.center.x += contentRect.Width();
		DrawContentNum(contentRect.center, currentStatus.GetMagicDifense());
		contentRect.center = Vector2Int(statusRect1.Left(), statusRect1.center.y) + contentSize * 0.5f;
		contentRect.center.y += contentSize.h;
		DrawContent(contentRect.center, "âÒî");
		contentRect.center.x += contentRect.Width();
		DrawContentNum(contentRect.center, currentStatus.GetAvoidance());
		contentRect.center.x += contentRect.Width()*1.5;
		DrawContent(contentRect.center, "éÀíˆ");
		contentRect.center.x += contentRect.Width()*2;
		string str;
		str.resize(256);
		auto attackRange = _charactor.GetAttackRange();
		if (attackRange.min == attackRange.max)
		{
			sprintf_s(str.data(), 255, "%d", attackRange.max);
		}
		else
		{
			sprintf_s(str.data(), 255, "%d Å` %d", attackRange.min, attackRange.max);
		}
		DrawContent(contentRect.center, str);
	}

	// Window2
	{
		statusRect2.Draw(0xffffff);

		Size contentSize(rightWindow.w / 2, rightWindow.h / 3);
		Rect contentRect(Vector2Int(statusRect2.Left(), statusRect2.Top()) + contentSize * 0.5f, contentSize);

		auto DrawNumContent = [&](const Rect& contentRect, const string& leftStr, const string& rightStr)
		{
			Size strSize;
			int line;
			GetDrawFormatStringSizeToHandle(&strSize.w, &strSize.h, &line, choplin40, leftStr.c_str());
			auto drawPos = GetDrawPos(Vector2Int(contentRect.Left(), contentRect.center.y), strSize, Anker::leftcenter);
			DrawFormatStringToHandle(drawPos.x + 10, drawPos.y, 0x000000, choplin40, leftStr.c_str());

			GetDrawFormatStringSizeToHandle(&strSize.w, &strSize.h, &line, choplin40, rightStr.c_str());
			drawPos = GetDrawPos(Vector2Int(contentRect.Right(), contentRect.center.y), strSize, Anker::rightcenter);
			DrawFormatStringToHandle(drawPos.x - 10, drawPos.y, 0x000000, choplin40, rightStr.c_str());
		};

		string num;
		num.resize(256);
		sprintf_s(num.data(), 255, "%d", currentStatus.power);
		DrawNumContent(contentRect, "óÕ", num);
		contentRect.center.y += contentRect.Height();
		sprintf_s(num.data(), 255, "%d", currentStatus.skill);
		DrawNumContent(contentRect, "ãZ", num);
		contentRect.center.y += contentRect.Height();
		sprintf_s(num.data(), 255, "%d", currentStatus.speed);
		DrawNumContent(contentRect, "ë¨Ç≥", num);
		contentRect.center = Vector2Int(statusRect2.Left(), statusRect2.Top()) + contentSize * 0.5f;
		contentRect.center.x += contentRect.Width();
		sprintf_s(num.data(), 255, "%d", currentStatus.defense);
		DrawNumContent(contentRect, "éÁîı", num);
		contentRect.center.y += contentRect.Height();
		sprintf_s(num.data(), 255, "%d", currentStatus.magic_defense);
		DrawNumContent(contentRect, "ñÇñh", num);
	}

}
