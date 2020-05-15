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

	// �폜���Ă�����
	bool GetDelete()const;

	// �J���Ă��邩(�\����)
	bool GetIsOpen()const;
};