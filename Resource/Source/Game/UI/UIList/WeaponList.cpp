#include "WeaponList.h"
#include "WeaponListItem.h"
#include "Application.h"
#include "DataBase.h"
#include "../WeaponStatusWindow.h"

#include <sstream>

void WeaponList::Decision()
{
	auto item = dynamic_cast<WeaponListItem*>(GetListItem());
	assert(item != nullptr);
}

WeaponList::WeaponList(const Vector2Int& leftup, std::deque<std::shared_ptr<UI>>* uiDeque)
	:UIList(leftup, 9, uiDeque)
{
	auto& dataBase = Application::Instance().GetDataBase();
	auto itemSize = std::make_shared<WeaponListItem>(dataBase.GetWeaponData(0), Vector2Int(0, 0), _uiDeque)->GetRect().size;
	int centerX = GetRect().size.w / 2;
	for (int i = 0; i < 30; i++)
	{
		auto pos = Vector2Int(centerX, i * (itemSize.h + LIST_ITEM_SPACE) + LIST_ITEM_SPACE + itemSize.h / 2);
		AddListItem(std::make_shared<WeaponListItem>(dataBase.GetWeaponData(i%6), pos, _uiDeque));
	}

	ListItemInit(leftup, itemSize);

	_weaponStatusWindow = std::make_unique<WeaponStatusWindow>(nullptr);
}

WeaponList::~WeaponList()
{
}

void WeaponList::Draw()
{
	UIList::Draw();

	auto item = dynamic_cast<WeaponListItem*>(GetListItem());
	assert(item != nullptr);
	auto rect = GetRect();

	_weaponStatusWindow->Draw(Vector2Int(rect.Right() + _weaponStatusWindow->GetSize().w/2+ 50, rect.center.y), item->GetWeaponData());
}
