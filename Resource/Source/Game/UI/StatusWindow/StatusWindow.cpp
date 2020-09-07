#include <Dxlib.h>
#include "StatusWindow.h"
#include "Charactor.h"
#include "Application.h"
#include "DxLibUtility.h"
#include "FileSystem.h"
#include "DataBase.h"
#include "Input.h"
#include "WeaponWindow.h"
#include "ItemWindow.h"
#include "SceneController.h"
#include "PlayScene.h"
#include "SoundLoader.h"

using namespace std;

void StatusWindow::NormalUpdate(const Input& input)
{
	if (input.GetButtonDown("status") || input.GetButtonDown("back"))
	{
		SoundL.PlaySE("Resource/Sound/SE/select06.mp3");
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
			_delete = true;
		}
		else
		{
			_updater = &StatusWindow::NormalUpdate;
		}
	}
}

StatusWindow::StatusWindow(std::deque<std::shared_ptr<UI>>* uiDeque, const Charactor& charactor):UI(uiDeque), _charactor (charactor)
{
	_weaponWindow = make_unique<WeaponWindow>(_charactor.GetStatus().weaponId, nullptr);
	_itemWindow = make_unique<ItemWindow>(nullptr);

	_isOpen = true;
	_animTrack = make_unique<Track<float>>();
	_animTrack->AddKey(0, 0.0f);
	_animTrack->AddKey(15, 1.0f);

	_sideWindowSize = Size(250, 250);
	_centerWindowSize = Size(450, 250);

	_updater = &StatusWindow::ScaleUpdate;
	SoundL.PlaySE("Resource/Sound/SE/select05.mp3");

	_windowH = FileSystem::Instance().
		MakeScreen("status_window", Size(_sideWindowSize.w *2+ _centerWindowSize.w, _sideWindowSize.h*2), true);

	_playScene = &dynamic_cast<PlayScene&>(Application::Instance().GetSceneController().GetCurrentScene());
	if (_playScene)
	{
		_playScene->SetFilter(PlayScene::FilterType::gauss);
	}
}

StatusWindow::~StatusWindow()
{
	if (_playScene)
	{
		_playScene->SetFilter(PlayScene::FilterType::none);
	}
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
	auto& fileSystem = FileSystem::Instance();

	Rect iconRect(_sideWindowSize.ToVector2Int() * 0.5f, _sideWindowSize);
	Rect levelRect(iconRect.center + Vector2Int(0, _sideWindowSize.h), _sideWindowSize);
	Rect battleStatusRect(Vector2Int(iconRect.Right() + static_cast<int>(_centerWindowSize.w * 0.5f), iconRect.center.y), _centerWindowSize);
	Rect statusRect(Vector2Int(battleStatusRect.center.x, battleStatusRect.Botton() + static_cast<int>(_centerWindowSize.h * 0.5f)), _centerWindowSize);
	Rect weaponRect(Vector2Int(battleStatusRect.Right() + _sideWindowSize.w/2, battleStatusRect.Top() + _sideWindowSize.h/2), _sideWindowSize);
	Rect itemRect(weaponRect.center + Vector2Int(0, _sideWindowSize.h), _sideWindowSize);

	DrawIcon(iconRect);
	DrawBaseInf(levelRect);
	DrawBattleStatus(battleStatusRect);
	DrawStatus(statusRect);
	_weaponWindow->Draw(weaponRect.center);
	_itemWindow->Draw(itemRect.center);

	SetDrawScreen(currentScreen);
}

void StatusWindow::DrawIcon(const Rect& iconRect)
{
	iconRect.DrawGraph(ImageHandle("Resource/Image/UI/statusWindow1.png"));
	_charactor.DrawCharactorIcon(iconRect);
}

