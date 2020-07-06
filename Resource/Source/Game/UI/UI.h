#pragma once
#include <Deque>
#include <memory>

class Input;

class UI
{
protected:
	std::deque<std::shared_ptr<UI>>* _uiDeque;
	bool _delete;
	bool _isOpen;

public:
	UI(std::deque<std::shared_ptr<UI>>* uiDeque);
	~UI();

	virtual void Update(const Input& input) = 0;
	virtual void Draw() = 0;

	// 削除していいか
	bool GetDelete()const;

	// 開いているか(表示中)
	bool GetIsOpen()const;

	// 最前面に表示するときなどに更新処理を走らせる
	virtual void OnActive();
};