#pragma once
#include <vector>
#include "UI.h"
#include "Geometry.h"

class UIListItem;

class UIList :
	public UI
{
private:
	Rect _rect;
	Rect _viewportRect;

	int _listWindowH;

	std::vector<std::unique_ptr<UIListItem>> _listItems;
	int _itemIdx;

	void DrawToListWindow();

public:
	UIList(const Size& size, std::deque<std::shared_ptr<UI>>& uiDeque);
	~UIList();

	void Update(const Input& input)override;
	void Draw()override;

	void SetCenter(const Vector2Int& center);

	const Rect& GetRect();
};

