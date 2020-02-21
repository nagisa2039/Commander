#pragma once
#include "Scene.h"
#include "../Utility/Geometry.h"

class PlayScene :
	public Scene
{
private:

	void PlayUpdate(const Input& input);

	void(PlayScene::*_updater)(const Input& input);

	bool debug;

	int _gameH;	// ƒQ[ƒ€‰æ–Ê‚Ìƒnƒ“ƒhƒ‹

public:
	PlayScene(SceneController & ctrl);
	~PlayScene();

	void Update(const Input& input) override final;
	void Draw(void) override final;
};
