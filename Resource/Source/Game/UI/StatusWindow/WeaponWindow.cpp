#include <Dxlib.h>
#include "WeaponWindow.h"
#include "Application.h"
#include "Geometry.h"
#include "WeaponData.h"
#include "FileSystem.h"
#include "DxLibUtility.h"
#include "DataBase.h"

namespace
{
	constexpr unsigned int WINDOW_SIZE_W = 250;
	constexpr unsigned int WINDOW_SIZE_H = 250;

	constexpr char STATUS_IMG1[]				= "Resource/Image/UI/statusWindow1.png";
	constexpr char EQUIPMENT_FRAME_IMG[]		= "Resource/Image/UI/equipmentFrame.png";
	constexpr char EQUIPMENT_STATUS_FRAME_IMG[]	= "Resource/Image/UI/equipmentStatusFrame.png";
	constexpr char WEAPON_TEXT_FRAME_IMG[]		= "Resource/Image/UI/weaponTextFrame.png";
}

WeaponWindow::WeaponWindow(const uint8_t weaponId, const Vector2Int& drawPos, std::deque<std::shared_ptr<UI>>* uiDeque)
	:UI(uiDeque), _drawPos(drawPos)
{
	_windowH = FileSystem::Instance().MakeScreen("weapon_window", Size(WINDOW_SIZE_W, WINDOW_SIZE_H), true);
	SetWeaponId(weaponId);
}

WeaponWindow::~WeaponWindow()
{
}

void WeaponWindow::Draw()
{
	Rect(_drawPos, GetSize()).DrawGraph(_windowH);
}

Size WeaponWindow::GetSize() const
{
	return Size(WINDOW_SIZE_W, WINDOW_SIZE_H);
}

void WeaponWindow::DrawToWindow()
{
	auto currentScreen = GetDrawScreen();
	SetDrawScreen(_windowH);

	auto& weaponData = DataBase::Instance().GetWeaponData(_weaponId);

	auto rectSize = GetSize();
	Rect rect = Rect(rectSize.ToVector2Int()*0.5f, rectSize);

	auto& fileSystem = FileSystem::Instance();
	rect.DrawGraph(fileSystem.GetImageHandle(STATUS_IMG1));

	auto choplin30 = fileSystem.GetFontHandle("choplin30edge");

	// ïêäÌñºÇÃï`âÊ
	Size nameRectSize = Size(250, 50);
	auto weaponNameRect = Rect(Vector2Int(rect.center.x, rect.Top() + nameRectSize.h / 2), nameRectSize);
	weaponNameRect.DrawGraph(fileSystem.GetImageHandle(EQUIPMENT_FRAME_IMG));

	int spaceX = 5;
	Size atributeIconSize(40, 40);
	weaponData.GetTypeData().DrawWeaponIcon(Rect(Vector2Int(weaponNameRect.Left() + atributeIconSize.w / 2 + spaceX, weaponNameRect.center.y), atributeIconSize));

	DrawStringToHandle(Vector2Int(weaponNameRect.center.x + (atributeIconSize.w + spaceX) / 2, weaponNameRect.center.y), Anker::center,
		0xffffff, choplin30, weaponData.name.c_str());

	int itemH = fileSystem.GetImageHandle(EQUIPMENT_STATUS_FRAME_IMG);
	Size itemSize = Size(125, 30);
	Rect itemRect = Rect(Vector2Int(rect.Left() + itemSize.w / 2, weaponNameRect.Botton() + itemSize.h / 2), itemSize);
	auto choplin20 = fileSystem.GetFontHandle("choplin20");

	auto drawItemNum = [&itemH, &itemRect, &choplin20](const char* str, const int value)
	{
		itemRect.DrawGraph(itemH);
		DrawStringToHandle(itemRect.center - Vector2Int(itemRect.size.w / 4, 0), Anker::center, 0xffffff, choplin20, str);
		DrawStringToHandle(itemRect.center + Vector2Int(itemRect.size.w / 4, 0), Anker::center, 0xffffff, choplin20, "%d", value);
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
	itemRect.center = Vector2Int(rect.Left() + itemSize.w / 2 + itemSize.w, weaponNameRect.Botton() + itemSize.h / 2);
	drawItemStr("éÀíˆ", weaponData.GetRengeString().c_str());
	itemRect.center.y += itemRect.size.h;
	drawItemNum("èdÇ≥", weaponData.weight);

	Size weaponTextSize = Size(250, 110);
	auto weaponTextRect = Rect(Vector2Int(rect.center.x, rect.Botton() - weaponTextSize.h / 2), weaponTextSize);
	weaponTextRect.DrawGraph(fileSystem.GetImageHandle(WEAPON_TEXT_FRAME_IMG));

	SetDrawScreen(currentScreen);
}

void WeaponWindow::SetWeaponId(const uint8_t weaponId)
{
	_weaponId = weaponId;

	DrawToWindow();
}
