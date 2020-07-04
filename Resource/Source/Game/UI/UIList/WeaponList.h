#pragma once
#include "UIList.h"
class WeaponList :
    public UIList
{
private:
	void Decision()override;

public:
	WeaponList(const Rect& rect, std::deque<std::shared_ptr<UI>>& uiDeque);
	~WeaponList();
};

