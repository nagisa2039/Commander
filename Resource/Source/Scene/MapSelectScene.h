#pragma once
#include "Scene.h"
#include <vector>
#include "Geometry.h"
#include <memory>
#include "TimeLine.h"

class Camera;
class MapSelectCharactor;

class MapSelectScene :
    public Scene
{
private:
	std::vector<Vector2Int> _contentPosVec;
	std::unique_ptr<Camera> _camera;
	std::vector<std::unique_ptr<MapSelectCharactor>> _mapSelectCharactors;

	int _selectIdx;

	std::unique_ptr<Track<int>> _moveStartTrack;
	std::vector<std::unique_ptr<MapSelectCharactor>>::iterator _moveStartItr;

public:
	MapSelectScene(SceneController& controller);
	~MapSelectScene();

	void Update(const Input& input)override;

	void MoveUpdate(const Input& input);

	void MapSelectCharactorUpdate();

	void CursorMove(const Input& input);

	void Draw()override;
};

