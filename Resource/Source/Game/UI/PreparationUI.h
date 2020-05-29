#pragma once
#include "UI.h"
class PreparationUI :
	public UI
{
private:

	enum class Item
	{
		start,	//　戦闘開始
		placement,	// 配置換え
		warsituation,	// 戦況確認
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