#pragma once
#include "UIList.h"

struct SaveDataCharactor;
class ItemWindow;
class WeaponWindow;
class MoneyUI;

class CharactorList :
    public UIList
{
private:
	const std::vector<SaveDataCharactor>& _charactorDatas;
	//std::unique_ptr<ItemWindow> _itemWindow;
	std::unique_ptr<WeaponWindow> _weaponWindow;
	std::unique_ptr<MoneyUI> _moneyUI;

	void Decision()override;
	void Back()override;
	void ChengeItem()override;

	uint8_t GetWeaponId();

public:
	CharactorList(const Vector2Int& leftup, const std::vector<SaveDataCharactor>& charactorDatas, std::deque<std::shared_ptr<UI>>* uiDeque);
	~CharactorList();

	void Draw()override;

	void OnActive()override;
};

