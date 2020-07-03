#include <Dxlib.h>
#include <algorithm>
#include "Input.h"
#include "UIList.h"
#include "UIListItem.h"

namespace
{
	constexpr unsigned int LIST_ITEM_SPACE_Y = 20;
}

void UIList::DrawToListWindow()
{
	int currentScreen = GetDrawScreen();
	SetDrawScreen(_listWindowH);
	ClsDrawScreen();

	auto drawRect = Rect(_viewportRect.size.ToVector2Int() * 0.5f, _viewportRect.size);
	drawRect.Draw(0x000000);

	int idx = 0;
	for (int idx = 0; idx < _listItems.size(); idx++)
	{
		if (!_viewportRect.IsHit(_listItems[idx]->GetRect()))continue;

		Vector2Int offset = drawRect.center - _viewportRect.center;
		_listItems[idx]->Draw(offset);

		auto rect = _listItems[idx]->GetRect();
		DrawFormatString(rect.center.x + offset.x, rect.center.y + offset.y, 0x000000, "%d", idx);
	}

	SetDrawScreen(currentScreen);
}

UIList::UIList(const Size& size, std::deque<std::shared_ptr<UI>>& uiDeque) :UI(uiDeque)
{
	_rect = Rect(Vector2Int(400,400), size);
	_viewportRect = Rect(size.ToVector2Int()*0.5f, size);

	_listWindowH = MakeScreen(size.w, size.h, true);

	_listItems.clear();
	_listItems.reserve(10);

	Size itemSize(size.w/2, size.h/5);

	_itemIdx = 0;

	for (int i = 0; i < 50; i++)
	{
		auto rect = Rect(Vector2Int(_rect.size.w/2, i * (itemSize.h + LIST_ITEM_SPACE_Y) + LIST_ITEM_SPACE_Y + itemSize.h/2), itemSize);
		_listItems.emplace_back(std::make_unique<UIListItem>(rect, uiDeque));
	}
}

UIList::~UIList()
{
}

void UIList::Update(const Input& input)
{
	if (input.GetButtonDown(0, "up"))
	{
		_itemIdx = max(_itemIdx-1, 0);
	}
	if (input.GetButtonDown(0, "down"))
	{
		_itemIdx = min(_itemIdx + 1, _listItems.size()-1);
	}

	auto itemRect = _listItems[_itemIdx]->GetRect();
	_viewportRect.center = itemRect.center + Vector2Int(0, _viewportRect.size.h/2 - LIST_ITEM_SPACE_Y - itemRect.size.h / 2);
}

void UIList::Draw()
{
	DrawToListWindow();
	_rect.DrawGraph(_listWindowH);
}

void UIList::SetCenter(const Vector2Int& center)
{
	_rect.center = center;
}

const Rect& UIList::GetRect()
{
	return _rect;
}
