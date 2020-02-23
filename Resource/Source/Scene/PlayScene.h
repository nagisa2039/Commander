#pragma once
#include <memory>
#include <list>
#include <vector>
#include "Scene.h"
#include "../Utility/Geometry.h"
#include "../Game/Team.h"

class Charactor;
class MapCtrl;
class Camera;
class PlayerCursor;

class PlayScene :
	public Scene
{
private:
	bool debug;
	int _gameH;	// ƒQ[ƒ€‰æ–Ê‚Ìƒnƒ“ƒhƒ‹
	Team _turn;
	
	std::shared_ptr<MapCtrl> _mapCtrl;
	std::shared_ptr<Camera> _camera;
	std::vector<std::shared_ptr<Charactor>> _charactors;
	std::shared_ptr<PlayerCursor> _playerCursor;

	void PlayUpdate(const Input& input);
	void(PlayScene::*_updater)(const Input& input);

	void TurnReset(const Team turn);

public:
	PlayScene(SceneController & ctrl);
	~PlayScene();

	void Update(const Input& input) override final;
	void Draw(void) override final;
};
