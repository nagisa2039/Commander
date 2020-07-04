#pragma once
#include "../UI.h"
#include "Geometry.h"

class UIListItem :
    public UI
{
private:
    Rect _rect;
    bool _isSelect;

public:
    UIListItem(const Rect& rect, std::deque<std::shared_ptr<UI>>* uiDeque);
    ~UIListItem();

    virtual void Update(const Input& input)override = 0;
    virtual void Draw()override;

    virtual void Draw(const Vector2Int& offset) = 0;

    const Rect& GetRect()const;
    const bool GetIsSelect()const;

    void SetPos(const Vector2Int& pos);

    void SetIsSelect(const bool select);
};

