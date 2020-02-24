#pragma once
#include "Scene.h"
#include "../Utility/Dir.h"
#include "../Utility/Geometry.h"

class Charactor;

class BattleScene :
	public Scene
{
private:
	struct CharInf
	{
		Charactor& charactor;
		Dir dir;

		CharInf(Charactor& charactor);
	};

	CharInf _lCharInf;
	CharInf _rCharInf;

	const Size _screenSize;

	int _screenH;

public:
	BattleScene(Charactor& lChar, Charactor& rchar, SceneController& ctrl);
	~BattleScene();

	void Update(const Input& input) override final;
	void Draw(void) override final;
};

