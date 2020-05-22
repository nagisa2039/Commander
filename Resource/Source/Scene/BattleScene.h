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

	Vector2Int _startPos;
	float _floatY;
	bool _pursuit;	// 追撃判定を行ったか

	std::unique_ptr<Track<float>> _exRateTL;
	std::unique_ptr<Track<float>> _brightTL;	// 画面高度

	bool SceneStartAnim(const Input& input);
	bool SceneEndAnim(const Input& input);

	bool LeftTurn(const Input& input);
	bool LeftHPAnim(const Input& input);

	bool RightTurn(const Input& input);
	bool RightHPAnim(const Input& input);

	bool(BattleScene::* _updater)(const Input& input);

	// 追撃できるキャラがいれば追撃する		rightAttack : 右側の攻撃が当たる範囲か
	bool PursuitAttack(const bool rightAttack = true);

public:
	BattleScene(BattleCharactor& leftBC, BattleCharactor& rightBC, SceneController& ctrl, const Vector2Int& cameraOffset);
	~BattleScene();

	void Update(const Input& input) override final;
	void BattleEnd();
	void Draw(void) override final;

	std::vector<std::shared_ptr<Effect>>& GetEffectVec();
};

