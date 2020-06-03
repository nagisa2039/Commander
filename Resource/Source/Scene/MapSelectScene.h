#pragma once
#include "Scene.h"
#include <vector>
#include "Geometry.h"
#include <memory>

class Camera;

class MapSelectScene :
    public Scene
{
private:
	std::vector<Vector2Int> _contentPosVec;
	std::unique_ptr<Camera> _camera;

	int _selectIdx;

public:
	MapSelectScene(SceneController& controller);
	~MapSelectScene();

	void Update(const Input& input)override;

	void Draw()override;
};

