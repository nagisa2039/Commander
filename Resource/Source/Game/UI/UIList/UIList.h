#pragma once
#include <vector>
#include "../UI.h"
#include "Geometry.h"

class UIListItem;

class UIList :
	public UI
{
private:
	const unsigned int DRAW_ITEM_MAX;

	Rect _rect;
	Rect _viewportRect;

	int _listWindowH;

	int _itemIdx;
	int _drawCnt;

	std::vector<std::shared_ptr<UIListItem>> _listItems;

	void DrawToListWindow();
	void SetItemIdx(const int add);
	void UpdateViewport();
	void CursorMove(const Input& input);

protected:
	const unsigned int LIST_ITEM_SPACE_Y;

	void ListItemInit();
	UIListItem* GetListItem();

	virtual void Decision()=0;
	void AddListItem(std::shared_ptr<UIListItem> item);

public:
	UIList(const Rect& rect, const unsigned int drawItemMax, std::deque<std::shared_ptr<UI>>* uiDeque);
	~UIList();

	virtual void Update(const Input& input)override;
	virtual void Draw()override;

	void SetCenter(const Vector2Int& center);
	const Rect& GetRect();
};

