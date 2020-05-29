#pragma once
#include "UI.h"
class PreparationUI :
	public UI
{
private:

	enum class Item
	{
		start,	//�@�퓬�J�n
		placement,	// �z�u����
		warsituation,	// �틵�m�F
		max
	};
	Item _selectItem;
	void (PreparationUI::* _updater)(const Input& input);
	

public:
	PreparationUI(std::deque<std::shared_ptr<UI>>& uiDeque);
	~PreparationUI();

	void Update(const Input& input)override;
	void Draw()override;
};