#pragma once
#include "UIList.h"

struct CharactorData;
class ItemWindow;
class WeaponWindow;

class CharactorList :
    public UIList
{
private:
	const std::vector<CharactorData>& _charactorDatas;
	std::unique_ptr<ItemWindow> _itemWindow;
	std::unique_ptr<WeaponWindow> _weaponWindow;

	void Decision()override;
	void Back()override;
	void ChengeItem()override;

	uint8_t GetWeaponId();

public:
	CharactorList(const Vector2Int& leftup, const std::vector<CharactorData>& charactorDatas, std::deque<std::shared_ptr<UI>>* uiDeque);
	~CharactorList();

	void Draw()override;
};

