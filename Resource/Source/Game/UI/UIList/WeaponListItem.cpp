#include <Dxlib.h>
#include "DxLibUtility.h"
#include "Application.h"
#include "FileSystem.h"
#include "WeaponListItem.h"
#include "DataBase.h"

WeaponListItem::WeaponListItem(const WeaponData& wd,
	const Vector2Int& pos, std::deque<std::shared_ptr<UI>>* uiDeque):
	_weaponData(wd), UIListItem(Rect(pos, Size(600, 50)), uiDeque)
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
	auto& rect = GetRect();
	auto drawRect = Rect(rect.center + offset, rect.size);
	drawRect.Draw(0xffffff);

	auto choplin30 = Application::Instance().GetFileSystem().GetFontHandle("choplin30edge");

	const int offsetX = 10;
	int drawX = drawRect.Left() + offsetX;

	int sizeH = rect.size.h * 0.8f;
	_weaponData.DrawWeaponIcon(Rect(Vector2Int(drawX + sizeH /2, drawRect.center.y), Size(sizeH, sizeH)));
	drawX += (sizeH + offsetX);

	DrawStringToHandle(Vector2Int(drawX, drawRect.center.y), Anker::leftcenter, GetIsSelect() ? 0xff0000 : 0xffffff, choplin30, _weaponData.name.c_str());

	DrawStringToHandle(Vector2Int(drawRect.Right() - offsetX, drawRect.center.y), Anker::rightcenter, 0xffffff, choplin30, "%d%s", _weaponData.price, " G");
}

const WeaponData& WeaponListItem::GetWeaponData()
{
	return _weaponData;
}
