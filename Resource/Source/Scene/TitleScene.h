#pragma once
#include <functional>
#include <vector>
#include <memory>
#include "Scene.h"
#include "TimeLine.h"
#include "CharactorType.h"
#include "Team.h"

class TitleScene :
    public Scene
{
private:
	// ‰æ‘œ
	int _bgH;
	int _start_bksH;
	int _titleH;
	// BGM
	int _bgmH;

	struct CharInf
	{
		bool active;
		Vector2 pos;
		Team team;
		CharactorType type;
		float angle;
	};
	std::vector<CharInf> _charInfVec;
	std::unique_ptr<Track_f> _charCreateTrack;

	std::unique_ptr<Track_f> _animTrack;
	std::unique_ptr<Track_f> _demoSceneCnt;

	void(TitleScene::* _updater)(const Input&);

	std::function<void()> _fadeEndFunc;

	void NormalUpdate(const Input& input);
	void FadeUpdate(const Input& input);

public:
	TitleScene(SceneController& controller);
	~TitleScene();

	void Update(const Input& input);
	void CharactorUpdate();
	void Draw(void);
};