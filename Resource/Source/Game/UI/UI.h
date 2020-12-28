#pragma once
#include <Deque>
#include <memory>

class Input;

/// <summary>
/// UI���N���X
/// </summary>
class UI
{
protected:
	// UI�̐e�q�֌W�Ǘ�Deque
	std::deque<std::shared_ptr<UI>>* _uiDeque;
	// �폜����
	bool _delete;
	// �J���Ă����Ԃ�
	bool _isOpen;

public:
	/// <param name="uiDeque">UI�̐e�q�֌W�Ǘ�Deque</param>
	UI(std::deque<std::shared_ptr<UI>>* uiDeque);
	virtual ~UI();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	virtual void Update(const Input& input) = 0;

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// �폜���Ă�����
	/// </summary>
	bool GetDelete()const;

	/// <summary>
	/// �J���Ă��邩(�\����)
	/// </summary>
	bool GetIsOpen()const;

	/// <summary>
	/// �őO�ʂɕ\������Ƃ��ȂǂɍX�V�����𑖂点��
	/// </summary>
	virtual void OnActive();
};