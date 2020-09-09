#pragma once
#include <functional>
#include <memory>
#include "Scene.h"
#include "TimeLine.h"

class TitleScene :
    public Scene
{
private:
	// ‰æ‘œ
	int _bgH;
	int _start_bksH;
	// BGM
	int _bgmH;

	std::shared_ptr<Track_f> _animTrack;

	void(TitleScene::* _updater)(const Input&);

	std::function<void()> _fadeEndFunc;

	void NormalUpdate(const Input& input);
	void FadeUpdate(const Input& input);

public:
	TitleScene(SceneController& controller);
	~TitleScene();

	void Update(const Input& input);
	void Draw(void);
};