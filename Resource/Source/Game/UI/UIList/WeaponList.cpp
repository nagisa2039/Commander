#include "WeaponList.h"
#include "WeaponListItem.h"
#include "Application.h"
#include "DataBase.h"
#include "../StatusWindow/WeaponWindow.h"
#include <sstream>

WeaponList::WeaponList(const Vector2Int& leftup, std::deque<std::shared_ptr<UI>>* uiDeque)
	:UIList(9, uiDeque)
{
	for (int i = 0; i < 30; i++)
	{
		AddListItem(std::make_shared<WeaponListItem>(i%6, nullptr));
	}

	ListItemInit(leftup);
}

WeaponList::~WeaponList()
{
}

void WeaponList::Draw()
{
	UIList::Draw();

	auto rect = GetRect();
	_weaponStatusWindow->Draw(Vector2Int(rect.Right() + _weaponStatusWindow->GetSize().w/2+ 50, rect.center.y));
}
