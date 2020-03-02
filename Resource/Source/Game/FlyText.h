#pragma once
#include "Effect.h"
#include "DxLibUtility.h"
#include <memory>

class FlyText :
	public Effect
{
private:
	int _lifeCnt;
	const int _lifeCntMax;
	Anker _anker;
	Vector2 _move;
	Vector2 _startPos;
	std::string _str;
	int _fontHandle;
	unsigned int _color;

public:
	FlyText(const std::string& str, const Vector2Int& pos, const int lifeCnt);
	~FlyText();

	void Update(const Input& input)override final;
	void Draw(const Camera& camera)override final;

	void SetAnker(const Anker anker);
	void SetMove(const Vector2& move);
	void SetFontHandle(const int handle);
	void SetColor(const unsigned int color);
};
