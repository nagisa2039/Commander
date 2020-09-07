#include "WarSituation.h"
#include "Input.h"
#include "MapCtrl.h"
#include "Application.h"
#include "DxLibUtility.h"
#include "FileSystem.h"
#include <DxLib.h>
#include "SceneController.h"
#include "PlayScene.h"
#include "SoundLoader.h"

using namespace std;

void WarSituation::ScalingUpdate(const Input& input)
{
	_exRate->Update();
	if (_exRate->GetEnd())
	{
		if (_end)
		{
			assert(_uiDeque != nullptr);
			_uiDeque->pop_front();
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
	if (input.GetButtonDown("back") || input.GetButtonDown("ok"))
	{
		SoundL.PlaySE("Resource/Sound/SE/select06.mp3");
		_end = true;
		_exRate->SetReverse(true);
		_exRate->Reset();
		_updater = &WarSituation::ScalingUpdate;
	}
}

WarSituation::WarSituation(std::deque<std::shared_ptr<UI>>* uiDeque, const MapCtrl& mapCtrl)
	: UI(uiDeque), _mapCtrl(mapCtrl)
{
	_end = false;

	_exRate = make_unique<Track<float>>();
	_exRate->AddKey(0, 0.0f);
	_exRate->AddKey(15, 1.0f);

	_updater = &WarSituation::ScalingUpdate;
	_mapCtrl.CreateWarSituation();

	_playScene = &dynamic_cast<PlayScene&>(Application::Instance().GetSceneController().GetCurrentScene());
	if (_playScene)
	{
		_playScene->SetFilter(PlayScene::FilterType::gauss);
	}

	SoundL.PlaySE("Resource/Sound/SE/select05.mp3");
}

WarSituation::~WarSituation()
{
	if (_playScene)
	{
		_playScene->SetFilter(PlayScene::FilterType::none);
	}
}

void WarSituation::Update(const Input& input)
{
	(this->*_updater)(input);
}

void WarSituation::Draw()
{
	auto wsize = Application::Instance().GetWindowSize().ToVector2();

	auto& fileSystem = FileSystem::Instance();
	int window0H = fileSystem.GetImageHandle("Resource/Image/UI/window0.png");
	Size window0Size;
	GetGraphSize(window0H, window0Size);

	int window1H = fileSystem.GetImageHandle("Resource/Image/UI/window1.png");
	Size window1Size;
	GetGraphSize(window1H, window1Size);

	int window2H = fileSystem.GetImageHandle("Resource/Image/UI/window2.png");
	Size window2Size;
	GetGraphSize(window2H, window2Size);

	auto DrawWinOrLoseWindow = [&](const Vector2Int& center, const char* header, const char* text) 
	{
		DrawGraph(GetDrawPos(center, window0Size, Anker::center), window0H);

		int choplin20 = fileSystem.GetFontHandle("choplin20");
		DrawStringToHandle(center, Anker::center, 0x000000, choplin20, text);

		auto headerCenterPos = center - Vector2Int(0, (window1Size.h + window0Size.h) / 2);
		DrawGraph(GetDrawPos(headerCenterPos, window1Size, Anker::center), window1H);

		DrawStringToHandle(headerCenterPos, Anker::center, 0xffffff, choplin20, header);
	};

	auto DrawCharactorNum = [&](const Vector2Int& center, const char* str, const int num)
	{
		DrawGraph(GetDrawPos(center, window1Size, Anker::center), window1H);

		int distance = 100;
		int choplin20 = fileSystem.GetFontHandle("choplin20");
		DrawStringToHandle(center - Vector2Int(distance / 2, 0), Anker::leftcenter, 0xffffff, choplin20, str);

		char numStr[10];
		sprintf_s(numStr, 10, "%d", num);
		DrawStringToHandle(center + Vector2Int(distance / 2, 0), Anker::rightcenter, 0xffffff, choplin20, numStr);
	};
	
	DrawWinOrLoseWindow((wsize * Vector2(0.25, 0.25)).ToVector2Int(), "èüóòèåè", "ìGÇÃëSñ≈");
	DrawWinOrLoseWindow((wsize * Vector2(0.25, 0.5)).ToVector2Int(), "îsñkèåè", "ñ°ï˚ÇÃëSñ≈");

	auto charactorNumCenter = (wsize * Vector2(0.25f, 0.65f)).ToVector2Int();
	auto charactorNum = _mapCtrl.GetCharactorCnt();
	int space = (window0Size.w - window1Size.w * 2) / 2 + window1Size.w / 2;
	DrawCharactorNum(charactorNumCenter - Vector2Int(space, 0), "é©åR", charactorNum.x);
	DrawCharactorNum(charactorNumCenter + Vector2Int(space, 0), "ìGåR", charactorNum.y);

	{
		Rect canMoveWindwoRect((wsize * Vector2(0.45f, 0.8f)).ToVector2Int(), window2Size);
		DrawGraph(GetDrawPos(canMoveWindwoRect.center, window2Size, Anker::center), window2H);

		int choplin20 = fileSystem.GetFontHandle("choplin20");

		DrawStringToHandle(canMoveWindwoRect.center - Vector2Int(0, canMoveWindwoRect.size.h) * 0.25f, Anker::center, 0xffffff, choplin20, "çsìÆâ¬î\");
		DrawStringToHandle(canMoveWindwoRect.center + Vector2Int(0, canMoveWindwoRect.size.h) * 0.25f, Anker::center, 0x000000, choplin20, "%d", _mapCtrl.GetCanMoveCnt());
	}

	_mapCtrl.DrawWarSituatin((wsize * Vector2(0.75, 0.5)).ToVector2Int());
}
