#pragma once
#include "Scene.h"
#include <vector>
#include "Geometry.h"
#include <memory>
#include "TimeLine.h"
#include <functional>

class Camera;
class MapSelectCharactor;

class MapSelectScene :
    public Scene
{
private:
	const Size _contentSize;
	std::vector<Vector2Int> _contentPosVec;
	std::unique_ptr<Camera> _camera;
	std::vector<std::unique_ptr<MapSelectCharactor>> _mapSelectCharactors;
	std::function<void()> _fadeEndFunc;

	unsigned int _selectIdx;

	std::unique_ptr<Track<int>> _moveStartTrack;
	unsigned int _charactorIdx;

	int _dir;
	int _bgmH;
	
	bool _debug;

	void(MapSelectScene::* _updater)(const Input&);

	void NormalUpdate(const Input& input);
	void FadeUpdate(const Input& input);

public:
	MapSelectScene(SceneController& controller);
	~MapSelectScene();

	void Update(const Input& input)override;

	void Decide(const bool debug = false);

	void MoveUpdate(const Input& input);

	void MapSelectCharactorUpdate();

	void CursorMove(const Input& input);

	void Draw()override;
};

