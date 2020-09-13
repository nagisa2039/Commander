#include <Dxlib.h>
#include <algorithm>
#include "../../../Utility/Input.h"
#include "UIList.h"
#include "UIListItem.h"
#include "../SelectPen.h"
#include "Application.h"
#include "FileSystem.h"

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

void UIList::InitInputItv()
{
	_inputItv = INPUT_ITV_MAX;
	_inputCnt = INPUT_ITV_MAX;
}

void UIList::MouseMove(const Input& input)
{
	auto mousePosRect = Rect(input.GetMousePos(), Size(1,1));
	if (mousePosRect.IsHit(_rect))
	{
		auto listMousePos = mousePosRect.center
			- (_rect.center - _rect.size.ToVector2Int() * 0.5f)
			+ (_viewportRect.center - _viewportRect.size.ToVector2Int()*0.5f);

		int idx = 0;
		bool sideOver = false;
		bool sideClick = false;
		for (const auto item : _listItems)
		{
			// “–‚½‚Á‚Ä‚È‚¢
			if (!Rect(listMousePos, Size(1, 1)).IsHit(item->GetRect()))
			{
				idx++;
				continue;
			}

			auto add = idx - _itemIdx;

			// ã‰º‚ÌÛ‚É‚ ‚é
			if (_drawCnt + add >= static_cast<int>(_drawItemMax - 1) || _drawCnt + add <= 0)
			{
				sideOver = true;
				if (_inputCnt >= _inputItv)
				{
					sideClick = true;
					_inputItv = max(_inputItv / 2, INPUT_ITV_MIN);
					_inputCnt = 0;
					SetItemIdx(idx);
					break;
				}
			}
			else
			{
				SetItemIdx(idx);
				break;
			}
			idx++;
		}

		if (!sideClick)
		{
			if (sideOver)
			{
				_inputCnt++;
			}
			else
			{
				InitInputItv();
			}
		}

		if (input.GetButtonDown("ok"))
		{
			Decision();
			return;
		}
	}

	if (input.GetButtonDown("back"))
	{
		Back();
		return;
	}

	if (input.GetAnyKeybordInput() || input.GetAnyPadInput())
	{
		_mover = &UIList::KeybordMove;
		return;
	}

}

void UIList::KeybordMove(const Input& input)
{
	if (input.GetButtonDown("ok"))
	{
		Decision();
		return;
	}

	if (input.GetButtonDown("back"))
	{
		Back();
		return;
	}

	bool move = false;
	int moveCnt = -1;
	if (input.GetButton("up")
		&& _itemIdx + moveCnt > 0)
	{
		move = true;
		if (_inputCnt >= _inputItv)
		{
			SetItemIdx(_itemIdx + moveCnt);
			_inputItv = max(_inputItv / 2, INPUT_ITV_MIN);
			_inputCnt = 0;
		}
	}
	moveCnt = 1;
	if (input.GetButton("down")
		&& static_cast<size_t>(_itemIdx) + moveCnt < _listItems.size())
	{
		move = true;
		if (_inputCnt >= _inputItv)
		{
			SetItemIdx(_itemIdx + moveCnt);
			_inputItv = max(_inputItv / 2, INPUT_ITV_MIN);
			_inputCnt = 0;

		}
	}

	if (input.GetMouseMove() != Vector2Int(0, 0))
	{
		_mover = &UIList::MouseMove;
		return;
	}

	if (move)
	{
		_inputCnt++;
		return;
	}
	else
	{
		InitInputItv();
		return;
	}

}

void UIList::ListItemInit(const Vector2Int& leftup)
{
	assert(_listItems.size() > 0);

	auto itemSize = _listItems[0]->GetRect().size;
	Size rectSize = Size(itemSize.w + LIST_ITEM_SPACE*2, LIST_ITEM_SPACE + (itemSize.h + LIST_ITEM_SPACE) * _drawItemMax);
	_rect = Rect(leftup + rectSize.ToVector2Int() * 0.5f, rectSize);
	_viewportRect = Rect(rectSize.ToVector2Int() * 0.5f, rectSize);
	for (auto& item : _listItems)
	{
		item->SetPos(Vector2Int(_viewportRect.center.x, item->GetRect().center.y));
	}

	_listWindowH = FileSystem::Instance().MakeScreen("list_window", rectSize, true);
	_selectPen = std::make_unique<SelectPen>(nullptr);

	int initIdx = 0;
	SetItemIdx(initIdx);
	_listItems[initIdx]->SetIsSelect(true);
	UpdateViewport();

	InitInputItv();
}

UIListItem* UIList::GetListItem()
{
	return _listItems[_itemIdx].get();
}

unsigned int UIList::GetListIdx()
{
	return _itemIdx;
}

void UIList::Decision()
{
}

void UIList::Back()
{
}

void UIList::ChengeItem()
{
}

void UIList::AddListItem(std::shared_ptr<UIListItem> item)
{
	auto itemSize = item->GetRect().size;
	int centerX = _rect.size.w / 2;
	auto pos = Vector2Int(centerX, static_cast<int>(_listItems.size()) * (itemSize.h + LIST_ITEM_SPACE) + LIST_ITEM_SPACE + itemSize.h / 2);
	item->SetPos(pos);
	_listItems.emplace_back(item);
}

UIList::UIList(const unsigned int drawItemMax, std::deque<std::shared_ptr<UI>>* uiDeque) :UI(uiDeque), LIST_ITEM_SPACE(5)
{
	_listWindowH = -1;
	_drawCnt = 0;
	_itemIdx = 0;
	_drawItemMax = drawItemMax;

	InitInputItv();

	_mover = &UIList::MouseMove;
}

UIList::~UIList()
{
}

void UIList::SetItemIdx(const int nextIdx)
{
	auto add = nextIdx - _itemIdx;
	if (add == 0)return;

	if(nextIdx >= 0 && nextIdx < _listItems.size())
	{
		_listItems[_itemIdx]->SetIsSelect(false);
		_itemIdx += add;
		_listItems[_itemIdx]->SetIsSelect(true);

		ChengeItem();

		if (add > 0)
		{
			if (_drawCnt + add >= static_cast<int>(_drawItemMax - 1))
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
	_selectPen->Update(input);
	(this->*_mover)(input);
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