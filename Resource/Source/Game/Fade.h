#pragma once
#include <memory>
#include "TimeLine.h"

class Fade
{
private:
	std::unique_ptr<Track<float>> _fadeTrack;
	unsigned int _fadeColor;

public:
	Fade();
	~Fade();

	void Update();
	void Draw();

	bool GetEnd()const;
	void StartFadeIn(const unsigned color = 0x000000);
	void StartFadeOut(const unsigned color = 0x000000);
};

