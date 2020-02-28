#pragma once
#include "Scene.h"
#include "../Utility/Dir.h"
#include "../Utility/Geometry.h"
#include <vector>
#include <memory>

class Camera;
class Effect;
class Charactor;
class BattleCharactor;

class BattleScene :
	public Scene
{
private:
	BattleCharactor& _leftBC;
	BattleCharactor& _rightBC;

	const Size _screenSize;
	int _screenH;
	std::shared_ptr<Camera> _camera;

	std::vector<std::shared_ptr<Effect>> _effects;

	float _floatY;

	void LeftTurn(const Input& input);
	void LeftHPAnim(const Input& input);

	void RightTuen(const Input& input);
	void RightHPAnim(const Input& input);

	void(BattleScene::* _updater)(const Input& input);

public:
	BattleScene(BattleCharactor& leftBC, BattleCharactor& rightBC, SceneController& ctrl);
	~BattleScene();

	void Update(const Input& input) override final;
	void Draw(void) override final;

	std::vector<std::shared_ptr<Effect>>& GetEffectVec();
};

