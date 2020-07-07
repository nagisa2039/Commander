#include <Dxlib.h>
#include "DxLibUtility.h"
#include "Application.h"
#include "FileSystem.h"
#include "WeaponListItem.h"
#include "DataBase.h"

WeaponListItem::WeaponListItem(const uint8_t weaponId, std::deque<std::shared_ptr<UI>>* uiDeque):
	_weaponId(weaponId), UIListItem(Rect(Vector2Int(), Size(600, 50)), uiDeque)
{

}

WeaponListItem::~WeaponListItem()
{
}

void WeaponListItem::Update(const Input& input)
{
}

void WeaponListItem::Draw(const Vector2Int& offset)
{
	auto& weaponData = Application::Instance().GetDataBase().GetWeaponData(_weaponId);

	auto& rect = GetRect();
	auto drawRect = Rect(rect.center + offset, rect.size);
	drawRect.Draw(0xffffff);

	auto choplin30 = Application::Instance().GetFileSystem().GetFontHandle("choplin30edge");

	const int offsetX = 10;
	int drawX = drawRect.Left() + offsetX;

	int sizeH = rect.size.h * 0.8f;
	weaponData.GetTypeData().DrawWeaponIcon(Rect(Vector2Int(drawX + sizeH /2, drawRect.center.y), Size(sizeH, sizeH)));
	drawX += (sizeH + offsetX);

	DrawStringToHandle(Vector2Int(drawX, drawRect.center.y), Anker::leftcenter, GetIsSelect() ? 0xff0000 : 0xffffff, choplin30, weaponData.name.c_str());

	DrawStringToHandle(Vector2Int(drawRect.Right() - offsetX, drawRect.center.y), Anker::rightcenter, 0xffffff, choplin30, "%d%s", weaponData.price, " G");
}

const uint8_t WeaponListItem::GetWeaponId()const
{
	return _weaponId;
}
