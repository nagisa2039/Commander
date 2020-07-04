#include <Dxlib.h>
#include <algorithm>
#include "Input.h"
#include "UIList.h"
#include "UIListItem.h"
#include "../SelectPen.h"

namespace
{
	constexpr unsigned int INPUT_ITV_MAX = 30;
	constexpr unsigned int INPUT_ITV_MIN = 5;
}

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

	bool move = false;
	if (input.GetButton(0, "up"))
	{
		move = true;
		if (_inputCnt >= _inputItv)
		{
			SetItemIdx(-1);
			_inputItv = max(_inputItv/2, INPUT_ITV_MIN);
			_inputCnt = 0;
		}
	}
	if (input.GetButton(0, "down"))
	{
		move = true;
		if (_inputCnt >= _inputItv)
		{
			SetItemIdx(+1);
			_inputItv = max(_inputItv / 2, INPUT_ITV_MIN);
			_inputCnt = 0;
		}
	}

	if (move)
	{
		_inputCnt++;
	}
	else
	{
		_inputItv = INPUT_ITV_MAX;
		_inputCnt = INPUT_ITV_MAX;
	}
}

void UIList::ListItemInit(const Vector2Int& leftup, const Size itemSize)
{
	Size rectSize = Size(itemSize.w + LIST_ITEM_SPACE*2, LIST_ITEM_SPACE + (itemSize.h + LIST_ITEM_SPACE) * DRAW_ITEM_MAX);
	_rect = Rect(leftup + rectSize.ToVector2Int() * 0.5f, rectSize);
	_viewportRect = Rect(rectSize.ToVector2Int() * 0.5f, rectSize);

	_listWindowH = MakeScreen(rectSize.w, rectSize.h, true);
	_selectPen = std::make_unique<SelectPen>(nullptr);

	assert(_listItems.size() > 0);

	SetItemIdx(0);
	UpdateViewport();

	_inputItv = INPUT_ITV_MAX;
	_inputCnt = INPUT_ITV_MAX;
}

UIListItem* UIList::GetListItem()
{
	return _listItems[_itemIdx].get();
}

void UIList::AddListItem(std::shared_ptr<UIListItem> item)
{
	_listItems.emplace_back(item);
}

UIList::UIList(const Vector2Int& leftup, const unsigned int drawItemMax, std::deque<std::shared_ptr<UI>>* uiDeque) :UI(uiDeque),
	DRAW_ITEM_MAX(drawItemMax), LIST_ITEM_SPACE(5)
{
	_listWindowH = -1;
	_drawCnt = 0;
	_itemIdx = 0;

}

UIList::~UIList()
{
}

void UIList::SetItemIdx(const int add)
{
	auto nextIdx = _itemIdx + add;
	if(nextIdx >= 0 && nextIdx < _listItems.size())
	{
		_listItems[_itemIdx]->SetIsSelect(false);
		_itemIdx += add;
		_listItems[_itemIdx]->SetIsSelect(true);

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
	int topY = _viewportRect.size.h / 2 - LIST_ITEM_SPACE - itemRect.size.h / 2;
	_viewportRect.center = itemRect.center + Vector2Int(0, topY - _drawCnt * (itemRect.size.h + LIST_ITEM_SPACE));
}

void UIList::Update(const Input& input)
{
	CursorMove(input);
	_selectPen->Update(input);
}

void UIList::Draw()
{
	DrawToListWindow();
	_rect.DrawGraph(_listWindowH);
	auto itemRect = _listItems[_itemIdx]->GetRect();
	_selectPen->Draw(Vector2Int(_rect.Left(), _rect.Top() + LIST_ITEM_SPACE + (LIST_ITEM_SPACE + itemRect.size.h) * _drawCnt));
}

void UIList::SetCenter(const Vector2Int& center)
{
	_rect.center = center;
}

const Rect& UIList::GetRect()
{
	return _rect;
}