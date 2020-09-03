#pragma once
#include "../UI.h"

struct Size;
struct Vector2Int;

class ItemWindow :
    public UI
{
private:
    int _windowH;

    void DrawToWindow();

public:
    ItemWindow(std::deque<std::shared_ptr<UI>>* uiDeque);
    ~ItemWindow();

    Size GetSize()const;

    void Update(const Input& input)override;
    void Draw()override;

    void Draw(const Vector2Int& pos);

    void SetHoge();
};

