#pragma once
#include <vector>
#include <memory>
#include <deque>
#include <functional>
#include "Scene.h"
#include "Geometry.h"
#include "TimeLine.h"

class Camera;
class MapSelectCharactor;
class UI;

class MapSelectScene :
    public Scene
{
private:
	const Size _contentSize;
	std::vector<Vector2Int> _contentPosVec;
	std::unique_ptr<Camera> _camera;
	std::vector<std::unique_ptr<MapSelectCharactor>> _mapSelectCharactors;
	std::function<void()> _fadeEndFunc;
	std::deque<std::shared_ptr<UI>> _uiDeque;

	unsigned int _selectIdx;

	std::unique_ptr<Track<int>> _moveStartTrack;
	std::unique_ptr<Track<float>> _selectAnimTrack;
	unsigned int _charactorIdx;

	int _dir;
	int _bgmH;

	int _uiH;

	int _screenH;

	void(MapSelectScene::* _updater)(const Input&);

	void NormalUpdate(const Input& input);
	void FadeUpdate(const Input& input);

	void MoveUpdate(const Input& input);
	void CursorMove(const Input& input);

	void MapSelectCharactorUpdate();
	void Decide(const bool debug = false);

	void SetSelectIdx(const unsigned int value);

public:
	MapSelectScene(SceneController& controller);
	~MapSelectScene();

	void Update(const Input& input)override;

	void Draw()override;
};

