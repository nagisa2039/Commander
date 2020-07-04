#include "UIListItem.h"

UIListItem::UIListItem(const Vector2Int& pos, std::deque<std::shared_ptr<UI>>* uiDeque) :UI(uiDeque), _rect(pos, Size(400, 74)), _isSelect(false)
{
}

UIListItem::~UIListItem()
{
}

void UIListItem::Draw()
{
}

const Rect& UIListItem::GetRect() const
{
	return _rect;
}

const bool UIListItem::GetIsSelect() const
{
	return _isSelect;
}

void UIListItem::SetPos(const Vector2Int& pos)
{
	_rect.center = pos;
}

void UIListItem::SetIsSelect(const bool select)
{
	_isSelect = select;
}
