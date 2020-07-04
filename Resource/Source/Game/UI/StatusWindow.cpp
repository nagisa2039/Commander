#include <Dxlib.h>
#include "StatusWindow.h"
#include "Charactor.h"
#include "Application.h"
#include "DxLibUtility.h"
#include "FileSystem.h"
#include "DataBase.h"
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
			_uiDeque->pop_front();
			_isOpen = false;
		}
		else
		{
			_updater = &StatusWindow::NormalUpdate;
		}
	}
}

StatusWindow::StatusWindow(std::deque<std::shared_ptr<UI>>* uiDeque, const Charactor& charactor):UI(uiDeque), _charactor (charactor)
{
	_isOpen = true;
	_animTrack = make_unique<Track<float>>();
	_animTrack->AddKey(0, 0.0f);
	_animTrack->AddKey(15, 1.0f);

	_sideWindowSize = Size(250, 250);
	_centerWindowSize = Size(450, 250);

	_updater = &StatusWindow::ScaleUpdate;

	if (_windowH == -1)
	{
		_windowH = MakeScreen(_sideWindowSize.w *2+ _centerWindowSize.w, _sideWindowSize.h*2, true);
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

	auto fileSystem = Application::Instance().GetFileSystem();


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
	DrawWeapon(weaponRect);
	DrawItems(itemRect);

	SetDrawScreen(currentScreen);
}

void StatusWindow::DrawIcon(const Rect& iconRect)
{
	iconRect.DrawGraph(Application::Instance().GetFileSystem().GetImageHandle("Resource/Image/UI/statusWindow1.png"));
	_charactor.DrawCharactorIcon(iconRect);
}

void StatusWindow::DrawBaseInf(const Rect& levelRect)
{
	auto& fileSystem = Application::Instance().GetFileSystem();
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
	auto& fileSystem = Application::Instance().GetFileSystem();
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
		Anker::center, 0xffffff, choplin30, GetAttackRengeString(_charactor.GetAttackRange()).c_str());
}

void StatusWindow::DrawStatus(const Rect& statusRect)
{
	auto& fileSystem = Application::Instance().GetFileSystem();
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

void StatusWindow::DrawWeapon(const Rect& weaponRect)
{
	auto& fileSystem = Application::Instance().GetFileSystem();
	weaponRect.DrawGraph(fileSystem.GetImageHandle("Resource/Image/UI/statusWindow1.png"));

	auto choplin30 = fileSystem.GetFontHandle("choplin30edge");
	auto& weaponData = Application::Instance().GetDataBase().GetWeaponData(_charactor.GetStartStatus().weaponId);

	// ïêäÌñºÇÃï`âÊ
	Size nameRectSize = Size(250, 50);
	auto weaponNameRect = Rect(Vector2Int(weaponRect.center.x, weaponRect.Top() + nameRectSize.h/2), nameRectSize);
	weaponNameRect.DrawGraph(fileSystem.GetImageHandle("Resource/Image/UI/equipmentFrame.png"));

	int spaceX = 5;
	Size atributeIconSize(40, 40);
	_charactor.DrawAtributeIcon(Rect(Vector2Int(weaponNameRect.Left() + atributeIconSize.w/2 + spaceX, weaponNameRect.center.y), atributeIconSize));

	DrawStringToHandle(Vector2Int(weaponNameRect.center.x + (atributeIconSize.w + spaceX) /2, weaponNameRect.center.y), Anker::center,
		0xffffff, choplin30, weaponData.name.c_str());
	
	int itemH		= fileSystem.GetImageHandle("Resource/Image/UI/equipmentStatusFrame.png");
	Size itemSize	= Size(125, 30);
	Rect itemRect	= Rect(Vector2Int(weaponRect.Left() + itemSize.w / 2, weaponNameRect.Botton() + itemSize.h / 2), itemSize);
	auto choplin20	= fileSystem.GetFontHandle("choplin20");

	auto drawItemNum = [&itemH, &itemRect, &choplin20](const char* str, const int value)
	{
		itemRect.DrawGraph(itemH);
		DrawStringToHandle(itemRect.center - Vector2Int(itemRect.size.w / 4, 0), Anker::center, 0xffffff, choplin20, str);
		DrawStringToHandle(itemRect.center + Vector2Int(itemRect.size.w/4, 0), Anker::center, 0xffffff, choplin20, "%d", value);
	};

	auto drawItemStr = [&itemH, &itemRect, &choplin20](const char* str, const char* value)
	{
		itemRect.DrawGraph(itemH);
		DrawStringToHandle(itemRect.center - Vector2Int(itemRect.size.w / 4, 0), Anker::center, 0xffffff, choplin20, str);
		DrawStringToHandle(itemRect.center + Vector2Int(itemRect.size.w / 4, 0), Anker::center, 0xffffff, choplin20, value);
	};

	drawItemNum("à–óÕ", weaponData.power);
	itemRect.center.y += itemRect.size.h;
	drawItemNum("ñΩíÜ", weaponData.hit);
	itemRect.center.y += itemRect.size.h;
	drawItemNum("ïKéE", weaponData.critical);
	itemRect.center = Vector2Int(weaponRect.Left() + itemSize.w / 2 + itemSize.w, weaponNameRect.Botton() + itemSize.h / 2);
	drawItemStr("éÀíˆ", GetAttackRengeString(_charactor.GetAttackRange()).c_str());
	itemRect.center.y += itemRect.size.h;
	drawItemNum("èdÇ≥", weaponData.weight);

	Size weaponTextSize = Size(250, 110);
	auto weaponTextRect = Rect(Vector2Int(weaponRect.center.x, weaponRect.Botton() - weaponTextSize.h / 2), weaponTextSize);
	weaponTextRect.DrawGraph(fileSystem.GetImageHandle("Resource/Image/UI/weaponTextFrame.png"));
}

void StatusWindow::DrawItems(const Rect& itemRect)
{
	auto& fileSystem = Application::Instance().GetFileSystem();

	itemRect.DrawGraph(fileSystem.GetImageHandle("Resource/Image/UI/statusWindow1.png"));

	auto itemSize = Size(250, 40);

	std::string itemNames[] = {"èùñÚ", "ê¶Ç¢èùñÚ", "É{ÉçÇÃíﬁÇËä∆", "íTåüÉZÉbÉg" };

	auto choplin30 = fileSystem.GetFontHandle("choplin30edge");
	Vector2Int center = Vector2Int(itemRect.center.x, itemRect.Top() +  itemSize.h / 2);
	for (const auto& itemName : itemNames)
	{
		Rect(center, itemSize).DrawGraph(fileSystem.GetImageHandle("Resource/Image/UI/itemFrame.png"));
		DrawStringToHandle(center, Anker::center, 0xffffff, choplin30, itemName.c_str());
		center.y += itemSize.h;
	}
}

std::string StatusWindow::GetAttackRengeString(const Range& attackRange)
{
	char str[256];
	if (attackRange.min == attackRange.max)
	{
		sprintf_s(str, 256, "%d", attackRange.max);
	}
	else
	{
		sprintf_s(str, 256, "%d Å` %d", attackRange.min, attackRange.max);
	}
	return str;
}
