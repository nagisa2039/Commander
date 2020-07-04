#include <Dxlib.h>
#include <algorithm>
#include "Input.h"
#include "UIList.h"
#include "UIListItem.h"

void UIList::DrawToListWindow()
{
	int currentScreen = GetDrawScreen();
	SetDrawScreen(_listWindowH);
	ClsDrawScreen();

	auto drawRect = Rect(_viewportRect.size.ToVector2Int() * 0.5f, _viewportRect.size);
	drawRect.Draw(0x000000);

	int idx = 0;
	for (auto& item : _listItems)
	{
		idx++;
		if (!_viewportRect.IsHit(item->GetRect()))continue;

		Vector2Int offset = drawRect.center - _viewportRect.center;
		item->Draw(offset);
	}

	SetDrawScreen(currentScreen);
}

void UIList::CursorMove(const Input& input)
{
	if (input.GetButtonDown(KEY_INPUT_X))
	{
		Decision();
		return;
	}
	if (input.GetButtonDown(0, "up"))
	{
		SetItemIdx(-1);
	}
	if (input.GetButtonDown(0, "down"))
	{
		SetItemIdx(+1);
	}
}

UIListItem* UIList::GetListItem()
{
	return _listItems[_itemIdx].get();
}

void UIList::AddListItem(std::shared_ptr<UIListItem> item)
{
	_listItems.emplace_back(item);
}

UIList::UIList(const Rect& rect, const unsigned int drawItemMax, std::deque<std::shared_ptr<UI>>& uiDeque) :UI(uiDeque), 
	DRAW_ITEM_MAX(drawItemMax), LIST_ITEM_SPACE_Y(5)
{
	_drawCnt = 0;
	_itemIdx = 0;
	_rect = rect;
	_viewportRect = Rect(rect.size.ToVector2Int()*0.5f, rect.size);

	_listWindowH = MakeScreen(rect.size.w, rect.size.h, true);
	_listItems.clear();
}

void UIList::ListItemInit()
{
	assert(_listItems.size() > 0);

	SetItemIdx(0);
	UpdateViewport();
}

UIList::~UIList()
{
}

void UIList::SetItemIdx(const int add)
{
	if (_itemIdx + add >= 0 && _itemIdx + add < _listItems.size())
	{
		_listItems[_itemIdx]->SetIsSelect(false);
		_listItems[_itemIdx + add]->SetIsSelect(true);
		_itemIdx += add;

		if (add > 0)
		{
			if (_drawCnt + add >= DRAW_ITEM_MAX - 1)
			{
				if (_itemIdx >= _listItems.size() - 1)
				{
					_drawCnt += add;
				}
				else
				{
					UpdateViewport();
				}
			}
			else
			{
				_drawCnt += add;
			}
		}
		else
		{
			if (_drawCnt + add <= 0)
			{
				if (_itemIdx <= 0)
				{
					_drawCnt += add;
				}
				else
				{
					UpdateViewport();
				}
			}
			else
			{
				_drawCnt += add;
			}
		}
	}
}

void UIList::UpdateViewport()
{
	auto itemRect = _listItems[_itemIdx]->GetRect();
	int topY = _viewportRect.size.h / 2 - LIST_ITEM_SPACE_Y - itemRect.size.h / 2;
	_viewportRect.center = itemRect.center + Vector2Int(0, topY - _drawCnt * (itemRect.size.h + LIST_ITEM_SPACE_Y));
}

void UIList::Update(const Input& input)
{
	CursorMove(input);
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