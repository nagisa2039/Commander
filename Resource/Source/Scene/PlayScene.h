#pragma once
#include <memory>
#include <list>
#include <vector>
#include "Scene.h"
#include "../Utility/Geometry.h"

class Charactor;
class MapCtrl;
class Camera;

class PlayScene :
	public Scene
{
private:
	bool debug;
	int _gameH;	// �Q�[����ʂ̃n���h��

	std::shared_ptr<MapCtrl> _mapCtrl;
	std::shared_ptr<Camera> _camera;
	std::vector<std::shared_ptr<Charactor>> _charactors;

	void PlayUpdate(const Input& input);
	void(PlayScene::*_updater)(const Input& input);

public:
	PlayScene(SceneController & ctrl);
	~PlayScene();

	void Update(const Input& input) override final;
	void Draw(void) override final;
};
