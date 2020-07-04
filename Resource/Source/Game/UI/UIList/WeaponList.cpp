#include "WeaponList.h"
#include "WeaponListItem.h"
#include "Application.h"
#include "DataBase.h"

#include <sstream>

void WeaponList::Decision()
{
	auto item = dynamic_cast<WeaponListItem*>(GetListItem());
	assert(item != nullptr);


}

WeaponList::WeaponList(const Rect& rect, std::deque<std::shared_ptr<UI>>& uiDeque)
	:UIList(rect, 5, uiDeque)
{
	auto& dataBase = Application::Instance().GetDataBase();
	auto itemSize = std::make_shared<WeaponListItem>(dataBase.GetWeaponData(0), Vector2Int(0, 0), _uiDeque)->GetRect().size;
	int centerX = GetRect().size.w / 2;
	for (int i = 0; i < 10; i++)
	{
		auto pos = Vector2Int(centerX, i * (itemSize.h + LIST_ITEM_SPACE_Y) + LIST_ITEM_SPACE_Y + itemSize.h / 2);
		AddListItem(std::make_shared<WeaponListItem>(dataBase.GetWeaponData(i%6), pos, _uiDeque));
	}

	ListItemInit();
}

WeaponList::~WeaponList()
{
}