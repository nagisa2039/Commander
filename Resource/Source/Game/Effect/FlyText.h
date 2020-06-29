#pragma once
#include <memory>
#include "Effect.h"
#include "DxLibUtility.h"
#include "TimeLine.h"

class FlyText :
	public Effect
{
private:
	Anker _anker;
	Vector2 _move;
	Vector2 _startPos;
	std::string _str;
	int _fontHandle;
	unsigned int _color;
	bool _critical;

	std::unique_ptr<Track<float>> _moveTrack;
	std::unique_ptr<Track<float>> _alphaTrack;

public:
	FlyText(const std::string& str, const Vector2Int& pos, const int lifeCnt, Camera& camera, bool cameraActive, bool critical = false);
	~FlyText();

	void Update(const Input& input)override final;
	void Draw()override final;

	void SetAnker(const Anker anker);
	void SetMove(const Vector2& move);
	void SetFontHandle(const int handle);
	void SetColor(const unsigned int color);
	void SetStartPos(const Vector2& startPos);
};

