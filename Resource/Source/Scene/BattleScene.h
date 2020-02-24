#pragma once
#include "Scene.h"
#include "../Utility/Dir.h"
#include "../Utility/Geometry.h"
#include <vector>
#include <memory>

class Camera;
class Effect;
class Charactor;

class BattleScene :
	public Scene
{
private:
	struct CharInf
	{
		Charactor& charactor;
		Dir dir;
		Vector2Int drawPos;

		CharInf(Charactor& charactor);
	};

	CharInf _lCharInf;
	CharInf _rCharInf;

	const Size _screenSize;
	int _screenH;
	std::shared_ptr<Camera> _camera;

	std::vector<std::shared_ptr<Effect>> _effects;

public:
	BattleScene(Charactor& lChar, Charactor& rchar, SceneController& ctrl);
	~BattleScene();

	void Update(const Input& input) override final;
	void Draw(void) override final;
};

