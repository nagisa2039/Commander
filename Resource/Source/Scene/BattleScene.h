#pragma once
#include "Scene.h"
#include "../Utility/Dir.h"
#include "../Utility/Geometry.h"
#include <vector>
#include <memory>
#include <deque>
#include "TimeLine.h"

class UI;
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
	std::deque< std::shared_ptr<UI>> _expUIDeque;

	Vector2Int _startPos;
	float _floatY;
	bool _pursuit;	// �ǌ�������s������

	std::unique_ptr<Track<float>> _exRateTL;
	std::unique_ptr<Track<float>> _brightTL;	// ��ʍ��x

	void SceneStartAnim(const Input& input);
	void SceneEndAnim(const Input& input);

	void LeftTurn(const Input& input);
	void LeftHPAnim(const Input& input);

	void RightTurn(const Input& input);
	void RightHPAnim(const Input& input);

	void ExpUpdate(const Input& input);

	void(BattleScene::* _updater)(const Input& input);

	// �ǌ��ł���L����������Βǌ�����		rightAttack : �E���̍U����������͈͂�
	bool PursuitAttack(const bool rightAttack = true);
	void End();
	void StartExpUpdate();

public:
	BattleScene(BattleCharactor& leftBC, BattleCharactor& rightBC, SceneController& ctrl, const Vector2Int& cameraOffset);
	~BattleScene();

	void Update(const Input& input) override final;
	void Draw(void) override final;

	std::vector<std::shared_ptr<Effect>>& GetEffectVec();
};

