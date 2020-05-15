#pragma once
#include <Deque>
#include <memory>

class Input;

class UI
{
protected:
	std::deque<std::shared_ptr<UI>>& _uiDeque;
	bool _delete;
	bool _isOpen;

public:
	UI(std::deque<std::shared_ptr<UI>>& uiDeque);
	~UI();

	virtual void Update(const Input& input) = 0;
	virtual void Draw() = 0;

	// íœ‚µ‚Ä‚¢‚¢‚©
	bool GetDelete()const;

	// ŠJ‚¢‚Ä‚¢‚é‚©(•\¦’†)
	bool GetIsOpen()const;
};