#pragma once
#include "Scene.h"
#include "../Utility/Dir.h"
#include "../Utility/Geometry.h"
#include <vector>
#include <memory>
#include "TimeLine.h"

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
	bool _pursuit;	// �ǌ�������s������

	std::unique_ptr<TimeLine<float>> _exRateTL;
	std::unique_ptr<TimeLine<float>> _brightTL;	// ��ʍ��x

	bool SceneStartAnim(const Input& input);
	bool SceneEndAnim(const Input& input);

	bool LeftTurn(const Input& input);
	bool LeftHPAnim(const Input& input);

	bool RightTurn(const Input& input);
	bool RightHPAnim(const Input& input);

	bool(BattleScene::* _updater)(const Input& input);

	// �ǌ������邩	�������鑤��Ԃ�	���Ȃ��ꍇ��max���Ԃ��Ă���
	bool PursuitAttack();

public:
	BattleScene(BattleCharactor& leftBC, BattleCharactor& rightBC, SceneController& ctrl);
	~BattleScene();

	void Update(const Input& input) override final;
	void BattleEnd();
	void Draw(void) override final;

	std::vector<std::shared_ptr<Effect>>& GetEffectVec();
};

