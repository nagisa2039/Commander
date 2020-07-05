#pragma once
#include "UIListItem.h"

class WeaponListItem :
    public UIListItem
{
private:
    const uint8_t _weaponId;

public:
    WeaponListItem(const uint8_t _weaponId, std::deque<std::shared_ptr<UI>>* uiDeque);
    ~WeaponListItem();

    void Update(const Input& input)override;
    void Draw(const Vector2Int& offset);

    const uint8_t GetWeaponId()const;
};

