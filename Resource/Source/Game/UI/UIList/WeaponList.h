#pragma once
#include "UIList.h"
#include "../StatusWindow/WeaponWindow.h"

class WeaponList :
    public UIList
{
private:
	std::unique_ptr<WeaponWindow> _weaponWindowBefore;
	std::unique_ptr<WeaponWindow> _weaponWindowAfter;

	const unsigned int _charactorDataIdx;

	void Decision()override;
	void Back()override;
	void ChengeItem()override;

	uint8_t GetWeaponId();

public:
	WeaponList(const Vector2Int& leftup, const unsigned int charactorDataIdx, std::deque<std::shared_ptr<UI>>* uiDeque);
	~WeaponList();

	void Draw()override;
};

