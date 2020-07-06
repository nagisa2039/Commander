#pragma once
#include <deque>
#include "Scene.h"

class UI;
class CharactorList;

class ShopScene :
	public Scene
{
private:
	std::deque<std::shared_ptr<UI>> _uiDeque;
	std::shared_ptr<CharactorList> _charactorList;

public:
	ShopScene(SceneController& controller);
	~ShopScene();

	void Update(const Input& input)override;

	void Draw(void)override;
};

