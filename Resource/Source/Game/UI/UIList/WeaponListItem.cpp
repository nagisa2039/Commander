#include <Dxlib.h>
#include "DxLibUtility.h"
#include "Application.h"
#include "FileSystem.h"
#include "WeaponListItem.h"
#include "DataBase.h"

WeaponListItem::WeaponListItem(const WeaponData& wd,
	const Vector2Int& pos, std::deque<std::shared_ptr<UI>>* uiDeque):_weaponData(wd), UIListItem(pos, uiDeque)
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
	DrawStringToHandle(Vector2Int(drawRect.Left(), drawRect.center.y), Anker::leftcenter, GetIsSelect() ? 0xff0000 : 0xffffff, choplin30, _weaponData.name.c_str());
}

const WeaponData& WeaponListItem::GetWeaponData()
{
	return _weaponData;
}
