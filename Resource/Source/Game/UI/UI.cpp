#include "UI.h"

UI::UI(std::deque<std::shared_ptr<UI>>& uiDeque):_uiDeque(uiDeque)
{
	_delete = false;
}

UI::~UI()
{
}

bool UI::GetDelete() const
{
	return _delete;
}
