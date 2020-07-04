#pragma once
#include "UI.h"

struct Size;
struct Vector2Int;
struct WeaponData;

class WeaponStatusWindow :
    public UI
{
private:

public:
    WeaponStatusWindow(std::deque<std::shared_ptr<UI>>* uiDeque);
    ~WeaponStatusWindow();

    Size GetSize()const;

    void Update(const Input& input)override;
    void Draw()override;

    void Draw(const Vector2Int& pos, const WeaponData& weaponData);
};

