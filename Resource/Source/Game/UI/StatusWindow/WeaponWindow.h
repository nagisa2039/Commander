#pragma once
#include "../UI.h"

struct Size;
struct Vector2Int;
struct WeaponData;

class WeaponWindow :
    public UI
{
private:
    static int _windowH;
    uint8_t _weaponId;

    void DrawToWindow();

public:
    WeaponWindow(const uint8_t weaponId, std::deque<std::shared_ptr<UI>>* uiDeque);
    ~WeaponWindow();

    void Update(const Input& input)override;
    void Draw()override;

    Size GetSize()const;
    void Draw(const Vector2Int& pos);
    void SetWeaponId(const uint8_t weaponId);
};

