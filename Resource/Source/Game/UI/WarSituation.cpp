#include "WarSituation.h"
#include "Input.h"
#include "MapCtrl.h"
#include "Application.h"
#include "DxLibUtility.h"
#include "FileSystem.h"
#include <DxLib.h>

using namespace std;

void WarSituation::ScalingUpdate(const Input& input)
{
	_exRate->Update();
	if (_exRate->GetEnd())
	{
		if (_end)
		{
			_uiDeque.pop_front();
		}
		else
		{
			_updater = &WarSituation::NormalUpdate;
		}
		return;
	}
}

void WarSituation::NormalUpdate(const Input& input)
{

	if (input.GetButtonDown(0, "back") || input.GetButtonDown(0, "ok")
	 || input.GetButtonDown(1, "back") || input.GetButtonDown(1, "ok"))
	{
		_end = true;
		_exRate->SetReverse(true);
		_exRate->Reset();
		_updater = &WarSituation::ScalingUpdate;
	}
}

WarSituation::WarSituation(std::deque<std::shared_ptr<UI>>& uiDeque, const MapCtrl& mapCtrl)
	: UI(uiDeque), _mapCtrl(mapCtrl)
{
	_end = false;

	_exRate = make_unique<Track<float>>();
	_exRate->AddKey(0, 0.0f);
	_exRate->AddKey(15, 1.0f);

	_updater = &WarSituation::ScalingUpdate;
	_mapCtrl.CreateWarSituation();
}

WarSituation::~WarSituation()
{
}

void WarSituation::Update(const Input& input)
{
	(this->*_updater)(input);
}

void WarSituation::Draw()
{
	auto wsize = Application::Instance().GetWindowSize().ToVector2();

	int window0H = Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/UI/window0.png");
	Size window0Size;
	GetGraphSize(window0H, window0Size);

	int window1H = Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/UI/window1.png");
	Size window1Size;
	GetGraphSize(window1H, window1Size);

	int window2H = Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/UI/window2.png");
	Size window2Size;
	GetGraphSize(window2H, window2Size);

	auto DrawWinOrLoseWindow = [&](const Vector2Int& center, const char* header, const char* text) 
	{
		DrawGraph(GetDrawPos(center, window0Size, Anker::center), window0H);

		int choplin30 = Application::Instance().GetFileSystem()->GetFontHandle("choplin30");
		Size strSize;
		int lineCnt;
		GetDrawFormatStringSize(&strSize.w, &strSize.h, &lineCnt,text);
		auto str1DrawPos = GetDrawPos(center, strSize, Anker::center);
		DrawFormatString(str1DrawPos.x, str1DrawPos.y, 0x000000, text);

		auto headerCenterPos = center - Vector2Int(0, (window1Size.h + window0Size.h) / 2);
		DrawGraph(GetDrawPos(headerCenterPos, window1Size, Anker::center), window1H);

		int choplin20 = Application::Instance().GetFileSystem()->GetFontHandle("choplin20");
		GetDrawFormatStringSize(&strSize.w, &strSize.h, &lineCnt, header);
		auto str2DrawPos = GetDrawPos(headerCenterPos, strSize, Anker::center);
		DrawFormatString(str2DrawPos.x, str2DrawPos.y, 0xffffff, header);
	};

	auto DrawCharactorNum = [&](const Vector2Int& center, const char* str, const int num)
	{
		DrawGraph(GetDrawPos(center, window1Size, Anker::center), window1H);

		int distance = 100;
		int choplin20 = Application::Instance().GetFileSystem()->GetFontHandle("choplin20");
		DrawStringToHandle(center - Vector2Int(distance / 2, 0), Anker::leftcenter, 0xffffff, choplin20, str);

		char numStr[10];
		sprintf_s(numStr, 10, "%d", num);
		DrawStringToHandle(center + Vector2Int(distance / 2, 0), Anker::rightcenter, 0xffffff, choplin20, numStr);
	};
	
	DrawWinOrLoseWindow((wsize * Vector2(0.25, 0.25)).ToVector2Int(), "Ÿ—˜ðŒ", "“G‚Ì‘S–Å");
	DrawWinOrLoseWindow((wsize * Vector2(0.25, 0.5)).ToVector2Int(), "”s–kðŒ", "–¡•û‚Ì‘S–Å");

	auto charactorNumCenter = (wsize * Vector2(0.25, 0.65)).ToVector2Int();
	auto charactorNum = _mapCtrl.GetCharactorCnt();
	int space = (window0Size.w - window1Size.w * 2) / 2 + window1Size.w / 2;
	DrawCharactorNum(charactorNumCenter - Vector2Int(space, 0), "Ž©ŒR", charactorNum.x);
	DrawCharactorNum(charactorNumCenter + Vector2Int(space, 0), "“GŒR", charactorNum.y);

	{
		Rect canMoveWindwoRect((wsize * Vector2(0.45, 0.8)).ToVector2Int(), window2Size);
		DrawGraph(GetDrawPos(canMoveWindwoRect.center, window2Size, Anker::center), window2H);

		const char* str = "s“®‰Â”\";
		Size strSize;
		int lineCnt;
		int choplin20 = Application::Instance().GetFileSystem()->GetFontHandle("choplin20");
		GetDrawFormatStringSize(&strSize.w, &strSize.h, &lineCnt, str);
		auto str0DrawPos = GetDrawPos(canMoveWindwoRect.center, strSize, Anker::centerdown);
		DrawFormatString(str0DrawPos.x, str0DrawPos.y, 0x000000, str);

		auto canMoveCnt = _mapCtrl.GetCanMoveCnt();
		str = "%d";
		GetDrawFormatStringSize(&strSize.w, &strSize.h, &lineCnt, str, canMoveCnt);
		auto str1DrawPos = GetDrawPos(canMoveWindwoRect.center, strSize, Anker::centerup);
		DrawFormatString(str1DrawPos.x, str1DrawPos.y, 0x000000, str, canMoveCnt);
	}

	_mapCtrl.DrawWarSituatin((wsize * Vector2(0.75, 0.5)).ToVector2Int());
}
