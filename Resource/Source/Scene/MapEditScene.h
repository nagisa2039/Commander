#pragma once
#include "Scene.h"
#include <memory>
#include <vector>

class MapCtrl;
class EditCursor;
class Camera;
class Charactor;

class MapEditScene :
	public Scene
{
private:
	std::shared_ptr<MapCtrl> _mapCtrl;
	std::shared_ptr<EditCursor> _editCursor;
	std::shared_ptr<Camera> _camera;

	std::vector<std::shared_ptr<Charactor>> _charactors;

public:
	MapEditScene(SceneController& controller);
	~MapEditScene();

	void Update(const Input& input)override;

	void Draw()override;
};

