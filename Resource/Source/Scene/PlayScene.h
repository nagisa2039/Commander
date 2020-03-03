#pragma once
#include <memory>
#include <list>
#include <vector>
#include "Scene.h"
#include "../Utility/Geometry.h"
#include "../Game/Team.h"
#include <list>

class Charactor;
class MapCtrl;
class Camera;
class PlayerCommander;
class EnemyCommander;
class Effect;

class PlayScene :
	public Scene
{
private:
	bool debug;
	int _gameH;	// ƒQ[ƒ€‰æ–Ê‚Ìƒnƒ“ƒhƒ‹
	
	std::shared_ptr<MapCtrl> _mapCtrl;
	std::shared_ptr<Camera> _camera;
	std::vector<std::shared_ptr<Charactor>> _charactors;
	std::shared_ptr<PlayerCommander> _playerCommander;
	std::shared_ptr<EnemyCommander> _enemyCommander;
	std::vector<std::shared_ptr<Effect>> _effects;

	void PlayerTurn(const Input& input);
	void EnemyTurn(const Input& input);
	void(PlayScene::*_turnUpdater)(const Input& input);

public:
	PlayScene(SceneController & ctrl);
	~PlayScene();

	void Update(const Input& input) override final;
	void Draw(void) override final;
};
