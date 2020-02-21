#pragma once
#include "Scene.h"
#include <memory>

class MapCtrl;
class EditCursor;
class Camera;

class MapEditScene :
	public Scene
{
private:
	std::shared_ptr<MapCtrl> _mapCtrl;
	std::shared_ptr<EditCursor> _editCursor;
	std::shared_ptr<Camera> _camera;

public:
	MapEditScene(SceneController& controller);
	~MapEditScene();

	void Update(const Input& input)override;

	void Draw()override;
};