void StatusWindow::DrawBaseInf(const Rect& levelRect)
{
	auto& fileSystem = FileSystem::Instance();
	levelRect.DrawGraph(fileSystem.GetImageHandle("Resource/Image/UI/statusWindow1.png"));

	Size contentSize(250, 250 / 4);
	Vector2Int center(levelRect.Left() + contentSize.w / 2, levelRect.Top() + contentSize.h / 2);

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
		auto drawPos = GetDrawPos(Vector2Int(levelRect.Left(), center.y), strSize, Anker::leftcenter);
		DrawFormatStringToHandle(drawPos.x + 10, drawPos.y, 0xffffff, choplin40, leftStr.c_str());

		GetDrawFormatStringSizeToHandle(&strSize.w, &strSize.h, &line, choplin40, rightStr.c_str());
		drawPos = GetDrawPos(Vector2Int(levelRect.Right(), center.y), strSize, Anker::rightcenter);
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

void StatusWindow::DrawBattleStatus(const Rect& battleStatusRect)
{
	auto& fileSystem = FileSystem::Instance();
	battleStatusRect.DrawGraph(fileSystem.GetImageHandle("Resource/Image/UI/statusWindow2.png"));

	auto choplin30 = fileSystem.GetFontHandle("choplin30edge");

	// 150, 125
	Rect contentRect = Rect(Vector2Int(0, 0), Size(_centerWindowSize.w / 3, _centerWindowSize.h / 2));
	contentRect.center = Vector2Int(battleStatusRect.Left(), battleStatusRect.Top()) + contentRect.size * 0.5f;
	int spaceX = 10;

	auto DrawContentVertical = [&](const string& name, const int num)
	{
		contentRect.DrawGraph(fileSystem.GetImageHandle("Resource/Image/UI/verticalWindow.png"));
		DrawStringToHandle(Vector2Int(contentRect.center.x, contentRect.center.y - static_cast<int>(contentRect.size.h * 0.25f)), 
			Anker::center, 0xffffff, choplin30, name.c_str());
		DrawStringToHandle(Vector2Int(contentRect.center.x, contentRect.center.y + static_cast<int>(contentRect.size.h * 0.25f)), 
			Anker::center, 0xffffff, choplin30, "%d", num);
	};

	auto DrawContentHorizontal = [&](const string& name, const int num)
	{
		contentRect.DrawGraph(fileSystem.GetImageHandle("Resource/Image/UI/horizontalWindow.png"));
		DrawStringToHandle(Vector2Int(contentRect.center.x - static_cast<int>(contentRect.size.w * 0.25f), contentRect.center.y), 
			Anker::center, 0xffffff, choplin30, name.c_str());
		DrawStringToHandle(Vector2Int(contentRect.center.x + static_cast<int>(contentRect.size.w * 0.25f), contentRect.center.y), 
			Anker::center, 0xffffff, choplin30, "%d", num);
	};

	auto currentBattleStatus = _charactor.GetBattleStatus();
	// çUåÇ
	DrawContentVertical("çUåÇ", currentBattleStatus.GetPower());
	contentRect.center.x += contentRect.size.w;
	// ñΩíÜ
	DrawContentVertical("ñΩíÜ", currentBattleStatus.GetHitRate());
	contentRect.center.x += contentRect.size.w;
	// ïKéE
	DrawContentVertical("ïKéE", currentBattleStatus.GetCriticalRate());
	contentRect.center.x += contentRect.size.w;

	//150, 62
	contentRect.size = Size(_centerWindowSize.w / 3, _centerWindowSize.h / 4);
	contentRect.center = Vector2Int(battleStatusRect.Left(), battleStatusRect.center.y) + contentRect.size * 0.5f;

	DrawContentHorizontal("çUë¨", currentBattleStatus.GetAttackSpeed());
	contentRect.center.x += contentRect.size.w;
	DrawContentHorizontal("ñhå‰", currentBattleStatus.GetDifense());
	contentRect.center.x += contentRect.size.w;
	DrawContentHorizontal("ëœñÇ", currentBattleStatus.GetMagicDifense());

	contentRect.center.x = battleStatusRect.Left() + static_cast<int>(contentRect.size.w * 0.5f);
	contentRect.center.y += contentRect.size.h;
	DrawContentHorizontal("âÒî", currentBattleStatus.GetAvoidance());

	// 300, 62
	contentRect.center.x += static_cast<int>(contentRect.size.w * 1.5f);
	contentRect.size.w *= 2;

	contentRect.DrawGraph(fileSystem.GetImageHandle("Resource/Image/UI/horizontalWindow.png"));
	DrawStringToHandle(Vector2Int(contentRect.center.x - static_cast<int>(contentRect.size.w * 0.25f), contentRect.center.y), 
		Anker::center, 0xffffff, choplin30, "éÀíˆ");
	DrawStringToHandle(Vector2Int(contentRect.center.x + static_cast<int>(contentRect.size.w * 0.25f), contentRect.center.y),
		Anker::center, 0xffffff, choplin30, 
		Application::Instance().GetDataBase().GetWeaponData(_charactor.GetStartStatus().weaponId).GetRengeString().c_str());
}

void StatusWindow::DrawStatus(const Rect& statusRect)
{
	auto& fileSystem = FileSystem::Instance();
	statusRect.DrawGraph(fileSystem.GetImageHandle("Resource/Image/UI/statusWindow2.png"));

	auto choplin30 = fileSystem.GetFontHandle("choplin30edge");

	Size contentSize(_centerWindowSize.w / 2, _centerWindowSize.h / 4);
	Rect contentRect(Vector2Int(statusRect.Left(), statusRect.Top()) + contentSize * 0.5f, contentSize);

	auto DrawNumContent = [&](const Rect& contentRect, const string& leftStr, const uint8_t num)
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

	DrawNumContent(contentRect, "ñÇóÕ", currentStatus.magic_power);
	contentRect.center.y += contentRect.Height();

	DrawNumContent(contentRect, "ãZ", currentStatus.skill);
	contentRect.center.y += contentRect.Height();

	DrawNumContent(contentRect, "ë¨Ç≥", currentStatus.speed);
	contentRect.center = Vector2Int(statusRect.Left(), statusRect.Top()) + contentSize * 0.5f;
	contentRect.center.x += contentRect.Width();

	DrawNumContent(contentRect, "çKâ^", currentStatus.luck);
	contentRect.center.y += contentRect.Height();

	DrawNumContent(contentRect, "éÁîı", currentStatus.defense);
	contentRect.center.y += contentRect.Height();

	DrawNumContent(contentRect, "ñÇñh", currentStatus.magic_defense);
}