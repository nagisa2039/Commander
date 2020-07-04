#pragma once
#include "UIListItem.h"

struct WeaponData;

class WeaponListItem :
    public UIListItem
{
private:
    const WeaponData& _weaponData;

public:
    WeaponListItem(const WeaponData& wd, 
        const Vector2Int& pos, std::deque<std::shared_ptr<UI>> uiDeque);
    ~WeaponListItem();

    void Update(const Input& input)override;
    void Draw(const Vector2Int& offset);

    const WeaponData& GetWeaponData();
};

