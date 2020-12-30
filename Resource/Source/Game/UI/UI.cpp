#include "UI.h"

UI::UI(std::deque<std::shared_ptr<UI>>* uiDeque):_uiDeque(uiDeque)
{
	_delete = false;
	_isOpen = false;
}

UI::~UI()
{
}

void UI::Update(const Input& input)
{
}

bool UI::GetDelete() const
{
	return _delete;
}

bool UI::GetIsOpen() const
{
	return _isOpen;
}

void UI::OnActive()
{
}
